#include "atlas.h"
#include "graphics.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace render {

Atlas::AnimationLine& Atlas::AnimationLine::SetFramesCount(int count, bool wr) {
  frames_count = count;
  with_reverse = wr;
  return *this;
}

Atlas::AnimationLine& Atlas::AnimationLine::SetFrameWidth(int width) {
  frame_width = width;
  return *this;
}

Atlas::AnimationLine& Atlas::AnimationLine::SetFrameHeight(int height) {
  frame_height = height;
  return *this;
}

// static
Atlas Atlas::Create(const std::filesystem::path& path,
                    const std::string& name) {
  return Atlas(LoadResource(path, name));
}

Atlas::Atlas(const std::string& name) : name_(name) {}

Atlas::AnimationLine& Atlas::AddAnimationLine(const std::string& name) {
  try {
    GetAnimationLine(name);
  } catch (std::invalid_argument&) {
    AnimationLine al;
    al.name = name;
    animation_lines_.push_back(std::move(al));
    return animation_lines_.back();
  }
  throw std::invalid_argument(name + " animation line already exists in " +
                              name_ + " atlas");
}

const std::string& Atlas::GetName() const {
  return name_;
}

const Atlas::AnimationLine& Atlas::GetAnimationLine(const std::string& name) const {
  auto fnd = std::find_if(animation_lines_.begin(), animation_lines_.end(),
                          [&name](const auto& al) { return al.name == name; });
  if (fnd == animation_lines_.end()) {
    throw std::invalid_argument(name + " animation line not found in the " +
                                name_ + " atlas");
  }
  return *fnd;
}

void Atlas::Bake() {
  auto* texture = GetTexture(name_);

  if (animation_lines_.empty()) {
    throw std::logic_error(name_ + " atlas has no animation lines");
  }

  int width = 0;
  int height = 0;
  SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

  height = height / animation_lines_.size();
  int y_offset = 0;

  for (auto& al : animation_lines_) {
    if (al.y_offset == 0) {
      al.y_offset = y_offset;
    }
    if (al.frame_width <= 0 && al.frames_count <= 0) {
      throw std::logic_error(
          al.name + " animation line in " + name_ +
          " atlas has no frames_count and/or frame_width fields");
    }

    if (al.frame_height <= 0) {
      al.frame_height = height;
    }
    if (al.frame_width <= 0 && al.frames_count > 0) {
      al.frame_width = width / al.frames_count;
    } else if (al.frames_count <= 0 && al.frame_width > 0) {
      al.frames_count = width / al.frame_width;
    }
    y_offset += al.frame_height;
  }
}

}  // namespace render
