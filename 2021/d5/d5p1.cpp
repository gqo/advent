#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;

// #####################
// #       Math        #
// #####################
enum orient {Collinear, Clockwise, CounterClockwise};

struct Point {
  int x,y;
};

ostream& operator<<(ostream& os, const Point& p) {
  os << "P(" << p.x << ',' << p.y << ')';
  return os;
}

// G4G: https://www.geeksforgeeks.org/orientation-3-ordered-points/
orient orientation(Point p1, Point p2, Point p3) {
  int expression = (p2.y - p1.y) * (p3.x - p2.x) 
                 - (p3.y - p2.y) * (p2.x - p1.x);

  if (!expression) return orient(Collinear);

  return (expression > 0)? orient(Clockwise): orient(CounterClockwise);
}

// StackOverflow: https://stackoverflow.com/a/328110
// Return true iff q is between p and r (inclusive)
bool within(int p, int q, int r) {
  return ((p <= q) && (q <= r)) || ((r <= q) && (q <= p));
}

// See above link
// Return true iff q is on the line segment from p to r where p, q, and r are collinear
bool onSegment(Point p, Point q, Point r) {
    return within(p.x, q.x, r.x) && within(p.y, q.y, r.y);
}

// G4G: https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
/* Return true iff:
    the orientations of (p1, q1, p2) and (p1, q1, q2) are different
    AND the orientations of (p2, q2, p1) and (p2, q2, q1) are different
    OR iff:
      (p1, q1, p2), (p1, q1, q2), (p2, q2, p1), (p2, q2, q1) are collinear
      AND: 
        x-projections of (p1, q1) and (p2, q2) intersect
        OR y-projections of (p1, q1) and (p2, q2) intersect
*/
bool doesIntersect(Point p1, Point q1, Point p2, Point q2) {
  orient p1q1p2 = orientation(p1, q1, p2);
  orient p1q1q2 = orientation(p1, q1, q2);
  orient p2q2p1 = orientation(p2, q2, p1);
  orient p2q2q1 = orientation(p2, q2, q1);

  if (p1q1p2 != p1q1q2 && p2q2p1 != p2q2q1) return true;
  
  if (p1q1p2 == orient(Collinear) && onSegment(p1, p2, q1)) return true;
  if (p1q1q2 == orient(Collinear) && onSegment(p1, q2, q1)) return true;
  if (p2q2p1 == orient(Collinear) && onSegment(p2, p1, q2)) return true;
  if (p2q2q1 == orient(Collinear) && onSegment(p2, q1, q2)) return true;

  return false;
}

double lenSegment(Point p1, Point q1) {
  return sqrt(pow(abs(p1.x - q1.x),2) + pow(abs(p1.y - q1.y), 2));
}

// #####################
// #      Problem      #
// #####################

class GridPoint {
  friend ostream& operator<<(ostream& os, const GridPoint& gp) {
    os << 'G' << gp.getPoint();
    return os;
  }

  public:
    GridPoint(const Point& p) : p(p) {}

    void addIntersection(GridPoint* p) {
      intersectedPoints.push_back(p); 
    }

    int getIntersections() {
      mark_counted();
      for (GridPoint* p : intersectedPoints) { p->mark_counted(); }
      return (intersectedPoints.size() >= 1) ? 1 : 0;   
    }

    void mark_counted() { counted = true; }

    bool isCounted() const { return counted; }

    void display() const {
      cout << "GridPoint:" << endl;
      cout << '\t' << p << endl;
      cout << "\tCounted? " << counted << endl;
      cout << "\tIntersection Points: ";
      for (GridPoint* gp : intersectedPoints) {
        cout << *gp << ", ";
      }
      cout << endl;
    }

    Point getPoint() const { return p; }
    int getX() const { return p.x; }
    int getY() const { return p.y; }

  private:
    const Point p;
    vector<GridPoint*> intersectedPoints;
    bool counted = false;
};

class VSeg;
class HSeg;
// class DSeg;

class Segment {
  public:
    Segment(const Point& p, const Point& q) : p(p), q(q) {}
  
    virtual void doIntersect(VSeg&) = 0;
    virtual void doIntersect(HSeg&) = 0;

    Point getP() const { return p; }
    Point getQ() const { return q; }

  private:
    const Point p;
    const Point q;
};

