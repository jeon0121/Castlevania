#include "Object/AnimatedItems.hpp"
#include "Utility/Time.hpp"

AnimatedItems::AnimatedItems(const std::vector<std::string> &AnimationPaths, int interval, glm::vec2 scale){
   m_Drawable = std::make_shared<Util::Animation>(AnimationPaths, false, interval, false, 0);
   m_Transform.scale = scale;
}

bool AnimatedItems::IsLooping() const {
   return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetLooping();
}

bool AnimatedItems::IsPlaying() const {
   return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetState() == Util::Animation::State::PLAY;
}

void AnimatedItems::SetLooping(bool looping) {
   auto animation = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
   animation->SetLooping(looping);
}

void AnimatedItems::SetPlaying() {
   auto animation = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
   animation->Play();
   if (ifStart) {
      s_Initial = SDL_GetPerformanceCounter();
   }
   ifStart = false;
}

void AnimatedItems::SetPaused() {
   auto animation = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
   animation->Pause();
   ifStart = true;
}

int AnimatedItems::GetCurrentFrameIndex(){
   auto animation = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
   return animation->GetCurrentFrameIndex();
}

void AnimatedItems::SetCurrentFrame(std::size_t index) {
   auto animation = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
   animation->SetCurrentFrame(index);
}

void AnimatedItems::SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }

const glm::vec2& AnimatedItems::GetPosition() const { return m_Transform.translation; }

void AnimatedItems::Move(const std::shared_ptr<AnimatedItems> &object, int ifRight, int ifUp, float distance, float duration) {
   object->SetPlaying();
   object->SetLooping(true);
   if (object->IsPlaying() && !object->IfPlayingTime(duration)) {
      glm::vec2 pos = object->GetPosition();
      (ifRight == 0) ? pos.x+=0 : ((ifRight > 0) ? pos.x += distance : pos.x -= distance);
      (ifUp == 0) ? pos.y+=0 : ((ifUp > 0) ? pos.y += distance : pos.y -= distance);
      object->SetPosition(pos);
   }
}

bool AnimatedItems::IfPlayingTime(float duration) {
   auto animation = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
   float nowTime = Time::GetRunTimeMs(s_Initial) / 1000;
   if (nowTime >= duration) {
      animation->Pause();
      return true;
   }
   return false;
}

bool AnimatedItems::IfAnimationEnds() const {
   auto animation = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
   return animation->GetCurrentFrameIndex() == animation->GetFrameCount() - 1;
}