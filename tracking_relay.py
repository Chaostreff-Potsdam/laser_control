#!/usr/bin/env python

__all__ = ["Relay"]
__doc__ = """
A relay directly replaying a tracked position to the laserserver
"""

import struct
import socket
from Simulation import LaserClient, Player

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

	def mapPoint(self, *args):
		return map(self._t, args)

	def parsePoint(self, data):
		_, _, idx, x, y, _ = struct.unpack(self.trackingMsg, data)
		if idx == TRACKING_PLAYER_ID:
			laserPoints = [self.mapPoint(x, y)]
			self.playerId = self.laser.instruction(Player, laserPoints, self.playerId)

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
