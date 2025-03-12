#ifndef BLOCK_H
#define BLOCK_H
#include "Object/ImageItems.hpp"
#include "Util/GameObject.hpp"

class Block : public ImageItems{
    public:
        Block(const glm::vec2 &position, glm::vec2 scale, std::string path=GA_RESOURCE_DIR"/fonts/Menu/weapon-frame.png");

        const glm::vec2& GetPosition() const;

        const glm::vec2& GetSize() const;

    private:
        float width;
        float height;

};
#endif