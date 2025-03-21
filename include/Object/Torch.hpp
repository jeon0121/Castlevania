#ifndef TORCH_HPP
#define TORCH_HPP

#include "Object/AnimatedItems.hpp"

class Torch : public AnimatedItems {
public:
   Torch(int type, glm::vec2 position)
      : AnimatedItems((type == 1) ? std::vector<std::string>{GA_RESOURCE_DIR "/items/fire/fire-1.png", GA_RESOURCE_DIR "/items/fire/fire-2.png"}
                                  : std::vector<std::string>{GA_RESOURCE_DIR "/items/fire/fire-3.png", GA_RESOURCE_DIR "/items/fire/fire-4.png"},
      65) {
      SetPosition(position);
   }

   void IsWhipped() {}

private:
   int type;
};

#endif
