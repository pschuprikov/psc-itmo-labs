#include "packets.hpp"

#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include "messenger.h"

using namespace boost::asio;

messenger_t::messenger_t(session_manager_t &sm, discover_listener_t const& dl)
    : sm_(sm)
    , sv_(dl)
    , acceptor_(dl.service(), ip::tcp::endpoint(ip::tcp::v4(), 9999))
{
    start_accept();
}

void messenger_t::send_message(std::string const& msg, ip::address_v4 const& addr)
{
    send_data_ptr data(new send_data_t(acceptor_.get_io_service(), msg, addr));

    data->sock.async_connect(ip::tcp::endpoint(addr, 9999),
        boost::bind(&messenger_t::handle_connect, this, placeholders::error, data));
}

void messenger_t::start_accept( )
{
    accept_data_ptr data(new accept_data_t(acceptor_.get_io_service()));
    acceptor_.async_accept(data->sock, data->remote,
        boost::bind(&messenger_t::handle_accept, this, placeholders::error, data));
}

void messenger_t::handle_accept( boost::system::error_code const& err, accept_data_ptr recv )
{
    if (err)
    {
        std::cerr << err.message() << "\n";
        return;
    }

    message_pack msg;
    read_message(recv->sock, msg);
    std::cerr << "message from: " << recv->remote.address().to_string() << "\n";
    if (sv_.validate(session_t(msg.session_id)))
        std::cout << "received:\n" << msg.message << "\n";
    else
        std::cout << "validation failed\n";
}

void messenger_t::handle_connect(boost::system::error_code const& err, send_data_ptr data)
{
    if (err)
    {
        std::cerr << err.message() << "\n";
        return;
    }
    boost::optional<session_t> session = sm_[data->addr];
    if (!session)
    {
        std::cerr << "no session from " << data->addr << "\n";
        return;
    }

    message_pack msg;
    msg.session_id = session->id();
    msg.message = data->message;

    write_message(data->sock, msg);
}
