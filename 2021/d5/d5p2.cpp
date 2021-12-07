#include <iostream>
#include <fstream>
#include <tuple>
#include <map>
#include <math.h>
using namespace std;

struct Point {
  int x, y;

  bool operator<(const Point& rhs) const {
    return tie(x, y) < tie (rhs.x, rhs.y);
  }
};

ostream& operator<<(ostream& os, const Point& p) {
  os << "P(" << p.x << ',' << p.y << ')';
  return os;
}

int main() {
  ifstream input("input.txt");
  if (!input) {
    cerr << "main(): Could not open file" << endl;
    exit(1);
  }

  map<Point, int> points;

  int x1, y1, x2, y2;
  char c;
  string arrow;
  while (input >> x1 >> c >> y1 >> arrow  >> x2 >> c >> y2) {
    // cout << x1 << c << y1 << arrow << x2 << c << y2 << endl;

    if (x1 == x2) {
      int start, end;

      if (y1 < y2) { start = y1; end = y2; }
      else { start = y2; end = y1; }

      for (int i = start; i < end + 1; i++) {
        Point p = Point{x1, i};

        map<Point, int>::iterator itr = points.find(p);
        if(itr == points.end()) {
          points.insert(make_pair(p, 0));
        } else {
          itr->second++;
        }
      }
    } else if (y1 == y2) {
      int start, end;

      if (x1 < x2) { start = x1; end = x2; }
      else { start = x2; end = x1; }

      for (int i = start; i < end + 1; i++) {
        Point p = Point{i, y1};

        map<Point, int>::iterator itr = points.find(p);
        if(itr == points.end()) {
          points.insert(make_pair(p, 0));
        } else {
          itr->second++;
        }
      }
    } else {
      int length = abs(x1 - x2);
      int stepX, stepY;

      if (x1 < x2) {
        if (y1 < y2) {
          stepX = stepY = 1;
        } else {
          stepX = 1;
          stepY = -1;
        }
      } else {
        if (y1 < y2) {
          stepX = -1;
          stepY = 1;
        } else {
          stepX = stepY = -1;
        }
      }

      for(int i = 0; i < length + 1; i++) {
        Point p = Point{x1 + i*stepX, y1 + i*stepY};

        map<Point, int>::iterator itr = points.find(p);
        if(itr == points.end()) {
          points.insert(make_pair(p, 0));
        } else {
          itr->second++;
        }
      }
    }
  }

  int acc = 0;

  map<Point, int>::iterator itr;
  for (itr = points.begin(); itr != points.end(); itr++) {
    if (itr->second > 0) acc++; 
  }

  cout << "# of intersections: " << acc << endl;
}