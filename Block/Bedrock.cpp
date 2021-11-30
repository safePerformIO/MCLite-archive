#include "Bedrock.hpp"
#include "../Texture/Texture.hpp"

Bedrock::Bedrock() : Block("bedrock", 10000)
{
    harvestLevel = HarvestLevel::Unbreakable;
}

const QImage *Bedrock::acquireModel(Texture::Ptr texture) const
{
    return texture->getTexture("bedrock");
}

Bedrock::BlockType Bedrock::get() const
{
    return BlockType::BEDROCK;
}
bool Bedrock::checkCanPass()
{
    return false;
}
