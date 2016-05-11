#include <mpi.h>
#include <string>
#include <memory>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "Config.hpp"
#include "parse_opts.hpp"
#include "version.hpp"

using namespace std;

void stepper(Config::ptr &conf); // forward declaration rather than mini hpp

int main(int argc, char* argv[]) 
{
  MPI::Init();

  Config::ptr conf = make_shared<Config>(); // each proc carries a "configuration" object

  if (conf->isRoot()) {
    cout << "#################################################" << endl;
    cout << ">>>>>>>>>>>>>>>>>>> CORRELATE <<<<<<<<<<<<<<<<<<<" << endl;
    cout << "#################################################" << endl;
    cout << "==> Version: " << version << endl;
    cout << "==> Build Date: " << build_date << endl;
    cout << "==> GCC Version: ";
    cout << __GNUC__ << ".";
    cout << __GNUC_MINOR__ << ".";
    cout << __GNUC_PATCHLEVEL__ << endl;
    cout << endl;
  }

  conf->printHeader("PARSE INPUT");
  bool success=false; //assume failure
  if (conf->isRoot()) {
    bool success1=false;
    bool success2=false;
    success1 = parse_opts(argc,argv,conf);
    if (success1)
      success2 = conf->setKernelFromStr();
    success = (success1 and success2);
  }

  // All MPI procs must fail together so we transmit
  // the parse_opts result and react together
  MPI::COMM_WORLD.Bcast(&success,1,MPI::BOOL,0);
  if (not success) {
    MPI::Finalize(); // must be called by all procs before exiting
    return EXIT_FAILURE;
  } 

  stepper(conf);

  MPI::Finalize(); // must be called by all procs before exiting
  return EXIT_SUCCESS;
}

