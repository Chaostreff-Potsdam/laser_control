#ifndef LASERSERVER_H
#define LASERSERVER_H

#include "LaserPainter.h"

#include <boost/asio.hpp>
#include <mutex>
#include <memory>

namespace laser {

	class LaserServer
	{
	public:
		LaserServer(LaserPainter &painter);

		void poll();

	protected:

		enum CommandType
		{
			INVALID = 0,
			DELETE,
			WALL,
			DOOR,
			TABLE
		};

		static unsigned int parseToInt(unsigned char *array, int at);
		void startAccept();
		void handleAccept(boost::asio::ip::tcp::socket *socket, const boost::system::error_code &error);
		void handleRead();
		void handleDelete();
		void handleWall();
		void handleTable();

		LaserPainter& m_painter;

		boost::asio::io_service m_ioService;
		boost::asio::ip::tcp::acceptor m_acceptor;
		std::vector<boost::asio::ip::tcp::socket*> m_connections;
		std::mutex m_connectionsMutex;
		std::mutex m_painterMutex;

		unsigned char m_buf[2048];
	};

}
#endif // LASERSERVER_H
