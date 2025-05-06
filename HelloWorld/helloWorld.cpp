#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_service io_service;

        // Resolve the server address and port
        tcp::resolver resolver(io_service);
        auto endpoints = resolver.resolve({"192.168.100.67", "8080"}); // Replace with your ESP32's IP and port

        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoints);

        std::string message = "Hello from C++ client!";
        boost::asio::write(socket, boost::asio::buffer(message));

        char reply[128];
        size_t reply_length = boost::asio::read(socket, boost::asio::buffer(reply));
        std::cout << "Received: ";
        std::cout.write(reply, reply_length);
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
