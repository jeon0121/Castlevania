#include "Object/AnimatedItems.hpp"

AnimatedItems::AnimatedItems(const std::vector<std::string> &AnimationPaths){
   m_Drawable = std::make_shared<Util::Animation>(AnimationPaths, false, 100, false, 0);
}