#!/usr/bin/python3
import socket
import threading

def echoHandleClient():
	pass

def echoServerListen(listenerPort):
	listenerSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.settimeout(3)
	s.listen(5)


def triggerHandleClient():
	pass

def triggerServerListen(listenerPort):
	listenerSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.settimeout(3)
	s.listen(5)



def setupServer(listenerPort, serverFlag="echo", scriptPath="NONE"):
	if serverFlag == "echo":
		echoServerListen(listenerPort)

	elif serverFlag == "trigger":
		triggerServerListen(listenerPort)