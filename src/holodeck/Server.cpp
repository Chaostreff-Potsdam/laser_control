#include "Server.h"
#include "InstructionFactory.h"

#include "../Painter.h"

#include <mutex>
#include <iostream>
#include <cstdint>
#include <boost/bind.hpp>

namespace laser { namespace holodeck {

Server::Server(Painter &painter)
:	//m_acceptor(m_ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 30000)),
	m_painter(painter),
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
	//m_acceptor.async_accept(*socket, boost::bind(&LaserServer::handleAccept, this, socket,
												// boost::asio::placeholders::error));
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
	std::cout << "LaserServer::handleRead " << (int)m_buf[0] << std::endl;
	switch (m_buf[0]) {
	case CommandType::DELETE:
		handleDelete();
		break;
	case CommandType::DELETE_ALL:
		handleDeleteAll();
		break;
	case CommandType::WALL:
		handleWall();
		break;
	case CommandType::TABLE:
		handleTable();
		break;
	case CommandType::PLAYER:
		handlePlayer();
		break;
	case CommandType::BUTTON:
		handleButton();
		break;
	case CommandType::DOOR:
		handleDoor();
		break;
	case CommandType::BEAM:
		handleBeam();
		break;
	case CommandType::PORTAL_INACTIVE:
		handlePortal(false);
		break;
	case CommandType::PORTAL_ACTIVE:
		handlePortal(true);
		break;
	case CommandType::ZIPLINE:
		handleZipline();
		break;
	case CommandType::CORPSE:
		handleCorpse();
		break;
	case CommandType::STOOL:
		handleStool();
		break;
	case CommandType::WATER:
		handleWater();
		break;
	case CommandType::POKE:
		handlePoke();
		break;
	case CommandType::STOMPER:
		handleStomper();
		break;
	default:
		break;
	}

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

}} // namespace laser::holodeck
