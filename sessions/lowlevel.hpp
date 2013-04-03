#ifndef LOWLEVEL_HPP
#define LOWLEVEL_HPP

#include <algorithm>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

template<class Uint, class Iter>
Iter read_uint(Uint& uint, Iter beg)
{
    unsigned char * out = reinterpret_cast<unsigned char *>(&uint);
    std::copy(beg, beg + sizeof(Uint), out);
    std::reverse(out, out + sizeof(Uint));
    return beg + sizeof(Uint);
}

template<class Uint, class Iter>
Iter read_string(std::string& str, Iter beg)
{
    Uint length;
    beg = read_uint(length, beg);
    str.assign(beg, beg + length);
    return beg + length;
}

template<class Uint, class Struct, Uint Struct::*uint>
struct uint_io
{
    template<class Socket>
    static void write(Socket& sock, Struct const& s)
    {
        Uint out_value = s.*uint;
        unsigned char * out = reinterpret_cast<unsigned char *>(&out_value);
        std::reverse(out, out + sizeof(Uint));
        boost::asio::write(sock, boost::asio::buffer(out, sizeof(Uint)));
    }

    template<class Socket>
    static void read(Socket& sock, Struct& s)
    {
        unsigned char * out = reinterpret_cast<unsigned char *>(&(s.*uint));
        boost::asio::read(sock, boost::asio::buffer(out, sizeof(Uint)));
        std::reverse(out, out + sizeof(Uint));
    }
};

template<class Uint>
struct uint_io_single
{
    uint_io_single(Uint value = Uint()) : value(value) {}
    Uint value;

    typedef uint_io<Uint, uint_io_single<Uint>, &uint_io_single<Uint>::value> uint_io_t;

    template<class Socket>
    uint_io_single const& write(Socket& sock) const { uint_io_t::write(sock, *this); return *this; }

    template<class Socket>
    uint_io_single& read(Socket& sock) { uint_io_t::read(sock, *this); return *this; }
};


template<class Uint, class Struct, std::string Struct::*str>
struct string_io
{
    template<class Socket>
    static void write(Socket& sock, Struct const& s)
    {
        uint_io_single<unsigned char>((s.*str).length()).write(sock);
        boost::asio::write(sock, boost::asio::buffer(s.*str));
    }

    template<class Socket>
    static void read(Socket& sock, Struct& s)
    {
        std::vector<unsigned char> buf(uint_io_single<unsigned char>().read(sock).value);
        boost::asio::read(sock, boost::asio::buffer(buf));
        (s.*str).assign(buf.begin(), buf.end());
    }
};

template<int idx, class Socket, class Packet>
struct write_message_t
{
    write_message_t(Socket& sock, Packet const& pack)
    {
        std::tuple_element<idx, typename Packet::desc>::type::write(sock, pack);
        write_message_t<idx - 1, Socket, Packet>(sock, pack);
    }
};

template<class Socket, class Packet>
struct write_message_t<-1, Socket, Packet>
{
    write_message_t(Socket&, Packet const&) {}
};

template<int idx, class Socket, class Packet>
struct read_message_t
{
    read_message_t(Socket& sock, Packet& pack)
    {
        std::tuple_element<idx, typename Packet::desc>::type::read(sock, pack);
        read_message_t<idx - 1, Socket, Packet>(sock, pack);
    }
};

template<class Socket, class Packet>
struct read_message_t<-1, Socket, Packet>
{
    read_message_t(Socket&, Packet&) {}
};

template<class Socket, class Packet>
inline void write_message(Socket& sock, Packet const& pack)
{
    uint_io_single<unsigned char>(Packet::type).write(sock);
    write_message_t<std::tuple_size<typename Packet::desc>::value - 1, Socket, Packet>(sock, pack);
}

template<class Socket, class Packet>
inline void read_message(Socket& sock, Packet& pack)
{
    read_message_t<std::tuple_size<typename Packet::desc>::value - 1, Socket, Packet>(sock, pack);
}

template<class Socket, class Processor, class... Args>
inline void finish_dispatch(Socket& sock, unsigned char type, Processor proc, Args... args)
{
    typedef typename Processor::argument_type Packet;
    if (Packet::type == type)
    {
        Packet pack;
        read_message(sock, pack);
        proc(pack);
    }
}

template<class Socket, class... Args>
inline void dispatch_message_read(Socket& sock, Args... args)
{
    uint_io_single<unsigned char> type;
    type.read(sock);
    finish_dispatch(sock, type.value, args...);
}

#endif // LOWLEVEL_HPP
