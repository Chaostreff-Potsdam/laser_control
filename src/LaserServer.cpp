#include "LaserServer.h"

#include "laser_utilities.h"
#include "LaserPainter.h"

#include <mutex>
#include <iostream>
#include <cstdint>
#include <boost/bind.hpp>

laser::LaserServer::LaserServer(LaserPainter &painter)
:	m_painter(painter),
	m_acceptor(m_ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 30000))
{
	std::lock_guard<std::mutex> lock(m_painterMutex);
	startAccept();
}

void laser::LaserServer::poll()
{
	std::cout << "LaserServer::poll" << std::endl;
	m_ioService.run();
}

unsigned int laser::LaserServer::parseToInt(unsigned char *array, int at)
{
	return ((array[at+3]<<24)|(array[at+2]<<16)|(array[at+1]<<8)|(array[at+0]<<0));
}

void laser::LaserServer::startAccept()
{
	std::cout << "LaserServer::startAccept" << std::endl;
	boost::asio::ip::tcp::socket *socket = new boost::asio::ip::tcp::socket(m_ioService);
	m_acceptor.async_accept(*socket, boost::bind(&LaserServer::handleAccept, this, socket,
												 boost::asio::placeholders::error));
}

void laser::LaserServer::handleAccept(boost::asio::ip::tcp::socket *socket, const boost::system::error_code &error)
{
	std::cout << "LaserServer::handleAccept" << std::endl;
	if (!error)
	{
		std::cout << "[Info] new client" << std::endl;
		m_connectionsMutex.lock();
		m_connections.push_back(socket);
		m_connectionsMutex.unlock();
		socket->async_receive(boost::asio::buffer(m_buf),
							  boost::bind(&LaserServer::handleRead, this));
	}

	startAccept();
}

void laser::LaserServer::handleRead()
{
	std::cout << "got " << (int)m_buf[0] << std::endl;
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
	default:
		break;
	}
}

void laser::LaserServer::handleDelete()
{
	int id = parseToInt(m_buf, 1);

	std::cout << "delete " << id << std::endl;

	std::lock_guard<std::mutex> lock(m_painterMutex);

	m_painter.deleteObject(id);
}

void laser::LaserServer::handleWall()
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

void laser::LaserServer::handleTable()
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
