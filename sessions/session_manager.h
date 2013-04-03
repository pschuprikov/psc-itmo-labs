#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <map>
#include <boost/optional.hpp>

#include "session.h"
#include "discover_listener.h"

struct session_manager_t
{
    session_manager_t(discover_listener_t const& dl);

    void obtain_session(boost::asio::ip::address_v4 const& remote);
    boost::optional<session_t> operator[](boost::asio::ip::address_v4 const& addr);
private:
    void handle_connect(boost::system::error_code const& err, const boost::asio::ip::address_v4 &remote);

private:
    std::map<boost::asio::ip::address_v4, session_t> sessions_;

    discover_listener_t const& dl_;
    boost::asio::ip::tcp::socket sock_;
};

#endif // SESSION_MANAGER_H
