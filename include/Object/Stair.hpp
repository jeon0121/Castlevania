#ifndef Stair_H
#define Stair_H
#include "Object/ImageItems.hpp"
#include "Util/GameObject.hpp"

class Stair : public ImageItems {
   public:
      Stair(const glm::vec2 &position, std::string direction);

      static std::vector<std::shared_ptr<Stair>> CreateStair(const glm::vec2& start, const glm::vec2& end);

      void SetConnected(const std::shared_ptr<Stair>& other);

      std::shared_ptr<Stair> GetConnected() const;

      const glm::vec2& GetPosition() const;

      const std::string& GetDirection() const;

   private:
      float width;
      float height;
      std::string m_direction;
      std::weak_ptr<Stair> m_ConnectedStair;
};
#endif