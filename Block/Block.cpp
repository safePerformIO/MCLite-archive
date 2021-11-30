#include "Block.hpp"

double Block::getCurrentDurability() const
{
    return durability;
}

bool Block::isDestoryed() const
{
    return durability <= 0;
}

bool Block::isTangible() const
{
    return tangible;
}

void Block::updateState(HarvestLevel h, const int &dt)
{
    if (harvestLevel == HarvestLevel::Unbreakable)
        return;
    if (harvestLevel == h)
        durability -= 0.2 * dt;
    durability -= 0.1 * dt;
}

const QImage* Block::acquireModel(Texture::Ptr texture) const
{
    return texture->getTexture(textureId);
}

int Block::getHp() const{
    return durability;
}

