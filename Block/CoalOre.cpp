#include "CoalOre.hpp"

CoalOre::CoalOre() : Block("CoalOre", 100)
{
    harvestLevel = HarvestLevel::Pickaxe;
}

const QImage *CoalOre::acquireModel(Texture::Ptr texture) const
{
    return texture->getTexture("coalore");
}

CoalOre::BlockType CoalOre::get() const
{
    return BlockType::COALORE;
}

bool CoalOre::checkCanPass()
{
    return false;
}