#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "packets.hpp"
#include "session_manager.h"

#include "global_params.h"

using namespace boost::asio;

namespace
{

struct session_request_connection_t
    : boost::enable_shared_from_this<session_request_connection_t>
{
    session_request_connection_t(discover_listener_t const& dl, boost::asio::ip::address_v4 const& addr, session_req_func func )
        : dl_(dl), sock_(dl.service()) , func_(func), addr_(addr)
    {}

    void start()
    {
        if (dl_.servers().empty())
        {
            std::cerr << "no servers\n";
            return;
        }
        sock_.async_connect(*dl_.servers().begin(),
            bind(&session_request_connection_t::handle_connect,
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
        packet_.dest_addr = addr_.to_string();

        write_message(sock_, packet_,
            bind(&session_request_connection_t::handle_write, shared_from_this()));
    }

    void handle_write( )
    {
        auto self = shared_from_this();
        dispatch_message_read(sock_,
            std::function<void(session_ack)>([&,self] (session_ack ack) mutable
            {
                session_req_func func = func_;
                self.reset();
                std::cerr << "session_id : " << ack.session_id << " for " << ack.dest_addr << "\n";
                if (func)
                    func(session_t(ack.session_id));
            }),
            std::function<void(session_rej)>([&, self](session_rej rej)
            {
                std::cerr << "session request was rejected\n";
            })
        );
    }

private:
    session_req packet_;
    discover_listener_t const& dl_;
    ip::tcp::socket sock_;
    session_req_func func_;
    boost::asio::ip::address_v4 addr_;
};
typedef boost::shared_ptr<session_request_connection_t> session_request_connection_ptr;

}

session_manager_t::session_manager_t(discover_listener_t const& dl)
    : dl_(dl)
{}

void session_manager_t::obtain_session_out(boost::asio::ip::address_v4 const& remote)
{
    dl_.service().post([&, remote] { obtain_session(remote, session_req_func()); });
}

void session_manager_t::obtain_session(const ip::address_v4 &remote, session_req_func func)
{
    session_request_connection_ptr conn(new session_request_connection_t(dl_, remote, func));
    conn->start();
}
