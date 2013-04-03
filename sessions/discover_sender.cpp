#include <boost/bind.hpp>

#include "discover_sender.h"

using namespace boost::asio;
using namespace std::placeholders;

discover_sender_t::discover_sender_t(boost::asio::io_service &ios, boost::asio::ip::udp::endpoint dest)
    : socket_(ios, ip::udp::endpoint())
    , timer_(ios)
    , dest_(dest)
{
    if (dest_.address().is_multicast())
        socket_.set_option(ip::udp::socket::broadcast(true));
    start_send();
}

void discover_sender_t::start_send()
{
    static unsigned char const type = 0;
    socket_.async_send_to(buffer(&type, 1), dest_,
        boost::bind(&discover_sender_t::handle_send, this, placeholders::error));
}

void discover_sender_t::handle_send(boost::system::error_code const& err)
{
    if (err)
        std::cerr << err.message() << "\n";
    timer_.expires_from_now(boost::posix_time::seconds(1));
    timer_.async_wait(boost::bind(&discover_sender_t::start_send, this));
}
