#include <iostream>
#include <fstream>
#include <string>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "Config.hpp"

using namespace std;


bool parse_opts(int argc, char* argv[], Config *conf) 
{
  cout << "Attempting to parse command line and configure file options..." << endl;

  string config_file;
  po::options_description opt("Calculation Configuration Options");
  opt.add_options()
    ("help,h", "produce help message")
    ("config_file,f", po::value<string>(&config_file),"calculation configuration file")
    ("path",        po::value<string>()->default_value("./"), "path to data files")
    ("xml",         po::value<string>(), "xml file name")
    ("dcd",         po::value<string>(), "dcd file name")
    ("type1",       po::value<string>(), "bead type 1 for calculating correlations")
    ("type2",       po::value<string>(), "bead type 2 for calculating correlations")
    ("frame_start", po::value<int>()->default_value(0), "first frame to use in calculation")
    ("frame_end",    po::value<int>()->default_value(-1), "last frame to use in calculation")
    ("frame_step",  po::value<int>()->default_value(1), "interval between used frames")
    ("nthreads",    po::value<int>()->default_value(1), "number of openMP/openACC threads to use per mpi process")
    ("kernel",    po::value<string>(), "Kernel used for calculation")
  ;

  
  cout << "--> Attempting to parse command line variables..." << endl;
  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, opt), vm);
    po::notify(vm);    
  } 
  catch(exception& e) {
    cerr << "==> Error! " << e.what() << endl;
    return false; // false = noSuccess
  }
  cout << "--> Successfully parsed command line variables!" << endl;

  
  // If help was requested, print command options and exit
  if (vm.count("help")) {
    cout << opt << endl;
    exit(0);
  }

  // Attempt to parse configure file
  if (not vm.count("config_file")) {
    cout << "--> No configuration file specified!" << endl;
    cout << "--> All variables must be specified via the command line." << endl;
  } else {
    cout << "--> Attempting to read configuration file ";
    cout << vm["config_file"].as<string>() << "..." << endl;
    ifstream ifs(config_file.c_str());
    if (!ifs) {
        cout << "==> Error! Can't open input file: " << config_file << endl;
        return false;
    } else {
      try {
        po::store(parse_config_file(ifs, opt), vm);
        po::notify(vm);
      } catch(exception& e) {
        cerr << "==> Error! " << e.what() << endl;
        return false;
      }
    }
    cout << "--> Successfully read configuration file!" << endl;
  }


  /*transfer parameters into conf structure*/

  // No need to error check opts with default values
  conf->path = vm["path"].as<string>();
  conf->frame_start = vm["frame_start"].as<int>();
  conf->frame_end = vm["frame_end"].as<int>();
  conf->frame_step = vm["frame_step"].as<int>();
  conf->nthreads = vm["nthreads"].as<int>();

  if (vm.count("xml")) {
    conf->xml = vm["xml"].as<string>();
  } else {
    cerr << "==> Error! Must specify xml file name via cmd line or input file." << endl;
    return false;
  }

  if (vm.count("dcd")) {
    conf->dcd = vm["dcd"].as<string>();
  } else {
    cerr << "==> Error! Must specify dcd file name via cmd line or input file." << endl;
    return false;
  }


  if (vm.count("type1")) {
    conf->type1 = vm["type1"].as<string>();
  } else {
    cerr << "==> Error! Must specify type 1 for calculation via cmd line or input file." << endl;
    return false;
  }

  if (vm.count("type2")) {
    conf->type2 = vm["type2"].as<string>();
  } else {
    cerr << "==> Error! Must specify type 2 for calculation via cmd line or input file." << endl;
    return false;
  }

  if (vm.count("kernel")) {
    conf->kernelStr = vm["kernel"].as<string>();
  } else {
    cerr << "==> Error! Must specify kernel string to use for calculation." << endl;
    return false;
  }

  cout << "Success! Command line and configure file options successfully parsed." << endl;
  return true; // If we've made it this far, assume success
}
