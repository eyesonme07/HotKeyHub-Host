#include"../header/server.h"

//привязываем коннетор к порту
Server::Server(boost::asio::io_context& io, short port)
	: acceptor_(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port )) {

	std::cout << "Server is running on port - " << port << '\n';
	std::cout << "Connect your client to this IP - " << get_local_ipv4() << '\n';

	//начинаем принимать подключения
	do_accept();
}

//тут получаем локальный IPv4 адрес, чтобы клиент мог подключиться к серверу по этому адресу
std::string Server::get_local_ipv4()
{
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);

    auto results = resolver.resolve(boost::asio::ip::host_name(), "");

    for (const auto& entry : results)
    {
        const auto& addr = entry.endpoint().address();
        // только верии 4
        if (addr.is_v4())
        {
            auto v4 = addr.to_v4();
            // пропускаем 127.0.0.1
            if (v4.is_loopback())
                continue;

            // Проверка на приватный адрес (10.x, 172.16-31.x, 192.168.x)
            uint32_t ip = v4.to_uint();

            if ((ip & 0xFF000000) == 0x0A000000 ||        // 10.0.0.0/8
                (ip & 0xFFF00000) == 0xAC100000 ||        // 172.16.0.0/12
                (ip & 0xFFFF0000) == 0xC0A80000)          // 192.168.0.0/16
            {
                return v4.to_string();
            }
        }
    }
    return "Не найден подходящий IPv4";
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