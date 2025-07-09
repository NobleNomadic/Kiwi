// netcatcher.c - Packet sniffing and analysis tool
#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to handle each packet
void packetHandler(u_char *args, const struct pcap_pkthdr *header,
                   const u_char *packet) {
  int eth_offset = 14;

  // Read initial EtherType
  uint16_t ether_type = ntohs(*(uint16_t *)(packet + 12));

  // Adjust for VLAN tag if present
  if (ether_type == 0x8100) {
    ether_type = ntohs(*(uint16_t *)(packet + 16));
    eth_offset += 4;
  }

  // Only handle IP packets
  if (ether_type != ETH_P_IP) {
    return;
  }

  // IP header
  struct iphdr *ip = (struct iphdr *)(packet + eth_offset);
  if (ip->version != 4 || ip->ihl < 5)
    return;

  unsigned short iphdrlen = ip->ihl * 4;

  struct in_addr src, dest;
  src.s_addr = ip->saddr;
  dest.s_addr = ip->daddr;

  char src_ip[INET_ADDRSTRLEN];
  char dest_ip[INET_ADDRSTRLEN];

  inet_ntop(AF_INET, &(ip->saddr), src_ip, INET_ADDRSTRLEN);
  inet_ntop(AF_INET, &(ip->daddr), dest_ip, INET_ADDRSTRLEN);

  printf("\n[+] IP Packet: %s >> %s\n", src_ip, dest_ip);

  // Protocol-specific
  if (ip->protocol == IPPROTO_TCP) {
    struct tcphdr *tcp = (struct tcphdr *)(packet + eth_offset + iphdrlen);
    printf("    TCP Packet: src port: %d :: dst port: %d\n", ntohs(tcp->source),
           ntohs(tcp->dest));

    int tcphdrlen = tcp->doff * 4;
    int total_headers_size = eth_offset + iphdrlen + tcphdrlen;

    const u_char *payload = packet + total_headers_size;
    int payload_len = header->len - total_headers_size;

    if (payload_len > 0) {
      printf("    Payload (%d bytes):\n    ", payload_len);
      for (int i = 0; i < payload_len; i++) {
        printf("%c", isprint(payload[i]) ? payload[i] : '.');
      }
      printf("\n");
    }

  } else if (ip->protocol == IPPROTO_UDP) {
    struct udphdr *udp = (struct udphdr *)(packet + eth_offset + iphdrlen);
    printf("    UDP Packet: src port: %d :: dst port: %d\n", ntohs(udp->source),
           ntohs(udp->dest));

  } else {
    printf("    Other Protocol: %d\n", ip->protocol);
  }
}

// Entry point
int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./netcatcher <Packet count> [Filter]\n");
    exit(1);
  }

  char *filterExpression = "ip";
  if (argc >= 3) {
    filterExpression = argv[2];
  }

  int packetCount = atoi(argv[1]);

  char *device, errbuf[PCAP_ERRBUF_SIZE];
  pcap_t *pcapHandle;

  device = "wlp2s0";
  printf("[+] Using device: %s\n", device);

  // Promiscuous mode enabled
  pcapHandle = pcap_open_live(device, 65536, 1, 1000, errbuf);
  if (pcapHandle == NULL) {
    fprintf(stderr, "[-] Could not open %s: %s\n", device, errbuf);
    return 2;
  }

  // Make sure link layer is Ethernet
  if (pcap_datalink(pcapHandle) != DLT_EN10MB) {
    fprintf(stderr, "[-] Unsupported link layer. Ethernet only.\n");
    return 3;
  }

  struct bpf_program fp;
  if (pcap_compile(pcapHandle, &fp, filterExpression, 0,
                   PCAP_NETMASK_UNKNOWN) == -1) {
    fprintf(stderr, "[-] Failed to compile filter\n");
    return 2;
  }

  if (pcap_setfilter(pcapHandle, &fp) == -1) {
    fprintf(stderr, "[-] Could not apply filter\n");
    return 2;
  }

  pcap_loop(pcapHandle, packetCount, packetHandler, NULL);
  pcap_close(pcapHandle);
  return 0;
}
