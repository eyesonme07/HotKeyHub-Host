#include<iostream>
#include"../header/server.h"
#include<boost/asio.hpp>
#include"../header/systemController.h"

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	SystemController systemController;

	systemController.GetMedia();

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