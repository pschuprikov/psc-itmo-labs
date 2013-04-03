#ifndef DISCOVER_SENDER_H
#define DISCOVER_SENDER_H

#include <boost/asio.hpp>

struct discover_sender_t
{
    discover_sender_t(boost::asio::io_service& ios, boost::asio::ip::udp::endpoint dest);

private:
    void start_send();
    void handle_send(const boost::system::error_code &err);

private:
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint dest_;
    boost::asio::deadline_timer timer_;
};

#endif // DISCOVER_SENDER_H
