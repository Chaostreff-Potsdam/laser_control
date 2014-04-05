#!/usr/bin/env python
import os, inspect, re

holodeckServerFile = "src/holodeck/Server.cpp"
projectDir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))

def readCode():
	filename = os.path.join(projectDir, holodeckServerFile)
	with open(filename) as f:
		return f.readlines()

def findHandlers():
	inHandlerCode = False
	for line in readCode():
		if not inHandlerCode:
			inHandlerCode = "Server::Handlers = {" in line
			continue
		if "};" in line:
			return
		handler = line.strip()[:-2].partition(",")[0].partition("(")[2]
		if handler:
			yield handler

def camelCaseToEnum(name):
	s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
	return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).upper()

def printEnum():
	print "enum CommandType"
	print "{"
	print ",\n".join("\t%s = %d" % (camelCaseToEnum(handler), idx)
						for idx, handler in enumerate(findHandlers()))
	print "};"

if __name__ == "__main__":
	printEnum()
