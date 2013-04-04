#ifndef GLOBAL_PARAMS_H
#define GLOBAL_PARAMS_H
#include <string>
#include <boost/asio.hpp>

extern std::string const g_hostname;
extern boost::asio::ip::address_v4 const g_hostaddr;
extern unsigned short const g_udp_listen_port;
extern unsigned short const g_udp_bcast_port;
extern unsigned short const g_tcp_listen_port;
extern unsigned short const g_tcp_send_port;

#endif // GLOBAL_PARAMS_H
