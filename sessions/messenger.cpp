#include "packets.hpp"

#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>

#include "messenger.h"
#include "global_params.h"

using namespace boost::asio;

namespace
{
struct send_connection_t
    : boost::enable_shared_from_this<send_connection_t>
{
    send_connection_t(io_service& ios, session_manager_t& sm, std::string const& msg, ip::address_v4 const& addr )
        : sm_(sm), sock_(ios) , msg_(msg), addr_(addr)
    {}

    void start(boost::optional<ip::address_v4> server)
    {
        std::cerr << "initializing send\n";
        sm_.obtain_session(addr_, boost::bind(&send_connection_t::handle_got_session,
            shared_from_this(), _1), server);
    }


private:
    void handle_got_session(session_t session)
    {
        session_ = session;
        sock_.async_connect(ip::tcp::endpoint(addr_, g_tcp_send_port),
            bind(&send_connection_t::handle_connect,
                 shared_from_this(), placeholders::error));
    }

    void handle_connect(boost::system::error_code const& err)
    {
        if (err)
        {
            std::cerr << err.message() << "\n";
            return;
        }

        packet_.message = msg_;
        packet_.session_id = session_->id();

        std::cerr << "sending\n" << msg_ << "\n for " << addr_ << "; sid " << packet_.session_id << "\n";

        write_message(sock_, packet_,
            bind(&send_connection_t::handle_write, shared_from_this()));
    }

    void handle_write( )
    {
        std::cerr << "message for " << addr_ << " was sent\n";
    }

private:
    boost::optional<session_t> session_;
    message_pack               packet_;
    session_manager_t&         sm_;
    ip::tcp::socket            sock_;
    std::string                msg_;
    ip::address_v4             addr_;
};
typedef boost::shared_ptr<send_connection_t> send_connection_ptr;

struct receive_connection_t
    : boost::enable_shared_from_this<receive_connection_t>
{
    receive_connection_t(session_validator_t& sv, messenger_t::accept_data_ptr data )
        : sv_(sv), data_(data)
    {}

    void start()
    {
        read_message(data_->sock, packet_,
            boost::bind(&receive_connection_t::handle_red, shared_from_this()));
    }


private:
    void handle_red()
    {
        data_->sock.close();
        std::cerr << "got incoming packet: sid=" <<  packet_.session_id << " msg=\"" << packet_.message << "\"\n";
        sv_.validate(packet_.session_id,
            boost::bind(&receive_connection_t::handle_session_checked, shared_from_this(), _1));
    }

    void handle_session_checked(bool valid)
    {
        if (!valid)
        {
            std::cerr << "not valid session from " << data_->remote << "\n";
            return;
        }
        else
            std::cout << "got message from: " << data_->remote << "\n" << packet_.message << "\n";
    }

private:
    boost::optional<session_t> session_;
    message_pack packet_;
    session_validator_t& sv_;
    messenger_t::accept_data_ptr data_;
};
typedef boost::shared_ptr<receive_connection_t> receive_connection_ptr;

}

messenger_t::messenger_t(session_manager_t &sm, discover_listener_t const& dl)
    : sm_(sm)
    , sv_(dl)
    , acceptor_(dl.service(), ip::tcp::endpoint(ip::tcp::v4(), g_tcp_listen_port))
{
    start_accept();
}

void messenger_t::send_message_out(std::string const& msg, ip::address_v4 const& addr, boost::optional<ip::address_v4> server)
{
    acceptor_.get_io_service().post(
        [&, msg, addr, server] { send_message(msg, addr); } );
}

void messenger_t::send_message(std::string const& msg, ip::address_v4 const& addr, boost::optional<ip::address_v4> server)
{
    send_connection_ptr conn(new send_connection_t(acceptor_.get_io_service(), sm_, msg, addr));
    conn->start(server);
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

    receive_connection_ptr conn(new receive_connection_t(sv_, recv));
    conn->start();
    start_accept();
}
