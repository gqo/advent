#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <tuple>
#include <optional>
using namespace std;

int toInt(char c) {
  return c - '0';
}

struct Point {
  Point (int x, int y) : x(x), y(y) {}

  int x, y;

  bool operator<(const Point& rhs) const {
    return tie(x, y) < tie (rhs.x, rhs.y);
  }
};

ostream& operator<<(ostream& os, const Point& p) {
  os << "P(" << p.x << ',' << p.y << ')';
  return os;
}

class Grid {
public:
  Grid(const vector<string>& grid) {
    height = grid.size();
    width = grid[0].length();

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        points.insert(make_pair(Point(i,j), toInt(grid[i][j])));
      }
    }
  }

  int riskLevel() {
    int acc = 0;

    map<Point, int>::iterator itr;
    for (itr = points.begin(); itr != points.end(); itr++) {
      if (checkLowPoint(itr->first, itr->second)) {
        acc += itr->second + 1;
      }
    }

    return acc;
  }

private:
  optional<int> top(Point p);
  optional<int> bottom(Point p);
  optional<int> left(Point p);
  optional<int> right(Point p);

  bool checkLowPoint(Point p, int val) {
    optional<int> t = top(p), b = bottom(p), l = left(p), r = right(p);
    if (t) { if (val >= *t) { return false; } }
    if (b) { if (val >= *b) { return false; } }
    if (l) { if (val >= *l) { return false; } }
    if (r) { if (val >= *r) { return false; } }

    return true;
  }

  int height;
  int width;
  map<Point, int> points;
};

int main() {
  ifstream input("input.txt");
  if (!input) {
    cerr << "main(): Could not open file" << endl;
    exit(1);
  }

  string line;
  vector<string> grid;
  while (getline(input, line)) {
    grid.push_back(line);
  }

  Grid g(grid);

  cout << "Part1: " << g.riskLevel() << endl;
}

optional<int> Grid::top(Point p) {
  map<Point, int>::iterator itr = points.find(Point(p.x, p.y - 1));
  if (itr == points.end()) {
    return nullopt;
  }
  
  return itr->second;
}

optional<int> Grid::bottom(Point p) {
  map<Point, int>::iterator itr = points.find(Point(p.x, p.y + 1));
  if (itr == points.end()) {
    return nullopt;
  }
  
  return itr->second;
}

optional<int> Grid::left(Point p) {
  map<Point, int>::iterator itr = points.find(Point(p.x - 1, p.y));
  if (itr == points.end()) {
    return nullopt;
  }
  
  return itr->second;
}

optional<int> Grid::right(Point p) {
  map<Point, int>::iterator itr = points.find(Point(p.x + 1, p.y));
  if (itr == points.end()) {
    return nullopt;
  }
  
  return itr->second;
}