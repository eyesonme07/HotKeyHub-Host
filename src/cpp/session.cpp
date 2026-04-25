#include<boost/json/src.hpp>
#include "../../src/header/session.h"

//тут забираем сокет к себе
Session::Session(boost::asio::ip::tcp::socket socket)
	: socket_(std::move(socket))
	, loadTimer_(socket_.get_executor()) {}


//начало инициализации сессии, отправляем клиенту статус подключения и запускаем таймер для загрузки данных(текущая песня громкость и тд)
void Session::start()
{
	boost::json::value connect_response = {
		{"status", "connected"}
	};
	sendJson(connect_response);

	recursiveTimer(loadTimer_);

	do_read_header();
}

//тут читаем сколько данных пришло(размер)
void Session::do_read_header()
{
	//получаем указатель на себя
	auto self = shared_from_this();

	//читаем первые 4 байта, в которых будет размер данных
	boost::asio::async_read(socket_, boost::asio::buffer(buffer_head_, 4),
		[this,self](const boost::system::error_code& error, const std::size_t bytes){
			//если ошибка
			if (error) {
				std::cout << "Client disconnected" << '\n';
				return;
			}

			//получаем размер данных, который нам нужно прочитать
			uint32_t net_length{};
			std::memcpy(&net_length, buffer_head_, 4);
			uint32_t length = ntohl(net_length);

			//меняем размер вектора под размер полученных данных
			buffer_data_.resize(length);
			//читаем уже готовый ответ
			do_read_body(length);
	});
}

void Session::do_read_body(std::size_t length)
{
	//получаем указатель на себя
	auto self = shared_from_this();

	//читаем данные в буфер
	boost::asio::async_read(socket_, boost::asio::buffer(buffer_data_.data(), length),
		[this,self](const boost::system::error_code& error, const std::size_t bytes){
			if (error) {
				std::cout << "Error reading body (error - " << error.what() << " )" << '\n';
				return;
			}
			//получаем строку из буфера
			std::string json_str(buffer_data_.begin(), buffer_data_.end());
			//парсим строку в json
			boost::json::value json_value = boost::json::parse(json_str);
			//обрабатываем json
			jsonParse_.parse(json_value);
			//начинаем читать заново
			do_read_header();
		});
}
//отправляем json обратно клиенту(статус и т.д.)
void Session::sendJson(const boost::json::value& value)
{
	// Сериализуем JSON в строку
	std::string body = boost::json::serialize(value);

	// Длина тела
	uint32_t length = static_cast<uint32_t>(body.size());
	uint32_t net_length = htonl(length);

	// Создаём буфер с 4-байтным заголовком (network byte order) + тело
	auto buffer = std::make_shared<std::vector<char>>(4 + length);
	std::memcpy(buffer->data(), &net_length, 4);
	if (length > 0) {
		std::memcpy(buffer->data() + 4, body.data(), length);
	}

	// Делаем асинхронную отправку. захватываем shared_ptr на буфер и self, чтобы данные и сессия оставались живы до завершения
	auto self = shared_from_this();
	boost::asio::async_write(socket_, boost::asio::buffer(*buffer),
		[this, self, buffer](const boost::system::error_code& error, const std::size_t bytes){
			if (error) {
				std::cout << "Error sending JSON (error - " << error.what() << " )" << std::endl;
				return;
			}
		});
}
//загружаем данные(текущая песня громкость и тд)
void Session::load_data()
{
	//получаем информацию о текущей песне
	boost::json::value mediaInfo = systemController_.GetMedia();
	
	//получаем текущую громкость
	float currentVolume{};
	systemController_.GetSystemVolume(&currentVolume);
	std::string pcName = systemController_.Get_PC_Name();


	sendJson({
		{"mediaInfo", mediaInfo},
		{"volume", currentVolume},
		{"pcName", pcName}
	});
}
//бесконечный таймер для загрузки данных(текущая песня громкость и тд)
void Session::recursiveTimer(boost::asio::steady_timer& timer)
{
	//установка таймера на 500 миллисекунд
	timer.expires_after(std::chrono::milliseconds(500));

	//запускаем
	timer.async_wait([&timer,this](const boost::system::error_code& ec) {
		if (ec) return;
		load_data();

		//рекурсия(бесконечность)
		recursiveTimer(timer);
	});
}
