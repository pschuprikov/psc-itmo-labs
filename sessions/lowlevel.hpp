#ifndef LOWLEVEL_HPP
#define LOWLEVEL_HPP

#include <iostream>
#include <algorithm>
#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

namespace
{
}

template<class Uint, class Iter>
Iter read_uint(Uint& uint, Iter beg)
{
    auto out = reinterpret_cast<unsigned char *>(&uint);
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
    template<class Socket, class Func>
    static void write(Socket& sock, Struct const& s, Func func)
    {
        auto out_value = std::make_shared<Uint>(s.*uint);
        auto out = reinterpret_cast<unsigned char *>(out_value.get());
        std::reverse(out, out + sizeof(Uint));
        boost::asio::async_write(sock, boost::asio::buffer(out_value.get(), sizeof(Uint)),
        [out_value, func] (boost::system::error_code const& err, size_t)
        { if (err) { std::cerr << err.message() << "\n"; return; }; func(); });
    }

    template<class Socket, class Func>
    static void read(Socket& sock, Struct& s, Func func)
    {
        boost::asio::async_read(sock, boost::asio::buffer(&(s.*uint), sizeof(Uint)),
        [&s, func] (boost::system::error_code const& err, size_t)
        {
            if (err) { std::cerr << err.message() << "\n"; return; }
            auto out = reinterpret_cast<unsigned char *>(&(s.*uint));
            std::reverse(out, out + sizeof(Uint));
            func();
        });
    }
};

template<class Uint>
struct uint_io_single
{
    uint_io_single(Uint value = Uint()) : value(value) {}
    Uint value;

    typedef uint_io<Uint, uint_io_single<Uint>, &uint_io_single<Uint>::value> uint_io_t;

    template<class Socket, class Func>
    void write(Socket& sock, Func func) const
    { uint_io_t::write(sock, *this, func); }

    template<class Socket, class Func>
    void read(Socket& sock, Func func)
    { uint_io_t::read(sock, *this, func); }
};


template<class Uint, class Struct, std::string Struct::*str>
struct string_io
{
    template<class Socket, class Func>
    static void write(Socket& sock, Struct const& s, Func func)
    {
        auto length = std::make_shared<uint_io_single<Uint>>((s.*str).length());
        length->write(sock, [length, &s, &sock, func]
        {
            boost::asio::async_write(sock, boost::asio::buffer(s.*str),
            [func] (boost::system::error_code const& err, size_t)
            { if (err) { std::cerr << err.message() << "\n"; return; } func(); });
        });
    }

    template<class Socket, class Func>
    static void read(Socket& sock, Struct& s, Func func)
    {
        std::shared_ptr< uint_io_single<Uint> > length(new uint_io_single<Uint>());
        length->read(sock, [&s, &sock, func, length] ()
        {
            auto buf = std::make_shared<std::vector<unsigned char>>(length->value);
            boost::asio::async_read(sock, boost::asio::buffer(*buf), [=,&s]
            (boost::system::error_code const& err, size_t)
            {
                if (err) { std::cerr << err.message() << "\n"; return; }
                (s.*str).assign(buf->begin(), buf->end()); func();
            });
        });
    }
};

template<int idx>
struct write_message_t
{
    template<class Socket, class Packet, class Func>
    write_message_t(Socket& sock, Packet const& pack, Func func)
    {
        std::tuple_element<std::tuple_size<typename Packet::desc>::value - idx - 1, typename Packet::desc>::type::write(sock, pack,
            [&sock, &pack, func] () { write_message_t<idx - 1>(sock, pack, func); });
    }
};

template<>
struct write_message_t<-1>
{
    template<class Socket, class Packet, class Func>
    write_message_t(Socket&, Packet const&, Func func) { func(); }
};

template<int idx>
struct read_message_t
{
    template<class Socket, class Packet, class Func>
    read_message_t(Socket& sock, Packet& pack, Func func)
    {
        std::tuple_element<std::tuple_size<typename Packet::desc>::value - idx - 1, typename Packet::desc>::type::read(sock, pack,
            [&sock, &pack, func] () { read_message_t<idx - 1>(sock, pack, func); });
    }
};

template<>
struct read_message_t<-1>
{
    template<class Socket, class Packet, class Func>
    read_message_t(Socket&, Packet&, Func func)
    { func(); }
};

template<class Socket, class Packet, class Func>
inline void write_message(Socket& sock, Packet const& pack, Func func)
{
    auto type = std::make_shared<uint_io_single<unsigned char> >(Packet::type);
    type->write(sock, [func, type, &sock, &pack] () {
        write_message_t<std::tuple_size<typename Packet::desc>::value - 1>(sock, pack, func);
    });
}

template<class Socket, class Packet, class Func>
inline void read_message_impl(Socket& sock, Packet& pack, Func func)
{
    read_message_t<std::tuple_size<typename Packet::desc>::value - 1>(sock, pack, func);
}

template<class Socket, class Packet, class Func>
inline void read_message(Socket& sock, Packet& pack, Func func)
{
    auto type = std::make_shared<uint_io_single<unsigned char> >(Packet::type);
    type->read(sock, [func, type, &sock, &pack] () {
        read_message_impl(sock, pack, [func] () { func(); });
    });
}

template<class Socket>
inline void finish_dispatch(Socket&, unsigned char)
{
    return;
}

template<class Socket, class Processor, class... Args>
inline void finish_dispatch(Socket& sock, unsigned char type, Processor proc, Args... args)
{
    typedef typename Processor::argument_type Packet;
    if (Packet::type == type)
    {
        auto pack = std::make_shared<Packet>();
        read_message_impl(sock, *pack, [pack, proc] () { proc(*pack); });
    }
    else
    finish_dispatch(sock, type, args...);
}

template<class Socket, class... Args>
inline void dispatch_message_read(Socket& sock, Args... args)
{
    auto type = std::make_shared<uint_io_single<unsigned char> >();
    type->read(sock, [=, &sock] ()
    { finish_dispatch(sock, type->value, args...); });
}

#endif // LOWLEVEL_HPP
