#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <mutex>
#include "SDL.h"


class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  Snake(const Snake &orig); // copy constructor
  Snake& operator = (const Snake &orig); // copy assignment
  Snake(Snake &&orig); // move constructor
  Snake& operator = (Snake &&orig); // move assignment

  ~Snake() = default; // destructor

  void Update();

  void GrowBody();
  void BoostBody();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  bool super_growing{false};
  int grid_width;
  int grid_height;
  mutable std::mutex _mutex;
};

#endif