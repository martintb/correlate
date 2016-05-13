#include <mpi.h>
#include <string>
#include <memory>
#include <chrono> // now()
#include <ctime>  //ctime()

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

  time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
  string date = ctime(&now);
  int gcc_major   = __GNUC__;
  int gcc_minor   = __GNUC_MINOR__;
  int gcc_patch   = __GNUC_PATCHLEVEL__;
  int boost_major = BOOST_VERSION/100000;
  int boost_minor = BOOST_VERSION/100 % 1000;
  int boost_patch = BOOST_VERSION % 1000;
  int ompi_major = OMPI_MAJOR_VERSION;
  int ompi_minor = OMPI_MINOR_VERSION;
  int ompi_patch = OMPI_RELEASE_VERSION;

  Config::ptr conf = make_shared<Config>(); // each proc carries a "configuration" object
  if (conf->isRoot()) {
    cout << "#################################################" << endl;
    cout << ">>>>>>>>>>>>>>>>>>> CORRELATE <<<<<<<<<<<<<<<<<<<" << endl;
    cout << "#################################################" << endl;
    cout << "==> Version:         "<<version<<endl;
    cout << "==> Build Date:      "<<build_date<<endl;
    cout << "==> GCC Version:     "<<gcc_major<<"."<<gcc_minor<< "."<< gcc_patch << endl;
    cout << "==> Boost Version:   "<<boost_major<<"."<< boost_minor<<"."<< boost_patch<< endl;
    cout << "==> OpenMPI Version: "<<ompi_major<<"."<<ompi_minor<<"."<<ompi_patch<< endl;
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

