#ifndef ANIMATED_ITEMS_HPP
#define ANIMATED_ITEMS_HPP
#include <vector>
#include <string>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"

class AnimatedItems : public Util::GameObject {
public:
    explicit AnimatedItems(const std::vector<std::string>& AnimationPaths,
    int interval, glm::vec2 scale=glm::vec2(1.0f, 1.0f));

    [[nodiscard]] bool IsLooping() const;

    [[nodiscard]] bool IsPlaying() const;

    void SetLooping(bool looping);

    void SetPlaying();

    void SetPaused();

    int GetCurrentFrameIndex();

    void SetCurrentFrame(std::size_t index);

    void SetPosition(const glm::vec2& Position);

    const glm::vec2& GetPosition() const;

    void Move(const std::shared_ptr<AnimatedItems> &object, int ifRight, int ifUp, float distance, float duration);
    //stay = 0, right = 1, left = -1

    bool IfPlayingTime(float time);

    [[nodiscard]] bool IfAnimationEnds() const;

private:
    bool ifStart = true;
    Uint64 s_Initial;
};

#endif