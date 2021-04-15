#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include <boost/asio.hpp>

std::string read_data(boost::asio::ip::tcp::socket& socket)
{
    const std::size_t length = 10;
    char buffer[length];
    boost::asio::read(socket, boost::asio::buffer(buffer, length));
    return std::string(buffer, length);
}

std::string read_data_until(boost::asio::ip::tcp::socket& socket)
{
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, '!');
    std::string message;
    std::istream input_stream(&buffer);
    std::getline(input_stream, message);
    return message;
}

void auto_read_data_until(boost::asio::ip::tcp::socket& socket)
{
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, '!');
    std::string message;
    std::istream input_stream(&buffer);
    std::getline(input_stream, message);
    std::cout << message << std::endl;
}

int main()
{
    const std::size_t size = 30;

    auto port = 3333;

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);

    boost::asio::io_service io_service;

    try
    {
        boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

        acceptor.bind(endpoint);

        acceptor.listen(size);

        boost::asio::ip::tcp::socket socket(io_service);
        acceptor.accept(socket);
        while (read_data_until(socket) != "end")
        {
            auto_read_data_until(socket);
        }
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