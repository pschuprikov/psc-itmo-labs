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
        try
        {
            if (cmd == "exit")
                break;
            else if (cmd == "listservers")
                ds.print_servers();
            else if (cmd == "getsession")
            {
                std::string remote;
                std::cin >> remote;
                sm.obtain_session_out(ip::address_v4::from_string(remote));
            } else if (cmd == "send")
            {
                std::string remote;
                std::string message;
                std::cin >> remote >> message;
                msg.send_message_out(message, ip::address_v4::from_string(remote));
            }
        }
        catch (std::exception const& ex)
        {
            std::cerr << ex.what() << "\n";
        }
    }
    ios.stop();
    t.join();
}
