#ifndef Stair_H
#define Stair_H
#include "Object/ImageItems.hpp"
#include "Util/GameObject.hpp"

class Stair : public ImageItems{
   public:
      Stair(const glm::vec2 &position, glm::vec2 scale, std::string direction, std::string path=GA_RESOURCE_DIR"/fonts/Menu/weapon-frame.png");

      const glm::vec2& GetPosition() const;

      const glm::vec2& GetSize() const;

   private:
      float width;
      float height;

};
#endif