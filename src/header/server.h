#ifndef SERVER
#define SERVER

#include<boost/asio.hpp>
#include<iostream>
#include"session.h"

//основнок класс сервера
class Server {
public:
	//конструктор
	Server(boost::asio::io_context& io, short port);

	//получаем локальный IPv4 адрес, чтобы клиент мог подключиться к серверу по этому адресу
	std::string get_local_ipv4();

private:
	//тут начинаем принимать подключения
	void do_accept();

	boost::asio::ip::tcp::acceptor acceptor_;
};
#endif //SERVER