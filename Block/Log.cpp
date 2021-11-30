#include "Log.hpp"

Log::Log() : Block("log", 64)
{
    harvestLevel = HarvestLevel::Axe;
}

const QImage *Log::acquireModel(Texture::Ptr texture) const
{
    return texture->getTexture("log");
}

Log::BlockType Log::get() const
{
    return BlockType::LOG;
}

bool Log::checkCanPass()
{
    return true;
}
