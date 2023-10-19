#pragma once

#include <SDL.h>

#include <filesystem>
#include <string>
#include "atlas.h"


namespace render {

class RenderWindow {
 public:
  RenderWindow(int width, int height);
  virtual ~RenderWindow();

 public:
  friend SDL_Renderer* GetRenderer();

  int width_ = 0;
  int height_ = 0;
  static SDL_Window* sdl_window_;
  static SDL_Renderer* sdl_renderer_;
};

SDL_Renderer* GetRenderer();

SDL_Texture* GetTexture(const std::string& name);

std::string LoadResource(const std::filesystem::path& path,
                         const std::string& name = {});
void BakeAtlas(class Atlas& atlas);
void FreeAllResources();

void DrawImage(const std::string& name, int x, int y, int w = 0, int h = 0);
void DrawImageFromAtlas(const std::string& name,
                        const std::string& line,
                        int frame,
                        int x,
                        int y,
                        int w = 0,
                        int h = 0);

const SDL_Rect* MakeRect(int x, int y, int w, int h);

}  // namespace render