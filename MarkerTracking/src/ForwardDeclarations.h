#pragma once

#include <memory>

namespace alvar
{
	class MarkerData;
}

namespace markerTracking
{
	class Marker;
	typedef std::shared_ptr<Marker> MarkerPtr;
}

namespace network
{
	class NetworkServer;
	typedef std::shared_ptr<NetworkServer> NetworkServerPtr;
}
