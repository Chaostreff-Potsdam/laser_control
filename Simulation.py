#!/usr/bin/env python -i
import struct
import socket
import itertools
import json
import getHolodeckEnum

"""
A point is a (int, int)-tuple
"""

globals().update(dict(
	(handler, idx) for idx, handler in enumerate(getHolodeckEnum.findHandlers())))

class LaserClient(object):

	def __init__(self, host="localhost", port=30000):
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
			packet.update({"id": instructionID})
		self.socket.sendto(json.dumps(packet), (self.host, self.port))

	def __packPoints(self, points):
		return [{"x": x, "y": y} for x, y in points]

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
	print """Created a UDP connection to %s:%d with object named \"laser\"
Usage:
\tlaser.deleteAll()
\tplayerID = laser.instruction(Player, [(100, 100)])
\tlaser.instruction(Player, [(1000, 1000)], playerID)""" % (host, port)

if __name__ == "__main__":
	import sys
	if sys.flags.inspect:
		host = sys.argv[1] if len(sys.argv) > 1 else "localhost"
		port = int(sys.argv[2]) if len(sys.argv) > 2 else 30000
		interact(host, port)
	else:
		print "Execute with \"python -i %s [host [port]]\"" % sys.argv[0]
