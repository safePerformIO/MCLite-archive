#include "Leaves.hpp"

Leaves::Leaves() : Block("leaves", 32)
{
    harvestLevel = HarvestLevel::Sword;
}

const QImage *Leaves::acquireModel(Texture::Ptr texture) const
{
    return texture->getTexture("leaves");
}

Leaves::BlockType Leaves::get() const
{
    return BlockType::LEAVES;
}

bool Leaves::checkCanPass()
{
    return true;
}
