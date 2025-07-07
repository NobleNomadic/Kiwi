// netcatcher.c - Packet sniffing and analysis tool
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>

// Function to handle each packet
void packetHandler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    // Cast the Ethernet header
    struct ethhdr *eth = (struct ethhdr *)packet;

    // Only process IP packets
    if (ntohs(eth->h_proto) != ETH_P_IP) {
        return;
    }

    // Cast the IP header
    struct iphdr *ip = (struct iphdr *)(packet + sizeof(struct ethhdr));
    unsigned short iphdrlen = ip->ihl * 4;

    // Source and destination IP addresses
    struct in_addr src, dest;
    src.s_addr = ip->saddr;
    dest.s_addr = ip->daddr;

    printf("\n[+] IP Packet: %s >> %s\n", inet_ntoa(src), inet_ntoa(dest));

    // Check protocol
    if (ip->protocol == IPPROTO_TCP) {
        // Cast the TCP header
        struct tcphdr *tcp = (struct tcphdr *)(packet + sizeof(struct ethhdr) + iphdrlen);
        printf("    TCP Packet: src port: %d :: dst port: %d\n", ntohs(tcp->source), ntohs(tcp->dest));

        // TCP payload analysis
        // Calculate payload offset and length
        int ethhdr_len = sizeof(struct ethhdr);
        int tcphdrlen = tcp->doff * 4;
        int total_headers_size = ethhdr_len + iphdrlen + tcphdrlen;

        const u_char *payload = packet + total_headers_size;
        int payload_len = header->len - total_headers_size;

        // Print payload if it exists
        if (payload_len > 0) {
            printf("    Payload (%d bytes):\n    ", payload_len);
            for (int i = 0; i < payload_len; i++) {
                printf("%c", isprint(payload[i]) ? payload[i] : '.');
            }
            printf("\n");
        }

    // UDP packet analysis
    } else if (ip->protocol == IPPROTO_UDP) {
        // Cast the UDP header
        struct udphdr *udp = (struct udphdr *)(packet + sizeof(struct ethhdr) + iphdrlen);
        printf("    UDP Packet: src port: %d :: dst port: %d\n", ntohs(udp->source), ntohs(udp->dest));

    // Misc protocols
    } else {
        printf("    Other Protocol: %d\n", ip->protocol);
    }
}

// Entry point
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./netcatcher <Packet count>\n");
        exit(1);
    }

    int packetCount = atoi(argv[1]);

    char *device, errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcapHandle;

    // Find a device
    device = pcap_lookupdev(errbuf);
    if (device == NULL) {
        fprintf(stderr, "[-] Could not find default device: %s\n", errbuf);
        return 1;
    }

    printf("[+] Using device: %s\n", device);

    // Open the device
    pcapHandle = pcap_open_live(device, 65536, 1, 1000, errbuf);
    if (pcapHandle == NULL) {
        fprintf(stderr, "[-] Could not open %s: %s\n", device, errbuf);
        return 2;
    }

    // Start capturing
    pcap_loop(pcapHandle, packetCount, packetHandler, NULL);

    pcap_close(pcapHandle);
    return 0;
}
