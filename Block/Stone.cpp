#include "Stone.hpp"

Stone::Stone() : Block("stone", 80)
{
    harvestLevel = HarvestLevel::Pickaxe;
}

Stone::BlockType Stone::get() const
{
    return BlockType::STONE;
}

bool Stone::checkCanPass()
{
    return false;
}

const QImage *Stone::acquireModel(Texture::Ptr texture) const
{
    return texture->getTexture("stone");
}
