#include "CobbleStone.hpp"

CobbleStone::CobbleStone() : Block("cobble", 100)
{
    harvestLevel = HarvestLevel::Pickaxe;
}

const QImage *CobbleStone::acquireModel(Texture::Ptr texture) const
{
    return texture->getTexture("cobblestone");
}

CobbleStone::BlockType CobbleStone::get() const
{
    return BlockType::COBBLESTONE;
}

bool CobbleStone::checkCanPass()
{
    return false;
}