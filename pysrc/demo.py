#!/usr/bin/env python

import time

import etherdream
import obj
import motive


scene = Scene()
fairydust = obj.LaserObject(motive.fairydust)

speed = 200
delay = 0.1

while True:
	fairydust.move(dy=-speed)
	scene.update()
	time.sleep(delay)
