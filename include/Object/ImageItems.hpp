#ifndef IMAGEITEMS_HPP
#define IMAGEITEMS_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class ImageItems : public Util::GameObject {
public:
   explicit ImageItems(const std::string path, glm::vec2 scale=glm::vec2(1.0f, 1.0f))
   : GameObject(std::make_unique<Util::Image>(path), 0) {
      m_Transform.scale = scale;
   }
   
   void SetImage(const std::string& imagePath) {
      m_Drawable = std::make_shared<Util::Image>(imagePath);
   }

   [[nodiscard]] virtual const glm::vec2& GetPosition() const { return m_Transform.translation; }

   [[nodiscard]] bool GetVisibility() const { return m_Visible; }

   virtual void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }
};

#endif