#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"

#include "blocks.hpp"
#include "bullets.hpp"
#include "paddle.hpp"
#include "starlayers.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_starsProgram{};
  GLuint m_objectsProgram{};
  GLuint m_paddleProgram{};

  GameData m_gameData;

  Blocks m_blocks;
  Bullets m_bullets;
  Paddle m_paddle;
  StarLayers m_starLayers;

  abcg::Timer m_restartWaitTimer;

  ImFont *m_font{};

  std::default_random_engine m_randomEngine;

  void restart();
  void checkCollisions();
  void checkWinCondition();
};

#endif