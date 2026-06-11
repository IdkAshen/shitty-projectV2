#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>

class NetworkSocket {
private:
    std::string ip_address;
    int port;
    bool is_connected;

public:
    NetworkSocket(std::string ip, int p) 
        : ip_address(std::move(ip)), port(p), is_connected(false) {
        if (port < 0 || port > 65535) {
            throw std::out_of_range("Invalid network port assignment.");
        }
    }

    ~NetworkSocket() {
        if (is_connected) {
            std::cout << "[RAII Close] Safely closing connection to " << ip_address << "\n";
        }
    }

    void establish_connection() {
        is_connected = true;
        std::cout << "Connected securely to " << ip_address << ":" << port << "\n";
    }

    // Explicitly disallow copying to protect socket lifecycle integrity
    NetworkSocket(const NetworkSocket&) = delete;
    NetworkSocket& operator=(const NetworkSocket&) = delete;
};

int main() {
    try {
        // Enforce modern smart pointer ownership boundaries
        auto primary_socket = std::make_unique<NetworkSocket>("192.168.1.50", 8080);
        primary_socket->establish_connection();
        
    } catch (const std::exception& error) {
        std::cerr << "Fatal System Execution Error: " << error.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
