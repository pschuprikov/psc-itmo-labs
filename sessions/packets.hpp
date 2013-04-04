#ifndef PACKETS_H
#define PACKETS_H

#include "lowlevel.hpp"

struct session_rej
{
    std::string dest_addr;
    enum { type = 0x01 };
    typedef std::tuple< string_io<unsigned char, session_rej, &session_rej::dest_addr> > desc;
};

struct session_req
{
    std::string cname;
    std::string dest_addr;
    enum { type = 0x00 };

    typedef std::tuple< string_io<unsigned char, session_req, &session_req::cname>,
                        string_io<unsigned char, session_req, &session_req::dest_addr> > desc;
};

struct session_ack
{
    enum { type = 0x00 };

    std::string session_id;
    std::string dest_addr;

    typedef std::tuple< string_io<unsigned char, session_ack, &session_ack::session_id>,
                        string_io<unsigned char, session_ack, &session_ack::dest_addr> > desc;
};

struct session_check
{
    enum { type = 0x01 };

    std::string session_id;
    std::string cname;

    typedef std::tuple< string_io<unsigned char, session_check, &session_check::session_id>,
                        string_io<unsigned char, session_check, &session_check::cname> > desc;
};

struct session_valid
{
    enum { type = 0x02 };

    std::string session_id;

    typedef std::tuple< string_io<unsigned char, session_valid, &session_valid::session_id> > desc;
};

struct session_fail
{
    enum { type = 0x03 };

    std::string session_id;

    typedef std::tuple< string_io<unsigned char, session_fail, &session_fail::session_id> > desc;
};

struct message_pack
{
    enum { type = 0xFF };

    std::string session_id;
    std::string message;
    typedef std::tuple< string_io<unsigned char , message_pack, &message_pack::session_id>,
                        string_io<unsigned short, message_pack, &message_pack::message> > desc;
};

#endif // PACKETS_H
