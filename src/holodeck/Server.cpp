#include "Server.h"
#include "InstructionFactory.h"

#include "../Painter.h"

#include <mutex>
#include <iostream>
#include <cstdint>
#include <boost/bind.hpp>

namespace laser { namespace holodeck {

const std::vector<Server::Handler> Server::Handlers = {
	nullptr,
	&Server::handleDelete,
	&Server::handleDeleteAll,
	&Server::handleWall,
	&Server::handleTable,
	&Server::handlePlayer,
	&Server::handleButton,
	&Server::handleDoor,
	&Server::handleBeam,
	&Server::handleInActivePortal,
	&Server::handleActivePortal,
	&Server::handleZipline,
	&Server::handleCorpse,
	&Server::handleStool,
	&Server::handleWater,
	&Server::handlePoke,
	&Server::handleStomper,
	&Server::handleFootwear,
	&Server::handleHeat,
	&Server::handleElevator,
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
			(this->*Handlers[command])();
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

void Server::handleWall()
{
	int id = parseToInt(m_buf, 1);
	std::vector<Point> ps(readPoints(2));

	std::cout << "build " << id << std::endl;
	std::cout << "(" << ps[0].x() << "; " << ps[0].y() << ")" << std::endl;
	std::cout << "(" << ps[1].x() << "; " << ps[1].y() << ")" << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, InstructionFactory::wall(ps[0], ps[1]));
}

void Server::handleTable()
{
	int id = parseToInt(m_buf, 1);
	std::vector<Point> ps(readPoints(4));

	std::cout << "build Table " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, InstructionFactory::table(ps[0], ps[1], ps[2], ps[3]));
}

void Server::handlePlayer()
{
	int id = parseToInt(m_buf, 1);
	std::vector<Point> ps(readPoints(1));

	std::cout << "build Player " << id << std::endl;
	std::cout << ps[0].x() << ", " << ps[0].y() << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, InstructionFactory::player(ps[0]));
}

void Server::handleButton()
{
	int id = parseToInt(m_buf, 1);
	std::vector<Point> ps(readPoints(1));

	std::cout << "build Button " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, InstructionFactory::button(ps[0]));
}

void Server::handleDoor()
{
	int id = parseToInt(m_buf, 1);
	std::vector<Point> ps(readPoints(2));

	std::cout << "build Door " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, InstructionFactory::door(ps[0], ps[1]));
}

void Server::handleBeam()
{
	int id = parseToInt(m_buf, 1);
	std::vector<Point> ps(readPoints(2));

	std::cout << "build Beam " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, InstructionFactory::beam(ps[0], ps[1]));
}

void Server::handlePortal(bool active)
{
	int id = parseToInt(m_buf, 1);
	std::vector<Point> ps(readPoints(2));

	std::cout << "build Portal " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, InstructionFactory::portal(ps[0], ps[1], active));
}

void Server::handleZipline()
{
	int id = parseToInt(m_buf, 1);
	std::vector<Point> ps(readPoints(2));

	std::cout << "build Zipline " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, InstructionFactory::zipline(ps[0], ps[1]));
}

void Server::handleCorpse()
{
	int id = parseToInt(m_buf, 1);
	std::vector<Point> ps(readPoints(4));

	std::cout << "build Corpse " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, InstructionFactory::corpse(ps[0], ps[1], ps[2], ps[3]));
}

void Server::handleStool()
{
	int id = parseToInt(m_buf, 1);
	std::vector<Point> ps(readPoints(2));

	std::cout << "build Stool " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, InstructionFactory::stool(ps[0], ps[1]));
}

void Server::handleWater()
{
	int id = parseToInt(m_buf, 1);
	std::vector<Point> ps(readPoints(1));

	std::cout << "build Water " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, InstructionFactory::water(ps[0]));

}

void Server::handlePoke()
{
	int id = parseToInt(m_buf, 1);
	std::vector<Point> ps(readPoints(2));

	std::cout << "build Poke " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, InstructionFactory::poke(ps[0], ps[1]));
}

void Server::handleStomper()
{
	int id = parseToInt(m_buf, 1);
	std::vector<Point> ps(readPoints(2));

	std::cout << "build Stomper " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, InstructionFactory::stomper(ps[0], ps[1]));
}

void Server::handleFootwear()
{
	int id = parseToInt(m_buf, 1);
	std::vector<Point> ps(readPoints(1));

	std::cout << "build Footwear " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, InstructionFactory::footwear(ps[0]));
}

void Server::handleHeat()
{
	int id = parseToInt(m_buf, 1);
	std::vector<Point> ps(readPoints(1));

	std::cout << "build Heat " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);
    m_painter.add(id, InstructionFactory::heat(ps[0]));
}

void Server::handleElevator()
{
    int id = parseToInt(m_buf, 1);
    std::vector<Point> ps(readPoints(3));

    std::cout << "build Elevator " << id << std::endl;

    std::lock_guard<std::mutex> lock(m_painterMutex);
    m_painter.add(id, InstructionFactory::elevator(ps[0], ps[1], ps[2]));
}

}} // namespace laser::holodeck
