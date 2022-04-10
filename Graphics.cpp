#include <cmath>
#include <iostream>
#include <vector>
#define x first
#define y second
using namespace std;
class CircleUtil {
public:
  enum class Quadrant { I, II, III, IV };
  void getCoordinatesForQuadrant(vector<pair<int, int>> &coords, Quadrant quad,
                                 int const &SIZE) {
    int xMul = 1, yMul = 1;
    switch (quad) {
    case Quadrant::I:
      break;
    case Quadrant::II:
      xMul = -1;
      break;
    case Quadrant::III:
      yMul = -1;
      break;
    case Quadrant::IV:
      xMul = -1, yMul = -1;
      break;
    }
    for (int i = 0; i < SIZE; i++) {
      coords.push_back({coords[i].x * xMul, coords[i].y * yMul});
    }
  }
};

/**
 * This class contains implementation for some graphic primitives(like line,
 * circle, ellipse, etc) drawing algorithms. This class doesn't use any external
 * library for drawing the graphic primitives instead it just imitates the
 * graphics drawing with the help of (n x m) matrix, if the coordinates value is
 * '0' the color would be white, and if it's '1' the color would be red. NOTE:
 * This class is intended for educational & demonstration purposes only.
 */
class Graphics : public CircleUtil {
  pair<int, int> size;
  vector<vector<int>> screen;
  void floodFill(pair<int, int> point) {
    if (point.x < 0 || point.x >= size.x || point.y < 0 || point.y >= size.y) {
      return;
    }
    this->screen[point.x][point.y] = 1;
    floodFill({point.x, point.y + 1});
    floodFill({point.x, point.y - 1});
    floodFill({point.x + 1, point.y});
    floodFill({point.x - 1, point.y});
  }

public:
  Graphics() {}
  Graphics(int x, int y) {
    size = {x, y};
    screen.resize(x, vector<int>(y, 0));
  }
  vector<vector<int>> &getScreen() { return screen; }
  virtual void Draw() = 0;
  virtual void paint(vector<pair<int, int>> const &coordinates) = 0;
  void DDALineAlgo(pair<int, int> p1, pair<int, int> p2) {
    if (p1.x < 0 || p1.x >= this->screen.size() || p2.x < 0 ||
        p2.x >= this->screen.back().size()) {
      cout << "Invalid coordinates!\n";
      return;
    }
    int dx = p2.x - p1.x, dy = p2.y - p1.y;
    int steps = max(dy, dx);
    double xInc = double(dx) / double(steps);
    double yInc = double(dy) / double(steps);
    double xx = p1.x, yy = p1.y;
    while (steps--) {
      this->screen[round(xx)][round(yy)] = 1;
      xx = double(xx + xInc);
      yy = double(yy + yInc);
    }
    this->Draw();
  }
  void BresenhamLineDrawingAlgo(pair<int, int> p1, pair<int, int> p2) {
    if (p1.x < 0 || p1.x >= this->screen.size() || p2.x < 0 ||
        p2.x >= this->screen.back().size()) {
      return;
    }
    int dx = p2.x - p1.x, dy = p2.y - p1.y;
    int steps = max(dy, dx);
    int P = 2 * dy + dx;
    int xx = p1.x, yy = p1.y;
    while (xx <= p2.x) {
      this->screen[xx++][yy] = 1;
      if (P < 0) {
        P += 2 * dy;
      } else {
        P += 2 * (dy - dx), ++yy;
      }
    }
    this->Draw();
  }
  void BresenhamCircleDrawingAlgo(int Radius, pair<int, int> center = {0, 0}) {
    int xx = center.x, yy = Radius + center.x;
    int P = 3 - (2 * Radius);
    vector<pair<int, int>> coords;
    while (xx <= yy) {
      coords.push_back({xx++, yy});
      if (P <= 0) {
        P = (4 * xx) + 6;
      } else {
        --yy;
        P = (4 * (xx - yy)) + 10;
      }
    }
    for (int i = 0, n = coords.size(); i < n; i++) {
      coords.push_back({coords[i].y, coords[i].x});
    }
    const int SIZE = coords.size();
    getCoordinatesForQuadrant(coords, Quadrant::II, SIZE);
    getCoordinatesForQuadrant(coords, Quadrant::III, SIZE);
    getCoordinatesForQuadrant(coords, Quadrant::IV, SIZE);
    this->paint(coords);
  }
  void MidPointCircleDrawingAlgo(int const Radius,
                                 pair<int, int> center = {0, 0}) {
    int xx = 0, yy = Radius;
    int P = 1 - Radius;
    vector<pair<int, int>> coords;
    while (xx <= yy) {
      coords.push_back({center.x + xx, center.y + yy});
      coords.push_back({center.x + xx, center.y - yy});
      coords.push_back({center.x - xx, center.y + yy});
      coords.push_back({center.x - xx, center.y - yy});
      coords.push_back({center.x + yy, center.y + xx});
      coords.push_back({center.x + yy, center.y - xx});
      coords.push_back({center.x - yy, center.y + xx});
      coords.push_back({center.x - yy, center.y - xx});
      if (P < 0) {
        P += (2 * xx) + 3;
      } else {
        P += 2 * (xx - yy) + 5, --yy;
      }
      ++xx;
    }
    for (int i = 0, n = coords.size(); i < n; i++) {
      coords.push_back({coords[i].y, coords[i].x});
    }
    this->paint(coords);
  }
  void FloodFillAlgo(pair<int, int> start, bool animate = false) {
    floodFill(start);
  }
  void EllipseDrawAlgo(int rx, int ry, pair<int, int> center = {0, 0}) {
    int xx = 0, yy = ry;
    int Rx2 = rx * rx, Ry2 = ry * ry;
    int tRx = 2 * Rx2, tRy = 2 * Ry2;
    int dx = 0, dy = tRx * yy, P = (Ry2 - (Rx2 - ry) + (0.25 * Rx2));
    vector<pair<int, int>> coords;
    while (dx < dy) {
      xx++;
      dx += tRy;
      if (P < 0) {
        P += Ry2 + dx;
      } else {
        yy--;
        dy -= tRx;
        P += Ry2 + (dx - dy);
      }
      coords.push_back({center.x + xx, center.y + yy});
      coords.push_back({center.x + xx, center.y - yy});
      coords.push_back({center.x - xx, center.y + yy});
      coords.push_back({center.x - xx, center.y - yy});
    }
    // Second Region
    P = (Ry2 * (xx + 0.5) * (xx + 0.5)) + Rx2 * (yy - 1) * (yy - 1) - Rx2 * Ry2;
    while (yy > 0) {
      --yy;
      dy -= tRx;
      if (P > 0) {
        P += Rx2 - dy;
      } else {
        ++xx;
        dx += tRy;
        P += Rx2 - dy + dx;
      }
      coords.push_back({center.x + xx, center.y + yy});
      coords.push_back({center.x + xx, center.y - yy});
      coords.push_back({center.x - xx, center.y + yy});
      coords.push_back({center.x - xx, center.y - yy});
    }
    this->paint(coords);
  }
};

