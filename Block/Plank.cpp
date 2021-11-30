#include "Plank.hpp"

Plank::Plank() : Block("plank", 64)
{
    harvestLevel = HarvestLevel::Axe;
}

Plank::BlockType Plank::get() const
{
    return BlockType::PLANK;
}

bool Plank::checkCanPass()
{
    return false;
}

const QImage *Plank::acquireModel(Texture::Ptr texture) const
{
    return texture->getTexture("plank");
}