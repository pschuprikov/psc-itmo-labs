#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <map>
#include <boost/optional.hpp>

#include "session.h"
#include "discover_listener.h"

typedef std::function<void(session_t)> session_req_func;

struct session_manager_t
{
    session_manager_t(discover_listener_t const& dl);

    void obtain_session_out(boost::asio::ip::address_v4 const& remote);

    void obtain_session(boost::asio::ip::address_v4 const& remote, session_req_func func);
private:

private:
    std::map<boost::asio::ip::address_v4, session_t> sessions_;

    discover_listener_t const& dl_;
};

#endif // SESSION_MANAGER_H