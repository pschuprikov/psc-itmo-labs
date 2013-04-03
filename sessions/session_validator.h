#ifndef SESSION_VALIDATOR_H
#define SESSION_VALIDATOR_H

#include "session.h"
#include "discover_listener.h"

struct session_validator_t
{
public:
    explicit session_validator_t(discover_listener_t const& dl);

    bool validate(session_t const& session);

private:
    discover_listener_t const& dl_;
};

#endif // SESSION_VALIDATOR_H
