#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "discover_listener.h"
#include "discover_sender.h"

using namespace boost::asio;

int main()
{
    io_service ios;

    discover_listener_t ds(ios);
    discover_sender_t dsender(ios, ip::udp::endpoint(ip::address_v4::from_string("192.168.1.36"), 9998));


    boost::thread t([&] { ios.run(); });

    std::string cmd;
    while (std::cin >> cmd)
    {
        if (cmd == "exit")
            break;
        if (cmd == "listservers")
            ds.print_servers();
    }
    ios.stop();
    t.join();
}
