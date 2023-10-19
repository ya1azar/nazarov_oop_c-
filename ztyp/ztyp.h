#pragma once

#include <string>
#include <vector>

namespace zt {

struct Vector2d {
    float x, y;

    Vector2d operator*(float t) const {
        return { x * t, y * t };
    }

    Vector2d operator+(const Vector2d& o) const {
        return { x + o.x, y + o.y };
    }
};

class Weapon {
    public:
    virtual ~Weapon() {}
};

class Rocket : public Weapon {
    public:

    private:
    friend class Player;

    Rocket(const Vector2d& p, const Vector2d& v) :
       position_(p), velocity_(v) {}

    Vector2d position_;
    Vector2d velocity_;
};

class Emp : public Weapon {
    public:

    private:
    friend class Player;

    Vector2d position_;
    float radius_ = 0;
};

class SpaceShip {
    public:
    SpaceShip(const std::string& name, const Vector2d& p, const Vector2d& v) :
        name_(name), position_(p), velocity_(v) {
        }
    virtual ~SpaceShip() {}

    virtual std::vector<SpaceShip*> Update(float dt) = 0;
    virtual void Damage(Weapon* w) = 0;

    const std::string& GetName() const {
        return name_;
    }

    const Vector2d& GetPosition() const {
        return position_;
    }

    protected:
    std::string name_;
    Vector2d position_;
    Vector2d velocity_;
};

class SmallShip : public SpaceShip {
  public:
  using SpaceShip::SpaceShip;
  std::vector<SpaceShip*> Update(float dt) override {
      position_ = position_ + velocity_ * dt;
      return {};
  }
  void Damage(Weapon* w) override {
  }
};

class MotherShip : public SpaceShip {
  public:
  using SpaceShip::SpaceShip;
  std::vector<SpaceShip*> Update(float dt) override {
      return {};
  }
  void Damage(Weapon* w) override {
  }
};

class SpamShip : public SpaceShip {
  public:
  using SpaceShip::SpaceShip;
  std::vector<SpaceShip*> Update(float dt) override {
      position_ = position_ + velocity_ * dt;

      t_ = t_ + dt;
      if (t_ > 10) {
          t_ = 0;

          std::vector<SpaceShip*> result;
          result.push_back(new SmallShip("", GetPosition(), { -1, 2 }));
          result.push_back(new SpamShip("", GetPosition(), { 0, 2 }));
          result.push_back(new SmallShip("", GetPosition(), { 1, 2 }));

          return result;
      }
      return {};
  }
  void Damage(Weapon* w) override {
  }

  private:
    float t_ = 0;
};

class Player {
    public:
    Player() {}

    Weapon* Shoot() {
        return nullptr;
    }

    const SpaceShip* GetTarget() const {
        return target_;
    }

    void AimTarget(SpaceShip* target) {
        target_ = target;
    }

    int GetHP() const {
        return hp_;
    }

    int GetEmp() const {
        return emp_;
    }

    private:
    SpaceShip* target_ = nullptr;
    int emp_ = 3;
    int hp_ = 10;
};

}
