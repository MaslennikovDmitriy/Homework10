#include <iostream>
#include <boost/asio.hpp>

void write_data(boost::asio::ip::tcp::socket& socket, std::string name) // принимаем сокет
{
	std::string message;
	while (message != "end!")
	{
		getline(std::cin, message);
		boost::asio::write(socket, boost::asio::buffer(name + ": " + message));
	}
}

int main()
{
	std::string raw_ip_address = "127.0.0.1";
	auto port = 3333;

	std::string name;
	std::cout << "Insert your name: ";
	std::cin >> name;

	try
	{
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(raw_ip_address), port);

		boost::asio::io_service io_service;

		boost::asio::ip::tcp::socket socket(io_service, endpoint.protocol());

		socket.connect(endpoint);

		write_data(socket, name);
	}
	catch (boost::system::system_error& e)
	{
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;

		system("pause");

		return e.code().value();
	}

	system("pause");

	return EXIT_SUCCESS;
}