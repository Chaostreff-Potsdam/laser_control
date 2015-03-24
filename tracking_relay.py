#!/usr/bin/env python

__all__ = ["Relay"]
__doc__ = """
A relay directly replaying a tracked position to the laserserver
"""

import struct
import socket
import math
from Simulation import LaserClient, Player, Table

TRACKING_PLAYER_ID = 1

class Relay(object):
	
	trackingMsg = "<bdbddd"
	trackingMsgSz = struct.calcsize(trackingMsg)
	scale = 2**16 / 5.0

	def __init__(self, tracking, laser):
		"""Expects tuples of (host, port)"""

		self.tracking = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.tracking.connect(tracking)
		self.laser = LaserClient(*laser)
		self.playerId = -1

	def __del__(self):
		if self.playerId != -1:
			self.laser.delete(self.playerId)
		self.tracking.close()

	def _t(self, val):
		return self.scale * (val - 2.5)

	def mapPoint(self, point):
		x, y = point
		return self._t(x), -self._t(y)

	def corner(self, midX, midY, angle):
		diag_2 = 0.1767
		return midX + diag_2 * math.cos(angle), midY + diag_2 * math.sin(angle)

	def parsePoint(self, data):
		_, _, idx, x, y, angle = struct.unpack(self.trackingMsg, data)
		if idx == TRACKING_PLAYER_ID:
			#laserPoints = [self.mapPoint((x, y))]
			#self.playerId = self.laser.instruction(Player, laserPoints, self.playerId)

			laserPoints = map(self.mapPoint, (self.corner(x, y, math.pi * (i / 2.0 - 1 / 4.0) + angle) for i in range(4)))
			self.playerId = self.laser.instruction(Table, laserPoints, self.playerId, turkerIds=[-1])

	def run(self):
		while True:
			data = self.tracking.recv(self.trackingMsgSz)
			if len(data) < self.trackingMsgSz:
				break
			self.parsePoint(data)

if __name__ == "__main__":
	def splitHost(s, defaultPort):
		h, _, p = s.partition(":")
		return h, int(p or str(defaultPort))

	import sys
	if len(sys.argv) < 3:
		print("Usage: %s trackinghost[:port] laserhost[:port]" % sys.argv[0])
	else:
		relay = Relay(splitHost(sys.argv[1], 3100),
		              splitHost(sys.argv[2], 30001))
		relay.run()
