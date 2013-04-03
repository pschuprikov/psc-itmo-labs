#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "discover_listener.h"
#include "discover_sender.h"
#include "session_manager.h"
#include "messenger.h"

#include "global_params.h"

using namespace boost::asio;

int main()
{
    io_service ios;

    discover_listener_t ds(ios);
    discover_sender_t dsender(ios, ip::udp::endpoint(g_hostaddr, 9998));
    session_manager_t sm(ds);
    messenger_t msg(sm, ds);

    boost::thread t([&] { ios.run(); });

    std::string cmd;
    while (std::cin >> cmd)
    {
        if (cmd == "exit")
            break;
        else if (cmd == "listservers")
            ds.print_servers();
        else if (cmd == "getsession")
        {
            std::string remote;
            std::cin >> remote;
            sm.obtain_session(ip::address_v4::from_string(remote));
        }
        else if (cmd == "send")
        {
            std::string remote;
            std::string message;
            std::cin >> remote >> message;
            msg.send_message(message, ip::address_v4::from_string(remote));
        }
    }
    ios.stop();
    t.join();
}
