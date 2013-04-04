#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>

#include "packets.hpp"
#include "session_validator.h"

#include "global_params.h"

using namespace boost::asio;

namespace
{

struct session_validate_connection_t
    : boost::enable_shared_from_this<session_validate_connection_t>
{
    session_validate_connection_t(discover_listener_t const& dl, session_t const& session, session_valid_func func )
        : dl_(dl), sock_(dl.service()) , func_(func), session_(session)
    {}

    void start()
    {
        if (dl_.servers().empty())
        {
            std::cerr << "no servers\n";
            return;
        }

        sock_.async_connect(*dl_.servers().begin(),
            bind(&session_validate_connection_t::handle_connect,
                 shared_from_this(), placeholders::error));
    }

private:
    void handle_connect(boost::system::error_code const& err)
    {
        if (err)
        {
            std::cerr << err.message() << "\n";
            return;
        }

        packet_.cname = g_hostname;
        packet_.session_id = session_.id();

        std::cerr << "checking session: " << packet_.session_id << "\n";
        write_message(sock_, packet_,
            bind(&session_validate_connection_t::handle_write, shared_from_this()));
    }

    void handle_write( )
    {
        auto self = shared_from_this();
        session_valid_func func = func_;

        dispatch_message_read(sock_,
            std::function<void(session_valid)>([self, func] (session_valid valid) mutable
            {
                self.reset();
                std::cerr << "session_valid : " << valid.session_id << "\n";
                if (func) func(true);
            }),
            std::function<void(session_fail)>([self, func] (session_fail fail) mutable
            {
                self.reset();
                std::cerr << "session_invalid : " << fail.session_id << "\n";
                if (func) func(false);
            })
        );
    }

private:
    session_check packet_;
    discover_listener_t const& dl_;
    ip::tcp::socket sock_;
    session_valid_func func_;
    session_t session_;
};
typedef boost::shared_ptr<session_validate_connection_t> session_validate_connection_ptr;

}

session_validator_t::session_validator_t(discover_listener_t const& dl)
    : dl_(dl)
{}

void session_validator_t::validate(session_t const& session, session_valid_func func)
{
    session_validate_connection_ptr conn(new session_validate_connection_t(dl_, session, func));
    conn->start();
}
