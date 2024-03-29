#!/usr/bin/env python2

from parser import ILDA

classSkeleton = """#pragma once

#include "../src/objects/Object.h"

namespace laser {
	class EXPORT_LASER_CONTROL FileObject: public Object
	{
	public:
		FileObject() : Object()
		{
			%s
		}

		EtherdreamPoints startPoints() const
		{ return EtherdreamPoints(); }
		EtherdreamPoints endPoints() const
		{ return EtherdreamPoints(); }
		EtherdreamPoints points() const
		{ return m_ps; }

	private:
		EtherdreamPoints m_ps;
	};
}
"""



def generateConstructor(pointsList):
	lines = ["m_ps.reserve(%d);" % len(pointsList)]
	lines.extend(("m_ps.push_back(etherdreamPoint(%d, %d, %s));"
						% (p.x, p.y, str(p.visible).lower())
						  for p in pointsList))
	return "\n\t\t\t".join(lines)

def generateClass(points):
	return classSkeleton % generateConstructor(list(points))

def main(filename):
	doc = ILDA(open(filename).read())
	print generateClass(doc.chunks[0])

if __name__ == "__main__":
	import sys
	if len(sys.argv) > 1:
		main(sys.argv[1])
	else:
		print "Usage: %s FILE > FileObject.h" % sys.argv[0]

