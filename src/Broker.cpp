
#include "../include/Application.h"
#include "../include/Broker.h"

int main() {
    try {
        net::io_context ioc; // need to pass this to constructor of Broker!!

        // Buffer to hold incoming messages
        beast::flat_buffer buffer;

        // Subscribe to SPY trades
        json::object subscribe_payload = {
            {"action", "subscribe"},
            {"trades", {"FAKEPACA"}}
        };
        ws.write(net::buffer(json::serialize(subscribe_payload)));

        

        // Read messages in a loop and print them to stdout
        while (true) {
            buffer.consume(buffer.size());
            ws.read(buffer);
            std::cout << beast::make_printable(buffer.data()) << std::endl;
        }

        // Close the WebSocket connection
        ws.close(websocket::close_code::normal);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}