#ifndef SESSION_VALIDATOR_H
#define SESSION_VALIDATOR_H

#include "session.h"
#include "discover_listener.h"

typedef boost::function<void(bool)> session_valid_func;

struct session_validator_t
{
public:
    explicit session_validator_t(discover_listener_t const& dl);

    void validate(session_t const& session, session_valid_func func);

private:
    discover_listener_t const& dl_;
};

#endif // SESSION_VALIDATOR_H