class Screen : public Graphics {
  void clearScreen() { cout << "\033[2J\033[1;1H"; }

public:
  Screen(int x, int y) {
    getScreen().resize(x, vector<int>(y));
    this->Draw();
  }
  /**
   * @brief Prints the whole matrix.
   *
   */
  void Draw() override {
    this->clearScreen();
    for (auto &sr : getScreen()) {
      for (auto &s : sr) {
        if (s == 0) {
          cout << s << " ";
        } else {
          cout << "\033[1;31m" << s << "\033[0m"
               << " ";
        }
      }
      cout << "\n";
    }
  }
  /**
   * @brief Paint certain coordinates and calls Draw();
   *
   * @param coordinates
   */
  void paint(vector<pair<int, int>> const &coordinates) override {
    for (auto &coord : coordinates) {
      if (coord.x < 0 || coord.x >= getScreen().size() || coord.y < 0 ||
          coord.y >= getScreen().back().size()) {
        continue;
      }
      getScreen()[coord.x][coord.y] = 1;
    }
    this->Draw();
  }
  void refresh() { getScreen().clear(), this->Draw(); }
  void delay(int time = 1) {
    for (int i = 1; i <= (1e9 / time); i++) {
    }
  }
};
int main() {
  Screen s(20, 20); // Make sure to pass the coordinates like this -> (smaller
                    // coordinate, larger coordinate) for line drawing algo;
  /* int frames = 100, mx = 15;
  while (frames-- > 0) { // ANIMATION TIME!
    if (mx <= 0)
      break;
    s.MidPointCircleDrawingAlgo((mx / 2) + 1, {mx, mx});
    mx -= 2;
    s.delay();
  } */
  s.EllipseDrawAlgo(3, 6, {10, 10});
  return cout << "\n", 0;
}
