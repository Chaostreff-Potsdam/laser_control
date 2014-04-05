#include "Server.h"
#include "InstructionFactory.h"

#include <iostream>
#include <boost/bind.hpp>

namespace laser { namespace holodeck {

#define HANDLER(Name) \
	[](Server *s) { \
		s->handle##Name(); \
	}

#define HANDLE_OBJECT(ObjectT, num_points) \
	[](Server *s) { \
		s->createObject<num_points>(#ObjectT, InstructionFactory::ObjectT); \
	}

#define EMPTY_LINE(ObjectT, num_points) \
	[](Server *) {}

const std::vector<Server::Handler> Server::Handlers = {
	EMPTY_LINE(Invalid, dummy),

	HANDLER(Delete),
	HANDLER(DeleteAll),
	HANDLE_OBJECT(Player, 1),
	HANDLE_OBJECT(Wall, 2),
	HANDLE_OBJECT(Door, 2),
	HANDLE_OBJECT(Table, 4),
	HANDLE_OBJECT(Button, 1),
	HANDLE_OBJECT(Beam, 2),
	HANDLE_OBJECT(PortalInactive, 2),
	HANDLE_OBJECT(PortalActive, 2),
	HANDLE_OBJECT(Zipline, 2),
	HANDLE_OBJECT(Corpse, 4),
	HANDLE_OBJECT(Stool, 2),
	HANDLE_OBJECT(Water, 1),
	HANDLE_OBJECT(Poke, 2),
	HANDLE_OBJECT(Stomper, 2),
	HANDLE_OBJECT(Footwear, 1),
	HANDLE_OBJECT(Heat, 1),
	HANDLE_OBJECT(Elevator, 3)
};

Server::Server(Painter &painter)
:	m_painter(painter),
	m_socket(m_ioService),
	m_localEndpoint(boost::asio::ip::udp::v4(), 30000),
	m_senderEndpoint(boost::asio::ip::address::from_string("192.168.1.112"), 30000)
{
	std::lock_guard<std::mutex> lock(m_painterMutex);

	m_socket.open(boost::asio::ip::udp::v4());
	m_socket.bind(m_localEndpoint);
	startAccept();
}

void Server::poll()
{
	std::cout << "LaserServer::poll" << std::endl;
	m_ioService.run();
}

unsigned int Server::parseToInt(unsigned char *array, int at)
{
	return ((array[at+3]<<24)|(array[at+2]<<16)|(array[at+1]<<8)|(array[at+0]<<0));
}

void Server::startAccept()
{
	std::cout << "LaserServer::startAccept" << std::endl;
	m_socket.async_receive_from(boost::asio::buffer(m_buf),
							   m_senderEndpoint,
							   boost::bind(&Server::handleRead, this));
}

void Server::handleAccept(boost::asio::ip::tcp::socket *socket, const boost::system::error_code &error)
{
	std::cout << "LaserServer::handleAccept" << std::endl;
	if (!error)
	{
		std::cout << "[Info] new client" << std::endl;
		m_connectionsMutex.lock();
		m_connections.push_back(socket);
		m_connectionsMutex.unlock();
		socket->async_receive(boost::asio::buffer(m_buf),
							  boost::bind(&Server::handleRead, this));
	}

	startAccept();
}

void Server::handleRead()
{
	unsigned int command = (unsigned int)m_buf[0];
	std::cout << "LaserServer::handleRead " << command  << std::endl;
	if (0 < command && command < Handlers.size())
			Handlers[command](this);
	startAccept();
}

std::vector<Point> Server::readPoints(int n)
{
	std::vector<Point> ps;

	for (int i = 0; i < n; ++i) {
		ps.push_back(Point(parseToInt(m_buf, 8*i+5),
						   -parseToInt(m_buf, 8*i+9)));
	}
	return ps;
}

void Server::handleDelete()
{
	int id = parseToInt(m_buf, 1);

	std::cout << "delete " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.deleteObject(id);
}

void Server::handleDeleteAll()
{
	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.deleteAll();
}

void Server::addObjectToPainter(const int id, const char *name, const ObjectPtr &object)
{
	std::cout << "build " << name << " " << id << std::endl;
	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, object);
}

}} // namespace laser::holodeck