class HSeg : public Segment {
  friend ostream& operator<<(ostream& os, const HSeg& s) {
    os << "HSeg(" << s.getP() << ", " << s.getQ() << ')';
    return os;
  }

  public:
    HSeg(const Point& p, const Point& q) : Segment(p,q) {
      for (int i = p.x; i < q.x + 1; i++) {
        points.push_back(GridPoint(Point{i, p.y}));
      }
    }

    void addIntersectionPoint(GridPoint* gp) {
      int index = gp->getX() - getP().x;
      points[index].addIntersection(gp);
      gp->addIntersection(&points[index]);
    }

    void addIntersectionPoints(vector<GridPoint*> gpRange) {
      for (GridPoint* gp : gpRange) {
        addIntersectionPoint(gp); 
      }
    }

    GridPoint* getGP(Point p) { return &points[p.x - getP().x]; }

    vector<GridPoint*> getGPRange(Point p, Point q) {
      vector<GridPoint*> res;
      for (int i = p.x - getP().x; i < q.x - getP().x + 1; i++) {
        res.push_back(&points[i]);
      }
      return res;
    }

    int countIntersections() {
      int acc = 0;

      for (GridPoint& gp : points) {
        if (!gp.isCounted()) {
          acc += gp.getIntersections();
        }
      }
      return acc;
    }

    void display() const {
      cout << "----- HSeg -----" << endl;
      cout << "----- Body -----" << endl;
      cout << *this << endl;
      cout << "----- Points -----" << endl;
      for (GridPoint gp : points) {
        gp.display();
      }
      cout << "----- End -----" << endl;
      
    }

    void doIntersect(VSeg& s);
    void doIntersect(HSeg& s);
    // void doIntersect(DSeg& s);

  private:
    vector<GridPoint> points;
};

class VSeg : public Segment {
  friend ostream& operator<<(ostream& os, const VSeg& s) {
    os << "VSeg(" << s.getP() << ", " << s.getQ() << ')';
    return os;
  }

  public:
    VSeg(const Point& p, const Point& q) : Segment(p,q) {
      for (int i = p.y; i < q.y + 1; i++) {
        points.push_back(GridPoint(Point{p.x, i}));
      }
    }

    void addIntersectionPoint(GridPoint* gp) {
      int index = gp->getY() - getP().y;
      points[index].addIntersection(gp);
      gp->addIntersection(&points[index]);
    }

    void addIntersectionPoints(vector<GridPoint*> gpRange) {
      for (GridPoint* gp : gpRange) {
        addIntersectionPoint(gp); 
      }
    }

    GridPoint* getGP(Point p) { return &points[p.y - getP().y]; }

    vector<GridPoint*> getGPRange(Point p, Point q) {
      vector<GridPoint*> res;
      for (int i = p.y - getP().y; i < q.y - getP().y + 1; i++) {
        res.push_back(&points[i]);
      }
      return res;
    }

    int countIntersections() {
      int acc = 0;

      for (GridPoint& gp : points) {
        if (!gp.isCounted()) acc += gp.getIntersections();
      }

      return acc;
    }

    void display() const {
      cout << "----- VSeg -----" << endl;
      cout << "----- Body -----" << endl;
      cout << *this << endl;
      cout << "----- Points -----" << endl;
      for (GridPoint gp : points) {
        gp.display();
      }
      cout << "----- End -----" << endl;
      
    }

    void doIntersect(VSeg& s);
    void doIntersect(HSeg& s);

  private:
    vector<GridPoint> points;
};

struct Segments {
  vector<HSeg*> hsegs;
  vector<VSeg*> vsegs;
};

ostream& operator<<(ostream& os, Segments segs) {
  os << "Horizontal Line Segments:";
  if (segs.hsegs.size() == 0) { os << " NONE" << endl; }
  else {
    os << endl;
    for (const HSeg* hs : segs.hsegs) {
      os << '\t' << *hs << endl;
    }
  }

  os << "Vertical Line Segments:";
  if (segs.vsegs.size() == 0) { os << " NONE" << endl; }
  else {
    os << endl;
    for (const VSeg* vs : segs.vsegs) {
      os << '\t' << *vs << endl;
    }
  }

  return os;
}

void checkIntersections(Segments& segments, HSeg& hseg) {
  for (HSeg* hs : segments.hsegs) { 
    hs->doIntersect(hseg); 
  }
  for (VSeg* vs : segments.vsegs) {
    vs->doIntersect(hseg);
  }
}
void checkIntersections(Segments& segments, VSeg& vseg) {
  for (HSeg* hs : segments.hsegs) { 
    hs->doIntersect(vseg); 
  }
  for (VSeg* vs : segments.vsegs) {
    vs->doIntersect(vseg);
  }
}

