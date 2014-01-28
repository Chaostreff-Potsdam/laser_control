#include "Server.h"

#include "laser_utilities.h"
#include "Painter.h"

#include <mutex>
#include <iostream>
#include <cstdint>
#include <boost/bind.hpp>

laser::Server::Server(Painter &painter)
:	//m_acceptor(m_ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 30000)),
	m_painter(painter),
	m_socket(m_ioService),
	m_localEndpoint(boost::asio::ip::udp::v4(), 30000),
	m_senderEndpoint(boost::asio::ip::address::from_string("192.168.1.102"), 30000)
{
	std::lock_guard<std::mutex> lock(m_painterMutex);

	m_socket.open(boost::asio::ip::udp::v4());
	m_socket.bind(m_localEndpoint);
	startAccept();
}

void laser::Server::poll()
{
	std::cout << "LaserServer::poll" << std::endl;
	m_ioService.run();
}

unsigned int laser::Server::parseToInt(unsigned char *array, int at)
{
	return ((array[at+3]<<24)|(array[at+2]<<16)|(array[at+1]<<8)|(array[at+0]<<0));
}

void laser::Server::startAccept()
{
	std::cout << "LaserServer::startAccept" << std::endl;
	m_socket.async_receive_from(boost::asio::buffer(m_buf),
							   m_senderEndpoint,
							   boost::bind(&Server::handleRead, this));
	//m_acceptor.async_accept(*socket, boost::bind(&LaserServer::handleAccept, this, socket,
												// boost::asio::placeholders::error));
}

void laser::Server::handleAccept(boost::asio::ip::tcp::socket *socket, const boost::system::error_code &error)
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

void laser::Server::handleRead()
{
	std::cout << "LaserServer::handleRead " << (int)m_buf[0] << std::endl;
	switch (m_buf[0]) {
	case CommandType::DELETE:
		handleDelete();
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
	default:
		break;
	}

	startAccept();
}

void laser::Server::handleDelete()
{
	int id = parseToInt(m_buf, 1);

	std::cout << "delete " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);

	m_painter.deleteObject(id);
}

void laser::Server::handleWall()
{
	int id = parseToInt(m_buf, 1);

	std::cout << "build " << id << std::endl;

	std::vector<Point> ps;

	for (int i = 0; i < 2; ++i) {
		ps.push_back(Point(parseToInt(m_buf, 8*i+5),
						   -parseToInt(m_buf, 8*i+9)));
	}

	std::cout << "(" << ps[0].x() << "; " << ps[0].y() << ")" << std::endl;
	std::cout << "(" << ps[1].x() << "; " << ps[1].y() << ")" << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);

	m_painter.drawWall(id, ps[0], ps[1]);
}

void laser::Server::handleTable()
{
	int id = parseToInt(m_buf, 1);

	std::cout << "build Table " << id << std::endl;

	std::vector<Point> ps;

	for (int i = 0; i < 4; ++i) {
		ps.push_back(Point(parseToInt(m_buf, 8*i+5),
						   -parseToInt(m_buf, 8*i+9)));
	}

	std::lock_guard<std::mutex> lock(m_painterMutex);

	m_painter.drawTable(id, ps[0], ps[1], ps[2], ps[3]);
}

void laser::Server::handlePlayer()
{
	int id = parseToInt(m_buf, 1);

	std::cout << "build Player " << id << std::endl;

	std::vector<Point> ps;

	for (int i = 0; i < 1; ++i) {
		ps.push_back(Point(parseToInt(m_buf, 8*i+5),
						   -parseToInt(m_buf, 8*i+9)));
	}

	std::cout << ps[0].x() << ", " << ps[0].y() << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);

	m_painter.drawPlayer(id, ps[0]);
}

void laser::Server::handleButton()
{
	int id = parseToInt(m_buf, 1);

	std::cout << "build Button " << id << std::endl;

	std::vector<Point> ps;

	for (int i = 0; i < 1; ++i) {
		ps.push_back(Point(parseToInt(m_buf, 8*i+5),
						   -parseToInt(m_buf, 8*i+9)));
	}

	std::lock_guard<std::mutex> lock(m_painterMutex);

	m_painter.drawButton(id, ps[0]);
}
