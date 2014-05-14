import struct
import socket
import itertools

"""
A point is a (int, int)-tuple
"""

Delete = 1
DeleteAll = 2
Player = 3
Wall = 4
Door = 5
Table = 6
Button = 7
Beam = 8
PortalInactive = 9
PortalActive = 10
Zipline = 11
Corpse = 12
Stool = 13
Water = 14
Poke = 15
Stomper = 16
Footwear = 17
Heat = 18
Elevator = 19
Guardrail = 20

class LaserClient(object):

	def __init__(self, host="localhost", port=30000):
		self.lastSendID = 0
		self.host = host
		self.port = port
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

	def __del__(self):
		self.socket.close()

	def __sendPacket(self, fmt_string, *values):
		packet = struct.pack(fmt_string, *values)
		self.socket.sendto(packet, (self.host, self.port))

	def delete(self, instructionID):
		self.__sendPacket("!BI", Delete, instructionID)

	def deleteAll(self):
		self.__sendPacket("!B", DeleteAll)

	def instruction(self, code, points=None, instructionID=-1, turkerIds=None):
		if points is None:
			points = []
		if instructionID == -1:
			self.lastSendID += 1
			instructionID = self.lastSendID

		self.__sendPacket("!BIi" + "i" * 2 * len(points), 
				code, instructionID, 0,
				 *itertools.chain(*points))
		return instructionID

