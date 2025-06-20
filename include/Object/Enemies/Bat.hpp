#ifndef BAT_HPP
#define BAT_HPP

#include "Object/Enemy.hpp"
#include "Object/Character.hpp"

class Bat : public Enemy {
public:
   Bat(glm::vec2 position, std::string direction);
   void MoveBehav() override;
   bool CollideDetection(std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu, int modeState) override;
   void SetReset() override;
private:
   std::vector<std::string> batImages;
   float y_vel = 0.0f;
   float x_initial = 0.0f;
};

#endif