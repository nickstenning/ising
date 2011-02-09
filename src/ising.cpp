#include <iostream>
#include <sstream>
#include <string>

#include "zmq.h"

#include "ising_2d.h"
#include "zhelpers.h"
#include "util.h"

int process_comm(zmq::socket_t&, Ising2D&);
int process_data(zmq::socket_t&, Ising2D&);

int main (unsigned int /*argc*/, char* const /*argv*/[])
{
  zmq::context_t ctx(1);

  zmq::socket_t comm(ctx, ZMQ_REP);
  zmq::socket_t data(ctx, ZMQ_REP);

  comm.connect("tcp://127.0.0.1:5000");
  data.connect("tcp://127.0.0.1:5001");

  // Initialize poll set
  zmq::pollitem_t poll_items [] = {
    { comm, 0, ZMQ_POLLIN, 0 },
    { data, 0, ZMQ_POLLIN, 0 }
  };

  Ising2D ising(200, 200);

  while (1) {
    zmq::poll(&poll_items[0], 2, -1);

    // Got command
    if (poll_items[0].revents & ZMQ_POLLIN) {
      process_comm(comm, ising);
    }

    // Got request for data
    if (poll_items[1].revents & ZMQ_POLLIN) {
      process_data(data, ising);
    }
  }

  return 0;
}

int process_comm(zmq::socket_t& sock, Ising2D& ising)
{
  std::istringstream s_in( s_recv(sock) );
  std::ostringstream s_out;
  std::string cmd;

  s_in >> cmd;

  if (cmd.compare("dims") == 0) {
    s_out << ising.rows() << " " << ising.cols();
  }

  if (cmd.compare("rst") == 0) {
    ising.reset();
    s_out << "OK";
  }

  s_send(sock, s_out.str());
  return 0;
}

int process_data(zmq::socket_t& sock, Ising2D& ising)
{
  std::istringstream s_in( s_recv(sock) );

  int numSteps;
  s_in >> numSteps;

  ising.step(numSteps);

  std::ostringstream s_out( std::ios::binary );
  ising.serialize(s_out);

  s_send(sock, s_out.str());

  return 0;
}

