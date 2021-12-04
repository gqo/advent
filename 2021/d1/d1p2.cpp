#include <iostream>
#include <fstream>
using namespace std;

int main() {
  ifstream input("input.txt");
  if (!input) {
    cerr << "main(): Could not open file" << endl;
    exit(1);
  }

  int current, count;
  int sums [3] = { };
  int sumsOps [3] = { };
  int prevSum = -1;

  // First round:
  input >> current;
  sums[0] += current;
  sumsOps[0]++;

  // Second round:
  input >> current;
  sums[0] += current;
  sums[1] += current;
  sumsOps[0]++;
  sumsOps[1]++;

  while(input >> current) {
    for (size_t i = 0; i < 3; i++) { 
      sums[i] += current;
      sumsOps[i]++;
    }

    for (size_t i = 0; i < 3; i++) {
      if (sumsOps[i] == 3) {
        if ((prevSum < sums[i]) && prevSum != -1) count++; 
        prevSum = sums[i];
        sums[i] = sumsOps[i] = 0;
      }
    }
  }

  cout << "Times increased: " << count << endl;
} 