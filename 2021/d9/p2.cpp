#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <tuple>
#include <optional>
#include <algorithm>
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

struct GridValue {
  GridValue (int v) {
    this->v = v;
    if (v == 9) { checked = true; }  
  }

  int v;
  bool checked = false;
};

ostream& operator<<(ostream& os, const GridValue& v) {
  os << "GV(V: " << v.v << ", C?: " << v.checked << ')';
  return os;
}

class Grid {
public:
  Grid(const vector<string>& grid) {
    height = grid.size();
    width = grid[0].length();

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        points.insert(make_pair(Point(j,i), GridValue(toInt(grid[i][j]))));
      }
    }
  }

  int riskLevel() {
    int acc = 0;

    map<Point, GridValue>::iterator itr;
    for (itr = points.begin(); itr != points.end(); itr++) {
      if (checkLowPoint(itr->first, itr->second.v)) {
        acc += itr->second.v + 1;
      }
    }

    return acc;
  }

  int basins() {
    vector<int> bs;

    map<Point, GridValue>::iterator itr;
    for (itr = points.begin(); itr != points.end(); itr++) {
      if (checkLowPoint(itr->first, itr->second.v)) {
        bs.push_back(basin(itr->first, &(itr->second)));
      }
    }

    int size = bs.size();
    sort(bs.begin(), bs.end());

    return bs[size-1] * bs[size-2] * bs[size-3];
  }

private:
  optional<GridValue*> top(Point p);
  optional<GridValue*> bottom(Point p);
  optional<GridValue*> left(Point p);
  optional<GridValue*> right(Point p);

  bool checkLowPoint(Point p, int val) {
    optional<GridValue*> t = top(p), b = bottom(p), l = left(p), r = right(p);
    if (t) { if (val >= t.value()->v) { return false; } }
    if (b) { if (val >= b.value()->v) { return false; } }
    if (l) { if (val >= l.value()->v) { return false; } }
    if (r) { if (val >= r.value()->v) { return false; } }

    return true;
  }

  int basin(Point p, GridValue* val) {
    int acc = 1;

    val->checked = true;

    optional<GridValue*> t = top(p), b = bottom(p), l = left(p), r = right(p);

    if (t) {
      GridValue* t_ptr = t.value();
      if (!t_ptr->checked && (t_ptr->v > val->v)) {
        acc += basin(Point(p.x, p.y - 1), t_ptr);
      }
    }
    if (b) {
      GridValue* b_ptr = b.value();
      if (!b_ptr->checked && (b_ptr->v > val->v)) {
        acc += basin(Point(p.x, p.y + 1), b_ptr);
      }
    }
    if (l) {
      GridValue* l_ptr = l.value();
      if (!l_ptr->checked && (l_ptr->v > val->v)) { 
        acc += basin(Point(p.x - 1, p.y), *l);
      }
    }
    if (r) {
      GridValue* r_ptr = r.value();
      if (!r_ptr->checked && (r_ptr->v > val->v)) {
        acc += basin(Point(p.x + 1, p.y), *r);
      }
    }

    return acc;
  }

  int height;
  int width;
  map<Point, GridValue> points;
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
  cout << "Part2: " << g.basins() << endl;
}

optional<GridValue*> Grid::top(Point p) {
  map<Point, GridValue>::iterator itr = points.find(Point(p.x, p.y - 1));
  if (itr == points.end()) {
    return nullopt;
  }
  
  return &(itr->second);
}

optional<GridValue*> Grid::bottom(Point p) {
  map<Point, GridValue>::iterator itr = points.find(Point(p.x, p.y + 1));
  if (itr == points.end()) {
    return nullopt;
  }
  
  return &(itr->second);
}

optional<GridValue*> Grid::left(Point p) {
  map<Point, GridValue>::iterator itr = points.find(Point(p.x - 1, p.y));
  if (itr == points.end()) {
    return nullopt;
  }
  
  return &(itr->second);
}

optional<GridValue*> Grid::right(Point p) {
  map<Point, GridValue>::iterator itr = points.find(Point(p.x + 1, p.y));
  if (itr == points.end()) {
    return nullopt;
  }
  
  return &(itr->second);
}