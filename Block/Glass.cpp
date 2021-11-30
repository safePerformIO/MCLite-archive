#include "Glass.hpp"

Glass::Glass() : Block("glass", 32)
{
    harvestLevel = HarvestLevel::Hand;
}

const QImage *Glass::acquireModel(Texture::Ptr texture) const
{
    return texture->getTexture("glass");
}

Glass::BlockType Glass::get() const
{
    return BlockType::GLASS;
}

bool Glass::checkCanPass()
{
    return true;
}
