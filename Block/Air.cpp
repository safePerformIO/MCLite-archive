#include "Air.hpp"

const QImage *Air::acquireModel(Texture::Ptr = nullptr) const
{
    return nullptr;
}

Air::BlockType Air::get() const
{
    return BlockType::AIR;
}

bool Air::checkCanPass()
{
    return true;
}
