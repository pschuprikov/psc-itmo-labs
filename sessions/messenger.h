#ifndef MESSENGER_H
#define MESSENGER_H

#include "session_manager.h"
#include "session_validator.h"
struct messenger_t
{
    messenger_t(session_manager_t& sm, discover_listener_t const& dl);

    void send_message(std::string const& msg, const boost::asio::ip::address_v4 &addr);

private:
    struct accept_data_t
    {
        accept_data_t(boost::asio::io_service& ios)
            : sock(ios)
        {}

        boost::asio::ip::tcp::socket sock;
        boost::asio::ip::tcp::endpoint remote;
    };
    typedef boost::shared_ptr<accept_data_t> accept_data_ptr;

    struct send_data_t
    {
        send_data_t(boost::asio::io_service &ios, std::string const& message,
                    boost::asio::ip::address_v4 const& addr)
            : sock(ios)
            , message(message)
            , addr(addr)
        {}

        boost::asio::ip::tcp::socket sock;
        boost::asio::ip::address_v4 addr;
        std::string message;
    };
    typedef boost::shared_ptr<send_data_t> send_data_ptr;

private:
    void start_accept();
    void handle_accept(boost::system::error_code const& err, accept_data_ptr data);
    void handle_connect(boost::system::error_code const& err, send_data_ptr data);

private:
    session_manager_t& sm_;
    session_validator_t sv_;

    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif // MESSENGER_H
