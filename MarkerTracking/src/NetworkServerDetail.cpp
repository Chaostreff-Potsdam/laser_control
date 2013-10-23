#include "NetworkServerDetail.h"

#include <boost/date_time/local_time/local_time.hpp>

#include "Common.h"

#ifdef _DEBUG
#define DEACTIVATE_NETWORK_SERVER // deactivate network server in debug mode to circumvent a strange error
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace network
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// NetworkServerDetail
//
////////////////////////////////////////////////////////////////////////////////////////////////////

NetworkServerDetail::NetworkServerDetail(int port)
:	m_acceptor(m_ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
	startAccept();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkServerDetail::poll()
{
#ifndef DEACTIVATE_NETWORK_SERVER
	m_ioService.poll();
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkServerDetail::sendMarkerInformation(const markerTracking::MarkerPtr marker)
{
	if (!marker)
		return;

	if (!marker->isAlive())
		return;

#ifndef DEACTIVATE_NETWORK_SERVER
	auto currentTimeStamp = []() -> double
	{
		boost::posix_time::ptime time_t_epoch(boost::gregorian::date(1970, 1, 1));
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
		boost::posix_time::time_duration diff = now - time_t_epoch;
		return diff.total_milliseconds();
	};

	boost::asio::streambuf streamBuffer;
	std::ostream outputStream(&streamBuffer);

	const osg::Vec3 position = marker->referencePosition();
	const osg::Vec2 orientation = marker->referenceOrientation();

	const char tag = 32;
	const double timeStamp = currentTimeStamp();
	const char id = marker->id();
	const double x = position.x();
	const double y = position.y();
	const double angle = atan2(orientation[1], orientation[0]);

	outputStream.write((char*)&tag,	sizeof(tag));
	outputStream.write((char*)&timeStamp, sizeof(timeStamp));
	outputStream.write((char*)&id, sizeof(id));
	outputStream.write((char*)&x, sizeof(x));
	outputStream.write((char*)&y, sizeof(y));
	outputStream.write((char*)&angle, sizeof(angle));

	sendMessageToAllConnections(streamBuffer);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkServerDetail::handleWrite(const boost::system::error_code &error, size_t bytesTransferred)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkServerDetail::sendMessageToAllConnections(boost::asio::streambuf &streamBuffer)
{
#ifndef DEACTIVATE_NETWORK_SERVER
	m_connectionsMutex.lock();

	for (std::vector<boost::asio::ip::tcp::socket*>::iterator it = m_connections.begin();
		 it != m_connections.end();)
	{
		boost::system::error_code error;
		boost::asio::write(**it, streamBuffer, boost::asio::transfer_all(), error);

		if (error)
			it = m_connections.erase(it);
		else
			it++;
	}

	m_connectionsMutex.unlock();
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkServerDetail::startAccept()
{
#ifndef DEACTIVATE_NETWORK_SERVER
	boost::asio::ip::tcp::socket *socket = new boost::asio::ip::tcp::socket(m_ioService);
	m_acceptor.async_accept(*socket, boost::bind(&NetworkServerDetail::handleAccept, this, socket,
							boost::asio::placeholders::error));

	std::cout << "[Info] Accepting network clients" << std::endl;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkServerDetail::handleAccept(boost::asio::ip::tcp::socket *socket, const boost::system::error_code &error)
{
#ifndef DEACTIVATE_NETWORK_SERVER
	if (!error)
	{
		m_connectionsMutex.lock();
		m_connections.push_back(socket);
		m_connectionsMutex.unlock();

		std::cout << "[Info] Accepted new client" << std::endl;
	}

	startAccept();
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
