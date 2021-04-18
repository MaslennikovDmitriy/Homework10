#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>
#include <mutex>

std::mutex m_mutex;

void write_data(boost::asio::ip::tcp::socket& name_socket, boost::asio::ip::tcp::socket& message_socket, std::string name, int& indicator)
{
	std::string message;
	while (message != "end")
	{
		if (indicator == -1)
		{
			getline(std::cin, message);
			indicator++;
		}
		else
		{
			getline(std::cin, message);
			boost::asio::write(name_socket, boost::asio::buffer(name + ':'));
			boost::asio::write(message_socket, boost::asio::buffer(message + '@'));
		}
	}
}

void read_data_until(boost::asio::ip::tcp::socket& name_socket, boost::asio::ip::tcp::socket& message_socket, std::string my_name, int& indicator)
{
	while (indicator != 2)
	{
		static int ID = 0;
		boost::asio::streambuf name_buffer;
		boost::asio::read_until(name_socket, name_buffer, ':');
		std::string name;
		std::istream input_stream(&name_buffer);
		std::getline(input_stream, name, ':');
		if (my_name != name)
		{
			ID = 1;
		}
		if (ID == 1)
		{
			std::lock_guard <std::mutex> lock(m_mutex);
			boost::asio::streambuf buffer;
			boost::asio::read_until(message_socket, buffer, '@');
			std::string message;
			std::istream input_stream(&buffer);
			std::getline(input_stream, message, '@');
			if (message == "end")
			{
				indicator = 2;
			}
			else
			{
				std::cout << name + ": " + message << std::endl;
			}
			ID = 0;
		}
	}
}

int main()
{
	const std::size_t size = 30;
	auto port = 8000;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);
	boost::asio::io_service io_service;

	std::string name;
	std::cout << "Insert your name: ";
	std::cin >> name;
	int indicator = -1;
	try
	{
		boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

		acceptor.bind(endpoint);
		acceptor.listen(size);
		boost::asio::ip::tcp::socket name_socket(io_service);
		boost::asio::ip::tcp::socket message_socket(io_service);
		acceptor.accept(name_socket);
		acceptor.accept(message_socket);
		std::thread writer(write_data, std::ref(name_socket), std::ref(message_socket), name, std::ref(indicator));
		std::thread reader(read_data_until, std::ref(name_socket), std::ref(message_socket), name, std::ref(indicator));
		writer.join();
		reader.join();
	}
	catch (boost::system::system_error& e)
	{
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
		return e.code().value();
	}
	system("pause");
	return EXIT_SUCCESS;
}