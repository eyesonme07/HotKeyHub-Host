#include"../header/server.h"

//привязываем коннетор к порту
Server::Server(boost::asio::io_context& io, short port)
	: acceptor_(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port )) {

	std::cout << "Server is running on port - " << port << '\n';

	do_accept();
}

//тут начинаем принимать подключения
void Server::do_accept()
{
	auto socket = std::make_shared<boost::asio::ip::tcp::socket>(acceptor_.get_executor());

	//начинаем примнмаем подключения
	acceptor_.async_accept(*socket, [this, socket](const boost::system::error_code &ec) {
		if (!ec) {
			std::cout << "New Connection (host - " <<socket.get()->remote_endpoint().address().to_string()<<")"<< '\n';
			//запускаем сессию для нового подключения
			std::make_shared<Session>(std::move(*socket))->start();
		}
		else
			std::cout << "Connection error (error - " << ec.what() << " )" << '\n';

		// рекурсивно начинаем принимать подключения, чтобы сервер не останавливался после первого подключения
		do_accept();
	});
}