int countIntersections(Segments& segments) {
  int acc = 0;

  for (HSeg* hs : segments.hsegs) {
    acc += hs->countIntersections();
  }
  for (VSeg* vs : segments.vsegs) {
    acc += vs->countIntersections();
  }

  return acc;
}

void display(const Segments& segments) {
  cout << "#####-----#####-----#####" << endl;
  cout << "#####--- Segments --#####" << endl;
  cout << "#####-----#####-----#####" << endl;
  for (const HSeg* hs : segments.hsegs) { 
    hs->display();
  }
  for (const VSeg* vs : segments.vsegs) {
    vs->display();
  }
  cout << "#####-----#####-----#####" << endl;
  cout << "#####-----#####-----#####" << endl;
  cout << "#####-----#####-----#####" << endl;
}

int main() {
  ifstream input("input.txt");
  if (!input) {
    cerr << "main(): Could not open file" << endl;
    exit(1);
  }
  
  Segments segments;

  int x1, y1, x2, y2;
  char c;
  char arrow;
  while (input >> x1 >> c >> y1 >> arrow >> arrow >> x2 >> c >> y2) {
    if (y1 == y2) {
      Point start, end;
      if (x1 < x2) {
        start = Point{x1,y1};
        end = Point{x2,y2};
      } else {
        start = Point{x2,y2};
        end = Point{x1,y1};
      }

      HSeg* h = new HSeg(start, end);

      checkIntersections(segments, *h);
      segments.hsegs.push_back(h);
    } else if (x1 == x2) {
      Point start, end;
      if (y1 < y2) {
        start = Point{x1,y1};
        end = Point{x2,y2};
      } else {
        start = Point{x2,y2};
        end = Point{x1,y1};
      }
      VSeg* v = new VSeg(start, end);

      checkIntersections(segments, *v);
      segments.vsegs.push_back(v);
    } else {
      cout << "Diagonal!" << endl;
    }
  }

  cout << "# of intersections: " << countIntersections(segments) << endl;
} 

void HSeg::doIntersect(VSeg& s)  {
  if (doesIntersect(getP(), getQ(), s.getP(), s.getQ())) { 
    Point collision = Point{s.getP().x, getP().y};

    GridPoint* gp = s.getGP(collision);
    addIntersectionPoint(gp);
  }
};

void HSeg::doIntersect(HSeg& s) {
  Point p1 = getP(), q1 = getQ();
  Point p2 = s.getP(), q2 = s.getQ();

  if (doesIntersect(p1, q1, p2, q2)) {
    vector<GridPoint*> gps;

    if ((p1.x < p2.x) && (q1.x < q2.x)) {
      gps = s.getGPRange(p2, q1);
    } else if ((p1.x > p2.x) && (q1.x > q2.x)) {
      gps = s.getGPRange(p1, q2);
    } else if ((p1.x >= p2.x) && (q1.x <= q2.x)) {
      gps = s.getGPRange(p1, q1);
    } else {
      gps = s.getGPRange(p2, q2);
    }
    addIntersectionPoints(gps);
  }
}

void VSeg::doIntersect(VSeg& s) {
  Point p1 = getP(), q1 = getQ();
  Point p2 = s.getP(), q2 = s.getQ();

  if (doesIntersect(p1, q1, p2, q2)) {
    vector<GridPoint*> gps;

    if ((p1.y < p2.y) && (q1.y < q2.y)) {
      gps = s.getGPRange(p2, q1);
    } else if ((p1.y > p2.y) && (q1.y > q2.y)) {
      gps = s.getGPRange(p1, q2);
    } else if ((p1.y >= p2.y) && (q1.y <= q2.y)) {
      gps = s.getGPRange(p1, q1);
    } else {
      gps = s.getGPRange(p2, q2);
    }
    addIntersectionPoints(gps);
  }
}

void VSeg::doIntersect(HSeg& s)  {
  if (doesIntersect(getP(), getQ(), s.getP(), s.getQ())) {
    Point collision = Point{getP().x, s.getP().y};

    GridPoint* gp = s.getGP(collision);
    addIntersectionPoint(gp);
  }
};