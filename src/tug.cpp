#include <iostream>
#include <sstream>
#include <string>

#include "zmq.h"

#include "wavefield.h"
#include "zhelpers.h"
#include "util.h"

int process_comm(zmq::socket_t&, Wavefield&);
int process_data(zmq::socket_t&, Wavefield&);

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

  Wavefield wf(200, 200);

  while (1) {
    zmq::poll(&poll_items[0], 2, -1);

    // Got command
    if (poll_items[0].revents & ZMQ_POLLIN) {
      process_comm(comm, wf);
    }

    // Got request for data
    if (poll_items[1].revents & ZMQ_POLLIN) {
      process_data(data, wf);
    }
  }

  return 0;
}

int process_comm(zmq::socket_t& sock, Wavefield& wf)
{
  std::istringstream s_in( s_recv(sock) );
  std::ostringstream s_out;
  std::string cmd;

  s_in >> cmd;

  if (cmd.compare("dims") == 0) {
    s_out << wf.rows() << " " << wf.cols();
  }

  s_send(sock, s_out.str());
  return 0;
}

int process_data(zmq::socket_t& sock, Wavefield& wf)
{
  std::istringstream s_in( s_recv(sock) );

  double dt;
  s_in >> dt;

  wf.step(dt);

  std::ostringstream s_out( std::ios::binary );
  wf.serialize(s_out);

  s_send(sock, s_out.str());

  return 0;
}

