#ifndef DISCOVER_LISTENER_H
#define DISCOVER_LISTENER_H

#include <boost/asio.hpp>
#include <array>
#include <set>

typedef std::set<boost::asio::ip::tcp::endpoint> endpoints_t;

struct discover_listener_t
{
    discover_listener_t(boost::asio::io_service& service);

    endpoints_t const& servers() const { return servers_; }
    endpoints_t const& clients() const { return clients_; }

    void print_servers() const;

private:
    typedef std::array<unsigned char, 260> buffer_t;

private:
    void start_receive();
    void handle_receive(const boost::system::error_code &err, size_t bytes_transferred);

private:
    boost::asio::io_service& ios_;
    boost::asio::ip::udp::socket socket_;
    buffer_t buf_;

    boost::asio::ip::udp::endpoint tmp_remote_;

    endpoints_t servers_;
    endpoints_t clients_;
};

#endif // DISCOVER_LISTENER_H
