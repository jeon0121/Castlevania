#ifndef STAGE3_HPP
#define STAGE3_HPP
#include "Scene.hpp"

#include "Object/AnimatedItems.hpp"

class Stage3 : public Scene{
public:
    void Start(App* app) override;
    void Update(App* app) override;
    void End(App* app) override;
    void BossFight(App* app);
    void DropCrystal(App* app);
    void EndAnimation(App *app);
private:
    int mapWidth = 3122;
    std::shared_ptr<Loot> m_crystal;
    Uint64 crystalTime = 0;
    Uint64 endScoreTime = 0;
    Uint64 countHeartTime = 0;
    std::shared_ptr<Util::SFX> countScore;
};
#endif

