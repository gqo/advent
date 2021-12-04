#include <iostream>
#include <fstream>
using namespace std;

int main() {
  ifstream input("input.txt");
  if (!input) {
    cerr << "main(): Could not open file" << endl;
    exit(1);
  }

  int current, prev, count;

  input >> prev;

  while(input >> current) {
    if (prev < current) count++;
    prev = current;
  }

  cout << "Times increased: " << count << endl;
} 