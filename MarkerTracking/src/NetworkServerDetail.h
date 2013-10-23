#pragma once

#define BOOST_ALL_NO_LIB

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "Marker.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace network
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// NetworkServerDetail
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class NetworkServerDetail
{
	public:
		NetworkServerDetail(int port);

		void sendMarkerInformation(const markerTracking::MarkerPtr marker);

		void poll();

	private:
		void startAccept();
		void handleAccept(boost::asio::ip::tcp::socket *socket, const boost::system::error_code &error);
		void handleWrite(const boost::system::error_code &error, size_t bytesTransferred);
		void sendMessageToAllConnections(boost::asio::streambuf &streamBuffer);

		boost::asio::io_service m_ioService;
		boost::asio::ip::tcp::acceptor m_acceptor;
		std::vector<boost::asio::ip::tcp::socket*> m_connections;
		boost::mutex m_connectionsMutex;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
