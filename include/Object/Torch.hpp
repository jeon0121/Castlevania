#ifndef TORCH_HPP
#define TORCH_HPP

#include "Object/AnimatedItems.hpp"

class Torch : public AnimatedItems {
public:
   Torch(glm::vec2 position, glm::vec2 scale, int type=1);

   void IsWhipped();

   bool is_destroyed;

private:
   int type;
   std::vector<std::string> torchDeath;
};

#endif
