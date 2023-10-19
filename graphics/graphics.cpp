#include "graphics.h"
#include "atlas.h"

#include <SDL.h>
#include <SDL_image.h>

#include <exception>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <unordered_map>


namespace render {

SDL_Renderer* GetRenderer() {
  if (!RenderWindow::sdl_renderer_)
    throw std::logic_error("Initialize RenderWindow first.");
  return RenderWindow::sdl_renderer_;
}

SDL_Window* RenderWindow::sdl_window_ = nullptr;
SDL_Renderer* RenderWindow::sdl_renderer_ = nullptr;

RenderWindow::RenderWindow(int width, int height)
    : width_(width), height_(height) {
  if (sdl_window_ || sdl_renderer_)
    throw std::logic_error("Renderer window already exist");

  SDL_Init(SDL_INIT_EVERYTHING);

  sdl_window_ =
      SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width_, height_, SDL_WINDOW_RESIZABLE);

  sdl_renderer_ = SDL_CreateRenderer(sdl_window_, -1, SDL_RENDERER_ACCELERATED);
}

RenderWindow::~RenderWindow() {
  SDL_DestroyRenderer(sdl_renderer_);
  SDL_DestroyWindow(sdl_window_);
  SDL_Quit();
}

}  // namespace render

namespace render {

class ResourceManager {
 public:
  static ResourceManager& GetInstance() {
    static ResourceManager instance;
    return instance;
  }

  void LoadResource(const std::filesystem::path& path,
                    const std::string& name) {
    SDL_Texture* texture =
        IMG_LoadTexture(GetRenderer(), path.string().c_str());
    if (!texture)
      throw std::invalid_argument("Can't find resource: " + path.string());
    textures_[name] = texture;
  }

  void FreeAllResources() {
    for (auto& [name, texture] : textures_) {
      SDL_DestroyTexture(texture);
    }
    textures_.clear();
  }

  void AddAtlas(Atlas&& atlas) { atlases_[atlas.GetName()] = std::move(atlas); }

  SDL_Texture* GetTexture(const std::string& name) {
    auto fnd = textures_.find(name);
    if (fnd == textures_.end())
      return nullptr;
    return fnd->second;
  }

  const Atlas& GetAtlas(const std::string& name) const {
    auto fnd = atlases_.find(name);
    if (fnd == atlases_.end())
      throw std::invalid_argument(name + " doesn't exist");
    return fnd->second;
  }

 private:
  ResourceManager() {
    if (!RenderWindow::sdl_window_)
      throw std::logic_error("Initialize RenderWindow first");
  }

  ~ResourceManager() { FreeAllResources(); }

  std::unordered_map<std::string, SDL_Texture*> textures_;
  std::unordered_map<std::string, Atlas> atlases_;
};

std::string LoadResource(const std::filesystem::path& path,
                         const std::string& name) {
  if (!name.empty()) {
    ResourceManager::GetInstance().LoadResource(path, name);
    return name;
  } else {
    ResourceManager::GetInstance().LoadResource(path, path.filename().string());
    return path.filename().string();
  }
}

void BakeAtlas(Atlas& atlas) {
  atlas.Bake();
  ResourceManager::GetInstance().AddAtlas(std::move(atlas));
}

SDL_Texture* GetTexture(const std::string& name) {
  auto* texture = ResourceManager::GetInstance().GetTexture(name);
  if (!texture)
    throw std::invalid_argument("Texture " + name + " is not loaded.");
  return texture;
}

void DrawImage(const std::string& name, int x, int y, int w, int h) {
  auto* texture = GetTexture(name);
  SDL_Rect rect = {x, y, w, h};
  if (w == 0 || h == 0) {
    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
  }

  SDL_RenderCopy(GetRenderer(), texture, nullptr, &rect);
}

void DrawImageFromAtlas(const std::string& name,
                        const std::string& line,
                        int frame,
                        int x,
                        int y,
                        int w,
                        int h) {
  const auto& atlas = ResourceManager::GetInstance().GetAtlas(name);
  const auto& al = atlas.GetAnimationLine(line);
  auto* texture = GetTexture(atlas.GetName());

  SDL_Rect source;
  source.y = al.y_offset;
  source.h = al.frame_height;
  source.w = al.frame_width;
  if (!al.with_reverse) {
    source.x = (frame % al.frames_count) * al.frame_width;
  } else {
    int frame_num = frame % (al.frames_count * 2 - 2);
    if (frame_num >= al.frames_count) {
      frame_num = al.frames_count - (frame_num - al.frames_count) - 2;
    }
    source.x = frame_num * al.frame_width;
  }

  if (w == 0 || h == 0) {
    w = source.w;
    h = source.h;
  }

  SDL_RenderCopy(GetRenderer(), texture, &source, MakeRect(x, y, w, h));
}

void DrawImageFromAtlas(const std::string& name,
                        int x,
                        int y,
                        int w,
                        int h,
                        int atlas_x,
                        int atlas_y,
                        int atlas_w,
                        int atlas_h) {
  auto* texture = GetTexture(name);
  SDL_RenderCopy(GetRenderer(), texture,
                 MakeRect(atlas_x, atlas_y, atlas_w, atlas_h),
                 MakeRect(x, y, w, h));
}

void FreeAllResources() {
  ResourceManager::GetInstance().FreeAllResources();
}

const SDL_Rect* MakeRect(int x, int y, int w, int h) {
  static SDL_Rect r[8];
  static int i = -1;
  i = (i + 1) % std::size(r);
  r[i].x = x;
  r[i].y = y;
  r[i].w = w;
  r[i].h = h;
  return &r[i];
}

}  // namespace render
