#ifndef ZHELPERS_H
#define ZHELPERS_H

#include <string>
#include <sstream>

namespace zmq {
  class socket_t;
}

/**
 * Receive 0MQ string from socket and convert into string
**/
std::string s_recv (zmq::socket_t& socket);

/**
 * Convert string to 0MQ string and send to socket
**/
bool s_send (zmq::socket_t& socket, std::string const& string);
bool s_send (zmq::socket_t& socket, char const string[]);

/**
 * Receives all message parts from socket, printing neatly
**/
void s_dump (zmq::socket_t& socket);

/**
 * Set simple random printable identity on socket
**/
void s_set_id (zmq::socket_t& socket);

#endif // ZHELPERS_H
