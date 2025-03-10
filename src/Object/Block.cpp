#include "Object/Block.hpp"

Block::Block(int type, const glm::vec2 &position, glm::vec2 scale, const std::string path): ImageItems(path, scale) {
    m_Transform.translation = position;
    m_Transform.scale = scale;
    glm::vec2 size = GetScaledSize();
    this->width = size.x;
    this->height = size.y;
    this->type = type;
}

const glm::vec2& Block::GetPosition() const { return m_Transform.translation; }

const glm::vec2& Block::GetSize() const { return glm::vec2(width, height); }

const int Block::GetType() const { return type; }