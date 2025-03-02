#include "Object/AnimatedItems.hpp"

AnimatedItems::AnimatedItems(const std::vector<std::string> &AnimationPaths, int interval){
   m_Drawable = std::make_shared<Util::Animation>(AnimationPaths, false, interval, false, 0);
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
}

void AnimatedItems::SetPaused() {
   auto animation = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
   animation->Pause();
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

bool AnimatedItems::IfAnimationEnds() const {
   auto animation = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
   return animation->GetCurrentFrameIndex() == animation->GetFrameCount() - 1;
}