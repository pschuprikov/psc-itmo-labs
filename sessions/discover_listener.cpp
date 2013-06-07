#include <iostream>
#include <boost/bind.hpp>

#include "lowlevel.hpp"
#include "discover_listener.h"
#include "global_params.h"

using namespace boost::asio;

discover_listener_t::discover_listener_t(boost::asio::io_service &service)
    : ios_(service)
    , socket_(service, ip::udp::endpoint(ip::udp::v4(), g_udp_listen_port))
{
    start_receive();
}

void discover_listener_t::start_receive()
{
    socket_.async_receive_from(buffer(buf_), tmp_remote_,
        boost::bind(&discover_listener_t::handle_receive, this,
            placeholders::error, placeholders::bytes_transferred));
}

void discover_listener_t::handle_receive(boost::system::error_code const& err, size_t)
{
    if (err)
        std::cerr << err.message() << "\n";
    auto cur = buf_.begin();

    unsigned char type;
    cur = read_uint(type, cur);

    if (type == 0)
    {
        ip::tcp::endpoint ep(tmp_remote_.address(), g_tcp_send_port);
        clients_.insert(std::make_pair(ep.address().to_v4(), ep));
    }
    else
    {
        std::string address;
        cur = read_string<unsigned char>(address, cur);
        unsigned short port;
        cur = read_uint(port, cur);
        ip::tcp::endpoint ep(ip::address_v4::from_string(address), port);
        servers_.insert(std::make_pair(ep.address().to_v4(), ep));
    }

    start_receive();
}

void discover_listener_t::print_servers_out() const
{
    ios_.post([&]{
        std::cerr << "severs:\n";
        for (endpoints_t::const_iterator it = servers().begin(); it != servers().end(); ++it)
            std::cerr << it->second << "\n";
    });
}

void discover_listener_t::print_clients_out() const
{
    ios_.post([&]{
        std::cerr << "clients:\n";
        for (endpoints_t::const_iterator it = clients().begin(); it != clients().end(); ++it)
            std::cerr << it->second << "\n";
    });
}
