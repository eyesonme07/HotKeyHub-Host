#include<iostream>
#include"../header/server.h"
#include<boost/asio.hpp>

int main() {
	setlocale(LC_ALL, "ru");
	try
	{
		boost::asio::io_context io;
		Server server(io, 5284);
		io.run();
	}
	catch (const std::exception& ex)
	{
		std::cout << "Error " << ex.what() << '\n';
	}
}