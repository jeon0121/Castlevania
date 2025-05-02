#include "Object/Enemies/Bat.hpp"

Bat::Bat(glm::vec2 position, std::string direction): Enemy(position, direction, {}, 250, 100) {
   for (int i = 0; i < 3; ++i) {
      batImages.emplace_back(GA_RESOURCE_DIR"/enemies/bat/bat-" + std::to_string(i + 1) + ".png");
   }
   SetAnimationFrames(batImages, 250);
   m_Transform.scale = glm::vec2(1, 0.9f);
   if (direction == "right")
      Flip();
   SetPlaying();
   SetLooping(true);
   countHurt = 2;
}

void Bat::MoveBehav() {
   glm::vec2 pos = GetPosition();
   pos.y -= 2 * sin(pos.x / 50.0f);
   pos.x += (direction == "right" ? 3 : -3);
   SetPosition(pos);
}

void Bat::SetReset() {
   is_dead = false;
   is_hidden = false;
   hasEnteredWindow = false;
   SetAnimationFrames(batImages, 250);
   SetPlaying();
   SetLooping(true);
}

void Bat::SetDirection(std::string direction) {
   if (direction != this->direction)
      Flip();
   this->direction = direction;
}