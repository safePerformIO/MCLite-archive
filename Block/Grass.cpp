#include "Grass.hpp"

Grass::Grass() : Block("grass", 40)
{
    harvestLevel = HarvestLevel::Shovel;
}

const QImage *Grass::acquireModel(Texture::Ptr texture) const
{
    return texture->getTexture("grass");
}

Grass::BlockType Grass::get() const
{
    return BlockType::GRASS;
}

bool Grass::checkCanPass()
{
    return false;
}
