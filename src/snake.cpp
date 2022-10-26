#include "snake.h"
#include <cmath>
#include <iostream>
#include <mutex>

//copy constructor

Snake::Snake(const Snake &orig) {
  std::unique_lock<std::mutex> lock(orig._mutex);
  speed = orig.speed;
  grid_height = orig.grid_height;
  grid_width = orig.grid_width;
  size = orig.size;
  alive = orig.alive;
  head_x = orig.head_x;
  head_y = orig.head_y;
  body = orig.body;
  growing = orig.growing;
  super_growing = orig.super_growing;
}

//copy assignment

Snake &Snake::operator=(const Snake &orig) {
  	if (this == &orig) {
      return *this;
    }
    std::unique_lock<std::mutex> lock(orig._mutex);
    grid_height = orig.grid_height;
    grid_width = orig.grid_width;
    speed = orig.speed;
    size = orig.size;
    alive = orig.alive;
    head_x = orig.head_x;
    head_y = orig.head_y;
    body = orig.body;
    growing = orig.growing;
    super_growing = orig.super_growing;
    return *this;
}

// move constructor

Snake::Snake(Snake &&orig) {
    std::unique_lock<std::mutex> lock(orig._mutex);
    grid_height = orig.grid_height;
    grid_width = orig.grid_width;
    speed = orig.speed;
    size = orig.size;
    alive = orig.alive;
    head_x = orig.head_x;
    head_y = orig.head_y;
    body = std::move(orig.body);
    growing = orig.growing;
    super_growing = orig.super_growing;
}

// move assignment

Snake &Snake::operator=(Snake &&orig) { 
    if (this == &orig) {
      return *this;
    }
    std::unique_lock<std::mutex> lock(orig._mutex);
    grid_height = orig.grid_height;
    grid_width = orig.grid_width;
    speed = orig.speed;
    size = orig.size;
    alive = orig.alive;
    head_x = orig.head_x;
    head_y = orig.head_y;
    body = std::move(orig.body);
    growing = orig.growing;
    super_growing = orig.super_growing;
    return *this;
}

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing && !super_growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } 
  else if (growing)  {
    growing = false;
    size++;
  }
  else {
    super_growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

void Snake::BoostBody() { 
  growing =true;
  super_growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}