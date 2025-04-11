#include "Object/Stair.hpp"

Stair::Stair(const glm::vec2 &position, glm::vec2 scale, std::string direction, const std::string path): ImageItems(path, scale) {
   m_Transform.translation = position;
   m_Transform.scale = scale;
   glm::vec2 size = GetScaledSize();
   this->width = size.x;
   this->height = size.y;
   SetZIndex(10);
}

const glm::vec2& Stair::GetPosition() const { return m_Transform.translation; }

const glm::vec2& Stair::GetSize() const { return glm::vec2(width, height); }