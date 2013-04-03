#include <boost/bind.hpp>

#include "packets.hpp"
#include "session_manager.h"

#include "global_params.h"

using namespace boost::asio;

session_manager_t::session_manager_t(discover_listener_t const& dl)
    : dl_(dl)
    , sock_(dl.service())
{}

boost::optional<session_t> session_manager_t::operator[](boost::asio::ip::address_v4 const& addr)
{
    if (sessions_.count(addr))
        return sessions_.at(addr);
    else
        return boost::none;
}

void session_manager_t::obtain_session(const ip::address_v4 &remote)
{
    if (dl_.servers().empty())
    {
        std::cerr << "no servers\n";
        return;
    }

    std::cerr << "connecting to " << *dl_.servers().begin() << "\n";

    sock_.connect(*dl_.servers().begin());

    session_req packet;
    packet.cname = g_hostaddr.to_string();
    packet.dest_addr = remote.to_string();

    write_message(sock_, packet);

    dispatch_message_read(sock_,
        std::function<void(session_ack)>([&] (session_ack ack)
        {
            std::cerr << "session_id : " << ack.session_id << " for " << ack.dest_addr << "\n";
            sessions_.insert(std::make_pair(remote, session_t(ack.session_id)));
        }),
        std::function<void(session_rej)>([&](session_rej rej)
        {
            std::cerr << "session request was rejected\n";
        })
    );

    sock_.close();
}

void session_manager_t::handle_connect(boost::system::error_code const& err, ip::address_v4 const& remote)
{
    if (err)
    {
        std::cerr << err.message() << "\n";
        return;
    }


}
