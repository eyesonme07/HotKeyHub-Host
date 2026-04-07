#ifndef SERVER
#define SERVER

#include<boost/asio.hpp>
#include<iostream>
#include"session.h"

//основнок класс сервера
class Server {
public:
	Server(boost::asio::io_context& io, short port);

private:
	//тут начинаем принимать подключения
	void do_accept();

	boost::asio::ip::tcp::acceptor acceptor_;
};
#endif //SERVER