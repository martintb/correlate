#include <iostream>
#include <fstream>
#include <string>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "Reader.hpp"
#include "Config.hpp"
#include "File.hpp"
#include "inFile.hpp"
#include "outFile.hpp"

using namespace std;


bool parse_opts(int argc, char* argv[], Config::ptr &conf) 
{
  string config_file;
  po::options_description baseOpt("Base Options");
  baseOpt.add_options()
    ("help,h", "produce help message")
    ("config_file,f", po::value<string>(&config_file),"optional configuration file")
    ("kernel",        po::value<string>(), "calculation kernel")
    ("intra",  po::bool_switch()->default_value(false), "restrict calculation to intra-molecular pairs")
    ("inter",  po::bool_switch()->default_value(false), "restrict calculation to inter-molecular pairs")
    ("type1",         po::value<string>(), "bead type 1 (e.g A or A,B)")
    ("type2",         po::value<string>(), "bead type 2 (e.g A or C,D)")
  ;

  po::options_description fileInOpt("File Reading");
  fileInOpt.add_options()
    ("input_path",        po::value<string>()->default_value("./"), "path to input data files")
    ("topo",        po::value<string>(),                      "topology file name")
    ("trj",         po::value<string>(),                      "trajectory file name")
  ;

  po::options_description fileOutOpt("File Writing");
  fileOutOpt.add_options()
    ("output_path",        po::value<string>()->default_value("./"), "path to ouput files")
    ("output_file",    po::value<string>()->default_value("calc.dat"), "file to write results to")
    ("output_freq",    po::value<int>()->default_value(-1), "frequency of writing data to file")
    ("overwrite",    po::bool_switch()->default_value(false), "overwrite existing output files")
  ;

  po::options_description calcOpt("Calculation");
  calcOpt.add_options()
    ("frame_start", po::value<int>()->default_value(0),   "first frame to use in calculation")
    ("frame_end",   po::value<int>()->default_value(-1), "last frame to use in calculation")
    ("frame_step",  po::value<int>()->default_value(1),   "interval between used frames")
    ("dx",          po::value<float>()->default_value(0.1), "grid size for output (real or kspace)")
    ("xmax",        po::value<float>()->default_value(100), "largest value in output grid (real or kspace)")
  ;

  po::options_description opt("All Options");
  opt.add(baseOpt).add(fileInOpt).add(fileOutOpt).add(calcOpt);

  
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
    cout << endl;
    cout << opt << endl;
    cout << endl;
    Reader::printReaderList();
    cout << endl;
    conf->printKernelList();
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
  conf->output_freq = vm["output_freq"].as<int>();
  conf->frame_start = vm["frame_start"].as<int>();
  conf->frame_end   = vm["frame_end"].as<int>();
  conf->frame_step  = vm["frame_step"].as<int>();
  conf->dx          = vm["dx"].as<float>();
  conf->xmax        = vm["xmax"].as<float>();
  conf->overwrite   = vm["overwrite"].as<bool>();
  conf->inter       = vm["inter"].as<bool>();
  conf->intra       = vm["intra"].as<bool>();
  if (conf->inter and conf->intra) {
    cerr << "Error! Cannot have intra and inter specified simultaneously." << endl;
    return false;
  }

  conf->output_file =  make_shared<outFile>(
                                            vm["output_path"].as<string>(),
                                            vm["output_file"].as<string>(),
                                            vm["overwrite"].as<bool>()
                                           );

  if (vm.count("topo")) {
    conf->topo_file =  make_shared<inFile>(
                                            vm["input_path"].as<string>(),
                                            vm["topo"].as<string>()
                                          );
  } else {
    cerr << "==> Error! Must specify topology file name via cmd line or input file." << endl;
    return false;
  }

  if (vm.count("trj")) {
    conf->trj_file =  make_shared<inFile>(
                                          vm["input_path"].as<string>(),
                                          vm["trj"].as<string>()
                                         );
  } else {
    cerr << "==> Error! Must specify trajectory file name via cmd line or input file." << endl;
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

  cout << "--> Success! Command line and configure file options successfully parsed." << endl;
  return true; // If we've made it this far, assume success
}
