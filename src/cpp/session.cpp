#include<boost/json/src.hpp>
#include "../../src/header/session.h"



Session::Session(boost::asio::ip::tcp::socket socket):socket_(std::move(socket)){}

void Session::start()
{
	do_read_header();
}

void Session::do_read_header()
{
	auto self = shared_from_this();

	boost::asio::async_read(socket_, boost::asio::buffer(buffer_head_, 4),
		[this,self](const boost::system::error_code& error, const std::size_t bytes){

			if (error) {
				std::cout << "Error reading header (error - " << error.what() << " )" << '\n';
				return;
			}

			uint64_t net_length{};
			std::memcpy(&net_length, buffer_head_, 4);
			uint64_t length = ntohl(net_length);

			buffer_data_.resize(length);
			do_read_body(length);
	});
}

void Session::do_read_body(std::size_t length)
{
	auto self = shared_from_this();

	boost::asio::async_read(socket_, boost::asio::buffer(buffer_data_.data(), length),
		[this,self](const boost::system::error_code& error, const std::size_t bytes){
			if (error) {
				std::cout << "Error reading body (error - " << error.what() << " )" << '\n';
				return;
			}
			std::string json_str(buffer_data_.begin(), buffer_data_.end());
			
			std::cout << "Получено сообщение от "<<socket_.remote_endpoint().address().to_string()<<"\nСООБЩЕНИЕ:\n" << json_str << std::endl;

			boost::json::value json_value = boost::json::parse(json_str);
			processJson(json_value);
			do_read_header();
		});
}

void Session::processJson(const boost::json::value & value)
{
	if (value.is_object())
	{
		const boost::json::object& obj = value.as_object();
		if (obj.contains("command"))
		{
			std::string command = obj.at("command").as_string().c_str();

			if (command == "play_pause")
			{
				systemController_.SendMediaKey(VK_MEDIA_PLAY_PAUSE);
			}
			else if (command == "next")
			{
				systemController_.SendMediaKey(VK_MEDIA_NEXT_TRACK);
			}
			else if (command == "previous")
			{
				systemController_.SendMediaKey(VK_MEDIA_PREV_TRACK);
			}
			else if (command == "volume_up")
			{
				float currentVolume;
				HRESULT hr = systemController_.GetSystemVolume(&currentVolume);

				if (SUCCEEDED(hr)) {

					currentVolume += 0.1f; // Увеличиваем громкость на 10%
					if (currentVolume > 1.0f) currentVolume = 1.0f; // Максимальная громкость - 100%
					hr = systemController_.SetSystemVolume(currentVolume);
					if (FAILED(hr)) {
						std::cout << "Failed to set volume (error - " << hr << " )" << std::endl;
					}
				}
			}
			else if (command == "volume_down")
			{
				float currentVolume;
				HRESULT hr = systemController_.GetSystemVolume(&currentVolume);
				if (SUCCEEDED(hr)) {
					currentVolume -= 0.1f; // Уменьшаем громкость на 10%
					if (currentVolume < 0.0f) currentVolume = 0.0f; // Минимальная громкость - 0%
					hr = systemController_.SetSystemVolume(currentVolume);
					if (FAILED(hr)) {
						std::cout << "Failed to set volume (error - " << hr << " )" << std::endl;
					}
				}
			}
		}
		else
		{
			std::cout << "command не найден!" << std::endl;
		}
	}
	else
	{
		std::cout << "Полученные данные не являются JSON объектом." << std::endl;
	}
}
