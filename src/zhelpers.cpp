#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "zmq.h"

#include "zhelpers.h"

#define within(num) (int) ((float) (num) * random () / (RAND_MAX + 1.0))

void s_set_id (zmq::socket_t& socket) {
  char identity[10];
  sprintf(identity, "%04X-%04X", within (0x10000), within (0x10000));
  socket.setsockopt(ZMQ_IDENTITY, identity, strlen(identity));
}

void s_dump (zmq::socket_t& socket) {
  std::cout << "----------------------------------------" << std::endl;

  while (1) {
    // Process all parts of the message

    zmq::message_t message;
    socket.recv(&message);

    // Dump the message as text or binary
    std::string data(static_cast<char*>(message.data()));
    int size = message.size();

    bool is_text = true;

    int char_nbr;
    unsigned char byte;
    for (char_nbr = 0; char_nbr < size; char_nbr++) {
      byte = data [char_nbr];
      if (byte < 32 || byte > 127) is_text = false;
    }

    printf("[%03d] ", size);

    for (char_nbr = 0; char_nbr < size; char_nbr++) {
      if (is_text) {
        printf("%c", data[char_nbr]);
      } else {
        printf("%02X", (unsigned char) data[char_nbr]);
      }
    }
    printf("\n");

    int64_t more; // Multipart detection
    size_t more_size = sizeof(more);
    socket.getsockopt(ZMQ_RCVMORE, &more, &more_size);

    if (!more) break; // Last message part
  }
}