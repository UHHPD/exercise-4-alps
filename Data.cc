#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <cmath>

using namespace std;

Data::Data(const std::string& filename) : m_data(), m_bins(), m_errors() {
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }

  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double entries; 
    file >> entries;
    m_data.push_back(entries);
  }

  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double error;
    file >> error;
    m_errors.push_back(error);
  }


   //done! close the file
   file.close();

   assertSizes();
};

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }


int Data::checkCompatibility(const Data &in, int n){
  int outliers = 0;
  for(int i = 0; i < m_data.size(); ++i){
    double comb_err = sqrt(m_errors[i]*m_errors[i] + in.m_errors[i]*in.m_errors[i]);
    if(std::abs(m_data[i] - in.m_data[i])/comb_err > n){
      std::cout << "Data incompatible at bin " << i << std::endl;
      outliers++;
    }
  }
  return outliers;
}


Data operator+(const Data& d1, const Data& d2){
  Data sum(d1);
  for(int i = 0; i < d1.m_data.size(); ++i){
    double w1 = 1 / d1.m_errors[i] / d1.m_errors[i];
    double w2 = 1 / d2.m_errors[i] / d2.m_errors[i];
    sum.m_data[i] = (w1*d1.m_data[i] + w2*d2.m_data[i]) / (w1 + w2);
    sum.m_errors[i] = 1 / sqrt(w1 + w2);
  }
  return sum;
}

double Data::chi2(){
  double alpha = 0.005, beta = -0.00001, gamma = 0.08, delta = 0.015;
  double chi2 = 0;
  for(int i = 0; i < m_data.size(); ++i){
    double f = alpha + beta * binCenter(i) + gamma * exp(-delta * binCenter(i));
    chi2 += (m_data[i]-f) * (m_data[i]-f) / m_errors[i] / m_errors[i];
  }
  return chi2;
}
