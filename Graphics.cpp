#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <unordered_map>
#include <vector>
#define x first
#define y second
using namespace std;
/**
 * This class contains implementation for some graphic primitives(like line,
 * circle, ellipse, etc) drawing algorithms. This class doesn't use any external
 * library for drawing the graphic primitives instead it just imitates the
 * graphics drawing with the help of (n x m) matrix, if the coordinates value is
 * '0' the color would be white, and if it's '1' the color would be red. NOTE:
 * This class is intended for educational & demonstration purposes only.
 */
class GraphicsPrimitives {
  vector<vector<int>> screen;
  unordered_map<string, int> COLORS = {
      {"WHITE", 0},
      {"RED", 1},
      {"BLUE", 2},
      {"GREEN", 3},
  };
  void fixedDelay() {
    for (int i(0); i < 1e6; i++) {
    }
  }
  void floodFill(pair<int, int> point, vector<pair<int, int>> &coordinates) {
    if (point.x < 0 || point.x >= this->screen.size() || point.y < 0 ||
        point.y >= this->screen.back().size()) {
      return;
    }
    coordinates.push_back(
        {point.x, point.y}); // TODO: IT's not working properly!
    if (this->screen[point.x][point.y] == 1) {
      return;
    }
    this->screen[point.x][point.y] = 1; // Using screen matrix as a cache;

    // Side-wise
    floodFill({point.x, point.y + 1}, coordinates);
    floodFill({point.x, point.y - 1}, coordinates);
    floodFill({point.x + 1, point.y}, coordinates);
    floodFill({point.x - 1, point.y}, coordinates);

    // Diagonally
    floodFill({point.x + 1, point.y + 1}, coordinates);
    floodFill({point.x + 1, point.y - 1}, coordinates);
    floodFill({point.x - 1, point.y + 1}, coordinates);
    floodFill({point.x - 1, point.y - 1}, coordinates);
  }
  struct CircleUtil {
    static void PlotCircle(vector<pair<int, int>> &coords,
                           pair<int, int> const &center, int const &xx,
                           int const &yy) {
      coords.push_back({center.x + xx, center.y + yy});
      coords.push_back({center.x + xx, center.y - yy});
      coords.push_back({center.x - xx, center.y + yy});
      coords.push_back({center.x - xx, center.y - yy});
      coords.push_back({center.x + yy, center.y + xx});
      coords.push_back({center.x + yy, center.y - xx});
      coords.push_back({center.x - yy, center.y + xx});
      coords.push_back({center.x - yy, center.y - xx});
    }
  };

public:
  GraphicsPrimitives() {}
  GraphicsPrimitives(int x, int y) { screen.resize(x, vector<int>(y, 0)); }

protected:
  virtual void Draw() = 0;
  virtual void paint(vector<pair<int, int>> const &coordinates,
                     bool animate = false) = 0;
  vector<vector<int>> &getScreen() { return screen; }
  void DDALineAlgo(pair<int, int> p1, pair<int, int> p2) {
    if (p1.x < 0 || p1.x >= this->screen.size() || p2.x < 0 ||
        p2.x >= this->screen.back().size()) {
      cout << "Invalid coordinates!\n";
      return;
    }
    if (p1.x > p2.x) {
      swap(p1, p2);
    }
    int dx = p2.x - p1.x, dy = p2.y - p1.y;
    int steps = max(abs(dy), abs(dx));
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
  void BresenhamLineDrawingAlgo(
      pair<int, int> p1,
      pair<int, int> p2) { // FIXME: It's showing faulty behaviour!
    auto invalid = [&](pair<int, int> a) -> bool {
      return a.x < 0 || a.x >= this->screen.size();
    };
    if (invalid(p1) || invalid(p2)) { // Change this in case you the change the
                                      // resolution to be unequal;
      return;
    }
    int dx = p2.x - p1.x, dy = p2.y - p1.y;
    int steps = max(abs(dy), abs(dx));
    if ((steps == abs(dy) && p1.y > p2.y) ||
        (steps == abs(dx) && p1.x > p2.x)) {
      swap(p1, p2);
    }
    int P = 2 * dy + dx;
    int xx = p1.x, yy = p1.y;
    while (steps--) {
      if (!invalid({xx, yy})) {
        this->screen[xx][yy] = 1;
      }
      if (P < 0) {
        P += 2 * dy;
      } else {
        P += 2 * (dy - dx), ++yy;
      }
      xx = xx >= p2.x ? xx : ++xx;
    }
    this->Draw();
  }
  void BresenhamCircleDrawingAlgo(int Radius, pair<int, int> center = {0, 0}) {
    int xx = 0, yy = Radius;
    int P = 3 - (2 * Radius);
    vector<pair<int, int>> coords;
    while (xx <= yy) {
      CircleUtil::PlotCircle(coords, center, xx, yy);
      ++xx;
      if (P <= 0) {
        P = (4 * xx) + 6;
      } else {
        --yy;
        P = (4 * (xx - yy)) + 10;
      }
    }
    this->paint(coords);
  }
  void MidPointCircleDrawingAlgo(int const Radius,
                                 pair<int, int> center = {0, 0}) {
    int xx = 0, yy = Radius;
    int P = 1 - Radius;
    vector<pair<int, int>> coords;
    while (xx <= yy) {
      CircleUtil::PlotCircle(coords, center, xx, yy);
      if (P < 0) {
        P += (2 * xx) + 3;
      } else {
        P += 2 * (xx - yy) + 5, --yy;
      }
      ++xx;
    }
    this->paint(coords);
  }
  void FloodFillAlgo(pair<int, int> start, bool animate = false) {
    vector<pair<int, int>> coordinates;
    queue<pair<int, int>> que;
  }
  void EllipseDrawAlgo(int rx, int ry, pair<int, int> center = {0, 0}) {
    int xx = 0, yy = ry;
    int Rx2 = rx * rx, Ry2 = ry * ry;
    int tRx = 2 * Rx2, tRy = 2 * Ry2;
    int dx = 0, dy = tRx * yy, P = (Ry2 - (Rx2 - ry) + (0.25 * Rx2));
    vector<pair<int, int>> coords;
    // First Region
    while (dx < dy) {
      coords.push_back({center.x + xx, center.y + yy});
      coords.push_back({center.x + xx, center.y - yy});
      coords.push_back({center.x - xx, center.y + yy});
      coords.push_back({center.x - xx, center.y - yy});
      xx++;
      dx += tRy;
      if (P < 0) {
        P += Ry2 + dx;
      } else {
        yy--;
        dy -= tRx;
        P += Ry2 + (dx - dy);
      }
    }
    // Second Region
    P = (Ry2 * (xx + 0.5) * (xx + 0.5)) + Rx2 * (yy - 1) * (yy - 1) - Rx2 * Ry2;
    while (yy > 0) {
      coords.push_back({center.x + xx, center.y + yy});
      coords.push_back({center.x + xx, center.y - yy});
      coords.push_back({center.x - xx, center.y + yy});
      coords.push_back({center.x - xx, center.y - yy});
      --yy;
      dy -= tRx;
      if (P > 0) {
        P += Rx2 - dy;
      } else {
        ++xx;
        dx += tRy;
        P += Rx2 - dy + dx;
      }
    }
    this->paint(coords, true);
  }
};

class Graphics : public GraphicsPrimitives {
public:
  void DrawPolygon(vector<pair<int, int>> coordinates) {
    if (coordinates.size() < 2 || coordinates.size() > 10) {
      return;
    }
    DDALineAlgo(coordinates.front(), coordinates.back());
    for (int i = 0; i < coordinates.size() - 1; ++i) {
      DDALineAlgo(coordinates[i], coordinates[i + 1]);
    }
  }
  void DrawCircle(int Radius, pair<int, int> center) {
    if (Radius >= getScreen().size() - 1) {
      return;
    }
    MidPointCircleDrawingAlgo(Radius, center);
  }
};

class Screen : public Graphics {
  pair<int, int> size;
  void clearScreen() { cout << "\033[2J\033[1;1H"; }
  void resizeScreen() { getScreen().resize(size.x, vector<int>(size.y)); }
  void refresh() {
    getScreen().clear();
    this->resizeScreen();
    this->Draw();
  }
  void delay(int time = 1) {
    for (int i = 1; i <= (1e9); i += max(time, 1)) {
    }
  }
  void PlayAnimation() {
    int frames = 100, mx = 15;
    while (frames-- > 0) { // ANIMATION TIME!
      if (mx <= 0)
        break;
      MidPointCircleDrawingAlgo((mx / 2) + 1, {mx, mx});
      mx -= 2;
      delay();
    }
  }

public:
  Screen(int x, int y) {
    size = {x, y};
    this->resizeScreen();
    this->Draw();
  }
  /**
   * @brief Prints the whole matrix.
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
  void paint(vector<pair<int, int>> const &coordinates,
             bool animate = false) override {
    this->refresh();
    for (auto &coord : coordinates) {
      if (coord.x < 0 || coord.x >= getScreen().size() || coord.y < 0 ||
          coord.y >= getScreen().back().size()) {
        continue;
      }
      getScreen()[coord.x][coord.y] = 1;
      if (animate) {
        this->Draw();
        this->delay(100);
      }
    }
    if (!animate)
      this->Draw();
  }
};
int main() {
  Screen s(20, 20);
  s.DrawPolygon({{1, 1}, {1, 18}, {18, 9}});
  return cout << "\n", 0;
}
