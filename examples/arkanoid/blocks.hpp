#ifndef BLOCKS_HPP_
#define BLOCKS_HPP_

#include <list>
#include <random>

#include "abcgOpenGL.hpp"

#include "gamedata.hpp"
#include "paddle.hpp"

class Blocks {
public:
  void create(GLuint program, int quantity);
  void paint();
  void destroy();
  void update(const Paddle &Paddle, float deltaTime);

  struct Block {
    GLuint m_VAO{};
    GLuint m_VBO{};
    GLuint m_EBO{};

    float m_angularVelocity{};
    glm::vec4 m_color{1};
    int m_polygonSides{};
    float m_rotation{};
    float m_scale{};
    glm::vec2 m_translation{};
    glm::vec2 m_velocity{};
    bool m_hit{};
  };

  std::list<Block> m_blocks;

  Block makeBlock(glm::vec2 translation = {}, float scale = 0.25f);

private:
  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};
};

#endif