#ifndef STAGE1_HPP
#define STAGE1_HPP
#include "Scene.hpp"

#include "Object/AnimatedItems.hpp"

class Stage1 : public Scene{
public:
    void Start(App*) override;
    void Update(App*) override;
    void End(App*) override;

    // check if loot already drop
    int isStopwatch = 0; // 1
    int isHolyWater = 0; // 1
    int isWhip = 0; // depends on the whip level
private:
    int mapWidth = 6144;
};
#endif

