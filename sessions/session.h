#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <string>

struct session_t
{
    session_t(std::string const& id)
        : id_(id)
    {}

    std::string const& id() const { return id_; }

private:
    std::string id_;
};

#endif // SESSION_H
