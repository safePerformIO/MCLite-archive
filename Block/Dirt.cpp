#include "Dirt.hpp"

Dirt::Dirt() : Block("dirt", 40)
{
    harvestLevel = HarvestLevel::Shovel;
}

const QImage *Dirt::acquireModel(Texture::Ptr texture) const
{
    return texture->getTexture("dirt");
}

Dirt::BlockType Dirt::get() const
{
    return BlockType::DIRT;
}

bool Dirt::checkCanPass()
{
    return false;
}