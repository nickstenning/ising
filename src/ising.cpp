#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

#include "zmq.h"

#include "ising_2d.h"
#include "zhelpers.h"
#include "util.h"

std::string process_comm(std::string&, Ising2D&);
std::string process_data(std::string&, Ising2D&);

int main (unsigned int argc, char* const argv[])
{
  zmq::context_t ctx(1);

  zmq::socket_t comm(ctx, ZMQ_REP);
  zmq::socket_t data(ctx, ZMQ_REP);

  comm.connect("tcp://127.0.0.1:5000");
  data.connect("tcp://127.0.0.1:5001");

  // Set dimensions from command line if given
  size_t rows, cols;

  if (argc == 3) {
    rows = strtoul(argv[1], NULL, 0);
    cols = strtoul(argv[2], NULL, 0);
  } else {
    rows = 200;
    cols = 200;
  }

  Ising2D ising(rows, cols);

  while (1) {
    std::string req, rep;

    // Process any commands
    if (s_recv(comm, req, ZMQ_NOBLOCK)) {
      rep = process_comm(req, ising);
      s_send(comm, rep);
    }

    // Process any requests for data
    if (s_recv(data, req, ZMQ_NOBLOCK)) {
      rep = process_data(req, ising);
      s_send(data, rep);
    }

    ising.step();
  }

  return 0;
}

std::string process_comm(std::string& msg, Ising2D& ising)
{
  std::istringstream s_in( msg );
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

  return s_out.str();
}

std::string process_data(std::string& msg, Ising2D& ising)
{
  std::istringstream s_in( msg );
  std::ostringstream s_out( std::ios::binary );

  ising.serialize(s_out);

  return s_out.str();
}

