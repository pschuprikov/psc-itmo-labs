#include <iostream>
#include <boost/bind.hpp>

#include "lowlevel.hpp"
#include "discover_listener.h"

using namespace boost::asio;

discover_listener_t::discover_listener_t(boost::asio::io_service &service)
    : ios_(service)
    , socket_(service, ip::udp::endpoint(ip::udp::v4(), 9999))
{
    start_receive();
}

void discover_listener_t::start_receive()
{
    socket_.async_receive_from(buffer(buf_), tmp_remote_,
        boost::bind(&discover_listener_t::handle_receive, this,
            placeholders::error, placeholders::bytes_transferred));
}

void discover_listener_t::handle_receive(boost::system::error_code const& err,
    size_t bytes_transferred)
{
    if (err)
    {
        std::cerr << err.message() << "\n";
    }
    buffer_t::iterator cur = buf_.begin();

    unsigned char type;
    cur = read_uint(type, cur);

    if (type == 0)
        clients_.insert(ip::tcp::endpoint(tmp_remote_.address(), 9998));
    else
    {
        std::string address;
        cur = read_string<unsigned char>(address, cur);
        unsigned short port;
        cur = read_uint(port, cur);
        servers_.insert(ip::tcp::endpoint(ip::address_v4::from_string(address), port));
    }
    start_receive();
}

void discover_listener_t::print_servers() const
{
    ios_.post([&]{
        std::cerr << "severs:\n";
        for (endpoints_t::const_iterator it = servers().begin(); it != servers().end(); ++it)
        {
            std::cerr << *it << "\n";
        }
    });
}
