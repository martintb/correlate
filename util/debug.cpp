#include <iostream>
#include <armadillo>
#include "debug.hpp"

//provide explicit instantiations of the template function for 
//every matrix type you use somewhere in your program.
template void print_arma<arma::mat>(arma::mat matrix);
template void print_arma< arma::Mat<int> >(arma::Mat<int> matrix);
template void print_arma< arma::Cube<double> >(arma::Cube<double> cube);
template void print_arma< arma::Mat<char> >(arma::Mat<char> matrix);
