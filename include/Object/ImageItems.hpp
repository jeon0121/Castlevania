#ifndef IMAGEITEMS_HPP
#define IMAGEITEMS_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class ImageItems : public Util::GameObject {
public:
   explicit ImageItems(const std::string path) : GameObject(std::make_unique<Util::Image>(path), 0) {}
   
   [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }

   [[nodiscard]] bool GetVisibility() const { return m_Visible; }

   void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }
};

#endif