#include <boost/assert.hpp>
#include "packets.hpp"
#include "session_validator.h"

#include "global_params.h"

using namespace boost::asio;

session_validator_t::session_validator_t(discover_listener_t const& dl)
    : dl_(dl)
{}

bool session_validator_t::validate(session_t const& session)
{
    if (dl_.servers().empty())
    {
        std::cerr << "no servers\n";
        return false;
    }

    ip::tcp::socket sock_(dl_.service());
    sock_.connect(*dl_.servers().begin());

    session_check pack;
    pack.cname = g_hostname;
    pack.session_id = session.id();

    write_message(sock_, pack);

    bool result;
    dispatch_message_read(sock_,
    std::function<void(session_fail)>([&] (session_fail fail)
    {
        result = false;
    }),
    std::function<void(session_valid)>([&] (session_valid fail)
    {
        result = true;
    })
    );

    sock_.close();
    return result;
}
