#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>

std::string read_data_until(boost::asio::ip::tcp::socket& socket, int& indicator)
{
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, '!');
    std::string message;
    std::istream input_stream(&buffer);
    std::getline(input_stream, message, '!');
    if (message == "end")
    {
        indicator = 2;
    }
    else indicator = 1;
    return message;
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
        size_t count = 0;
        acceptor.bind(endpoint);
        acceptor.listen(size);
        boost::asio::ip::tcp::socket socket(io_service);

        acceptor.accept(socket);

        int indicator = 0;
        while (indicator != 2)
        {
            std::cout << read_data_until(socket, indicator) << std::endl;
        }
    }
    catch (boost::system::system_error& e)
    {
        std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
        return e.code().value();
    }
    system("pause");
    return EXIT_SUCCESS;
}