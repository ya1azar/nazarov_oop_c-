#pragma once

#include "../graphics/graphics.h"

#include <functional>
#include <memory>
#include <vector>


class Cell {
 public:
  virtual ~Cell() = default;
  virtual void Render(int x, int y) = 0;

  void OnVisit(std::function<void()> on_visit) { this->on_visit = on_visit; }

  void Visit() {
    if (on_visit) {
      on_visit();
    }
  }

 private:
  std::function<void()> on_visit;
};

class EmptyCell : public Cell {
 public:
  void Render(int x, int y) override {
    SDL_SetRenderDrawColor(render::GetRenderer(), 255, 255, 255, 255);
    SDL_RenderDrawRect(render::GetRenderer(),
                       render::MakeRect(x * 32, y * 32, 32, 32));
  }
};

class AppleCell : public Cell {
 public:
  void Render(int x, int y) {
    SDL_SetRenderDrawColor(render::GetRenderer(), 255, 255, 255, 255);
    SDL_RenderDrawRect(render::GetRenderer(),
                       render::MakeRect(x * 32, y * 32, 32, 32));

    render::DrawImage("apple.png", x * 32, y * 32,  32, 32);
  }
};

class GameField {
 public:
  GameField(int w = 24, int h = 24) {
    fields.resize(w);
    for (auto& column : fields) {
      column.resize(h);
      for (auto& cell : column) {
        cell = std::make_unique<EmptyCell>();
      }
    }
  }

  int GetWidth() const { return fields.size(); }
  int GetHeight() const { return fields[0].size(); }

  void Render() {
    // Render background
    for (int x = 0; x < GetWidth(); ++x) {
      for (int y = 0; y < GetHeight(); ++y) {
        fields[x][y]->Render(x, y);
      }
    }
  }

  Cell* GetCell(int x, int y) const {
    if (x < 0 || x >= fields.size())
      return nullptr;
    if (y < 0 || y >= fields[x].size())
      return nullptr;
    return fields[x][y].get();
  }

  Cell* SetCell(int x, int y, Cell* cell) {
    fields[x][y].reset(cell);
    return fields[x][y].get();
  }

 private:
  std::vector<std::vector<std::unique_ptr<Cell>>> fields;
};

struct Coords {
  int x, y;

  bool operator ==(const Coords& o) const {
    return x == o.x && y == o.y;
  }
};

enum Direction {
  NONE,
  UP,
  DOWN,
  LEFT,
  RIGHT,
};

class Snake {
 public:
  explicit Snake(const Coords& head) {
    direction = NONE;
    units.push_back(head);
  }

  const std::vector<Coords>& GetUnits() const { return units; }

  void SetHead(const Coords& head) { units[0] = head; }
  const Coords& GetHead() const { return units[0]; }

  void SetDirection(Direction dir) {
    if (direction != NONE && dir == NONE)
      return;
    if (direction == UP && dir == DOWN)
      return;
    if (direction == DOWN && dir == UP)
      return;
    if (direction == LEFT && dir == RIGHT)
      return;
    if (direction == RIGHT && dir == LEFT)
      return;
    direction = dir;
  }

  void Grow(int count) { grow += count; }

  void UpdateState() {
    Coords new_head = units[0];
    switch (direction) {
      case NONE:
        return;
      case UP:
        new_head.y--;
        break;
      case DOWN:
        new_head.y++;
        break;
      case RIGHT:
        new_head.x++;
        break;
      case LEFT:
        new_head.x--;
        break;
    }
    units.insert(units.begin(), new_head);
    if (grow > 0) {
      --grow;
      return;
    }

    units.pop_back();
  }

  void Render() {
    for (const Coords& u : units) {
      SDL_SetRenderDrawColor(render::GetRenderer(), 255, 191, 0, 0);
      SDL_RenderFillRect(render::GetRenderer(),
                         render::MakeRect(u.x * 32 + 3, u.y * 32 + 3, 26, 26));
    }
  }

 private:
  Direction direction;
  std::vector<Coords> units;
  int grow = 0;
};
