#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>


namespace render {

class Atlas {
 public:
  struct AnimationLine {
    std::string name;
    int y_offset = 0;
    int frames_count = 0;

    int frame_width = 0;
    int frame_height = 0;
    bool with_reverse = false;

    AnimationLine& SetFramesCount(int count, bool with_reverse = false);
    AnimationLine& SetFrameWidth(int width);
    AnimationLine& SetFrameHeight(int height);
  };

  static Atlas Create(const std::filesystem::path& path,
                      const std::string& name = {});

  AnimationLine& AddAnimationLine(const std::string& name);

  const std::string& GetName() const;
  const AnimationLine& GetAnimationLine(const std::string& name) const;
  void Bake();

  Atlas() = default;

 private:
  explicit Atlas(const std::string& name);

  std::string name_;
  std::vector<AnimationLine> animation_lines_;
};

}  // namespace render
