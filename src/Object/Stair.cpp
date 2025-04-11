#include "Object/Stair.hpp"

Stair::Stair(const glm::vec2 &position, std::string direction): ImageItems(GA_RESOURCE_DIR"/fonts/Menu/weapon-frame.png", glm::vec2(1.08, 0.72)) {
   m_Transform.translation = position;
   m_Transform.scale = glm::vec2(1.08, 0.72);
   m_direction = direction;
   glm::vec2 size = GetScaledSize();
   this->width = size.x;
   this->height = size.y;
   SetZIndex(10);
}

std::vector<std::shared_ptr<Stair>> Stair::CreateStair(const glm::vec2& start, const glm::vec2& end) {
   std::vector<std::shared_ptr<Stair>> stairs;
   std::string dir1 = "up";
   std::string dir2 = "down";
   if (start.y < end.y) std::swap(dir1, dir2);
   auto stair1 = std::make_shared<Stair>(start, dir1);
   auto stair2 = std::make_shared<Stair>(end, dir2);
   stair1->SetConnected(stair2);
   stair2->SetConnected(stair1);
   stairs.push_back(stair1);
   stairs.push_back(stair2);
   return stairs;
}

void Stair::SetConnected(const std::shared_ptr<Stair>& other) {
   m_ConnectedStair = other;
}

std::shared_ptr<Stair> Stair::GetConnected() const {
   return m_ConnectedStair.lock();
}

const glm::vec2& Stair::GetPosition() const {
   return m_Transform.translation;
}

const std::string& Stair::GetDirection() const {
   return m_direction;
}