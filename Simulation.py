#!/usr/bin/env python -i
import struct
import socket
import itertools
import json

"""
A point is a (int, int)-tuple
"""

try:
	import getHolodeckEnum
	globals().update(dict(
		(handler, idx) for idx, handler in enumerate(getHolodeckEnum.findHandlers())))
except ImportError:
	Delete = 1
	DeleteAll = 2

defaultPort = 30001

class LaserClient(object):

	def __init__(self, host="localhost", port=30001):
		self.lastSendID = 0
		self.host = host
		self.port = port
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

	def __del__(self):
		self.socket.close()

	def __sendPacket(self, code, instructionID=None, other=None):
		packet = other.copy() if other is not None else {}
		packet.update({"instruction": code})
		
		if instructionID is not None:
			packet.update({"instance": instructionID})
		self.sendBytes(json.dumps(packet).encode())

	def __packPoints(self, points):
		return [{"x": x, "y": y} for x, y in points]

	def sendBytes(self, s):
		self.socket.sendto(s, (self.host, self.port))

	def delete(self, instructionID):
		self.__sendPacket(Delete, instructionID)

	def deleteAll(self):
		self.__sendPacket(DeleteAll)

	def instruction(self, code, points=None, instructionID=-1, turkerIds=None):
		if points is None:
			points = []
		if self.lastSendID < instructionID:
			self.lastSendID = instructionID + 1
		if instructionID == -1:
			self.lastSendID += 1
			instructionID = self.lastSendID
		if turkerIds is None:
			turkerIds = []

		self.__sendPacket(code, instructionID, 
				{"points": self.__packPoints(points),
				 "turkers": turkerIds})
		return instructionID

def interact(host, port):
	globals()["laser"] = LaserClient(host, port)
	print("""Created a UDP connection to %s:%d with object named \"laser\"
Usage:
\tlaser.deleteAll()
\tplayerID = laser.instruction(Player, [(100, 100)])
\tlaser.instruction(Player, [(1000, 1000)], playerID)""" % (host, port))

if __name__ == "__main__":
	import sys
	if sys.flags.inspect:
		host = sys.argv[1] if len(sys.argv) > 1 else "localhost"
		port = int(sys.argv[2]) if len(sys.argv) > 2 else defaultPort
		interact(host, port)
	else:
		print("Execute with \"python -i %s [host [port]]\"" % sys.argv[0])
