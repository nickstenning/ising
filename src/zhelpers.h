#ifndef ZHELPERS_H
#define ZHELPERS_H

#include "zmq.h"
#include <string>

namespace zmq {
  class socket_t;
}

/**
 * Receive 0MQ string from socket and convert into string
**/
inline bool s_recv (zmq::socket_t& socket, std::string& string, int flags = 0) {
  zmq::message_t message;
  bool rc = socket.recv(&message, flags);

  string.assign(static_cast<char*>(message.data()), message.size());

  return rc;
}

/**
 * Convert string to 0MQ string and send to socket
**/
inline bool s_send (zmq::socket_t& socket, std::string const& string, int flags = 0) {
  zmq::message_t message(string.size());
  memcpy(message.data(), string.data(), string.size());

  bool rc = socket.send(message, flags);
  return rc;
}

inline bool s_send (zmq::socket_t& socket, char const string[], int flags = 0) {
  std::string s(string);
  return s_send(socket, s, flags);
}

/**
 * Receives all message parts from socket, printing neatly
**/
void s_dump (zmq::socket_t& socket);

/**
 * Set simple random printable identity on socket
**/
void s_set_id (zmq::socket_t& socket);

#endif // ZHELPERS_H
