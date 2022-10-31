#include "blocks.hpp"

#include <glm/gtx/fast_trigonometry.hpp>

void Blocks::create(GLuint program, int quantity) {
  destroy();

  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;

  // Get location of uniforms in the program
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Create blocks
  m_blocks.clear();
  m_blocks.resize(quantity);

  for (auto &block : m_blocks) {
    glm::vec2 tr = {
      m_randomDist(m_randomEngine), 
      m_randomDist(m_randomEngine)
    };
    block = makeBlock(tr);
  }
}


void Blocks::paint() {
  
  for (auto &block : m_blocks) {
    abcg::glUseProgram(m_program);

    abcg::glBindVertexArray(block.m_VAO);
    abcg::glUniform2f(m_translationLoc, block.m_translation.x,
                        block.m_translation.y);

    abcg::glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);
  }
}

void Blocks::destroy() {
  for (auto &block : m_blocks) {
    abcg::glDeleteBuffers(1, &block.m_VBO);
    abcg::glDeleteVertexArrays(1, &block.m_VAO);
  }
}

void Blocks::update(const Paddle &paddle, float deltaTime) {}

Blocks::Block Blocks::makeBlock(glm::vec2 translation,
                                            float scale) {
  Block block;

  auto &re{m_randomEngine}; // Shortcut

  // Randomly pick the number of sides
  std::uniform_int_distribution randomSides(6, 20);
  block.m_polygonSides = randomSides(re);

  // Get a random color (actually, a grayscale)
  std::uniform_real_distribution randomIntensity(0.5f, 1.0f);
  block.m_color = glm::vec4(randomIntensity(re));

  block.m_color.a = 1.0f;
  block.m_rotation = 0.0f;
  block.m_scale = scale;
  block.m_translation = translation;

  // Get a random angular velocity
  block.m_angularVelocity = m_randomDist(re);

  // Get a random direction
  glm::vec2 const direction{m_randomDist(re), m_randomDist(re)};
  block.m_velocity = glm::normalize(direction) / 7.0f;

  // Create geometry data
  std::array positions{
        // Paddle body
        glm::vec2{-15.5f, -46.0f + 02.5f}, glm::vec2{15.5f, -46.0f + 02.5f},
        glm::vec2{-15.5f, -46.0f - 02.5f}, glm::vec2{15.5f, -46.0f - 02.5f},
        };

  std::array const indices{0, 1, 3,
                           0, 2, 3,};
  // clang-format on

  // Generate VBO
  abcg::glGenBuffers(1, &block.m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, block.m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &block.m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, block.m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &block.m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(block.m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, block.m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, block.m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return block;
}