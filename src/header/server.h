#pragma once

#include<boost/asio.hpp>
#include<iostream>

//основнок класс сервера
class Server {
public:
	Server(boost::asio::io_context& io, short port);

private:
	//тут начинаем принимать подключения
	void do_accept();

	void do_read(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

	boost::asio::ip::tcp::acceptor acceptor_;

	char buffer_[1024];
};
