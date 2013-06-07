#include "global_params.h"

std::string const g_hostname = "rogmachine";
boost::asio::ip::address_v4 const g_hostaddr = boost::asio::ip::address_v4::from_string("192.168.80.8");
boost::asio::ip::address_v4 const g_bcast_addr = boost::asio::ip::address_v4::from_string("192.168.80.8");
unsigned short const g_udp_listen_port = 9999;
unsigned short const g_udp_bcast_port = 9998;
unsigned short const g_tcp_listen_port = 9999;
unsigned short const g_tcp_send_port = 9999;
