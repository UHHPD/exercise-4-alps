#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <list>
#include <cmath>

#include "Data.hh"

// generic function comparing two values of some type T used later for int and
// double
template <class T>
bool testEqual(const std::string& name, T expected, T real) {
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}

bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}

int main() {
  using namespace std;

  cout << "******************************************************" << endl;
  runTests();
  cout << "******************************************************" << endl;
  // create an object which holds data of experiment A
  std::vector<Data> exps;
  for (auto s : {"exp_A","exp_B","exp_C","exp_D"}){
    exps.push_back(Data(s));
  }

std::vector<char>  exp_label = {'A','B','C','D'};

double error_sum = 0;
for(int i = 0; i < exp_label.size(); ++i){
  // here is the data from experiment A  ;
  cout << "bin 27: from " << exps[i].binLow(27) << " to " << exps[i].binHigh(27)
       << endl;
  cout << "measurement of experiment " << exp_label[i] << " in bin 27: " << exps[i].measurement(27)
       << " +/- " << exps[i].error(27)  << endl;
  error_sum += exps[i].error(27)*exps[i].error(27);
}

cout << "error = " << 4*pow(error_sum,0.5) << endl;
  return 0;

for(int n = 2; n < 4; ++n){
  int j = 0;
  for(int i = 0; i < exp_label.size(); ++i){
    std::cout << "exp " << exp_label[i] << "vs exp " << exp_label[i+1] << " differ in " << exps[i].checkCompatibility(exps[i+1], n) << "measurements by " << n << " sigma" << std::endl;
  }
}

Data sum12  = exps[0] + exps[1];
for(int i = 0; i < sum12.size(); ++i){
  std::cout << exps[0].measurement(i) << " + " << exps[1].measurement(i) << " = " << sum12.measurement(i) << std::endl;
  std::cout << exps[0].error(i) << ", " << exps[1].error(i) << ", " << sum12.error(i) << std::endl;
} 

for(auto &e : exps){
  std::cout << "chi2: " << e.chi2() << " chi2/ndf: " << e.chi2()/52 << std::endl;
}

Data sum = exps[0]+exps[1]+exps[2]+exps[3];
std::cout << "chi2: " << sum.chi2() << " chi2/ndf: " << sum.chi2()/52 << std::endl;

return 0;
}

