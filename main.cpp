#include "app/baseapp.h"
#include "ztyp/ztyp.h"

#include <iostream>

class GameApp : public app::GameApp {
 public:
  GameApp(int w, int h)
      : app::GameApp(w, h) {
  }

 private:

  void Initialize() override {
    render::LoadResource("resources/images/apple.png", "apple");
    render::LoadResource("resources/images/stars.jpg", "stars");
    render::LoadResource("resources/images/gradient.png", "gradient");
    render::LoadResource("resources/images/mother.png", "mother");

    space_ships_.push_back(new zt::SmallShip("abc", {10, 10}, {0,0}));
    space_ships_.push_back(new zt::SmallShip("abc", {100, 20}, {0,1}));
    space_ships_.push_back(new zt::SpamShip("abc", {200, 10}, {0,0}));
    space_ships_.push_back(new zt::SmallShip("abc", {220, 40}, {0,0}));
    space_ships_.push_back(new zt::SmallShip("abc", {300, 50}, {0,0}));
  }

  void ProcessInput(const Uint8* keyboard, const MouseState& mouse) override {
  }

  void Render() override {
    render::DrawImage("stars", 0, 0, 480, 720);
    /// ?? entire alpha channel render::DrawImage("gradient",0, 0, 480, 720);

    for (const zt::SpaceShip* ss: space_ships_) {
      const zt::Vector2d& pos = ss->GetPosition();
      render::DrawImage("mother", pos.x, pos.y);
    }
  }

  void Update(Uint32 millis) override {

    std::vector<zt::SpaceShip*> new_ss;
    for (zt::SpaceShip* ss: space_ships_) {
      auto ns = ss->Update(0.1f);
      new_ss.insert(new_ss.end(), ns.begin(), ns.end());
      //space_ships_.insert(space_ships_.end(), new_ss.begin(), new_ss.end());
    }
    space_ships_.insert(space_ships_.end(), new_ss.begin(), new_ss.end());
  }

  zt::Player player_;
  std::vector<zt::SpaceShip*> space_ships_;
  int level_ = 1;
};

#undef main
int main() {
  try {
    GameApp(800, 800).Run();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}