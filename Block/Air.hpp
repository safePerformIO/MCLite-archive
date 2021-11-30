#pragma once

#include "Block.hpp"

/* The Air Block that does not have textures
 */
class Air : public Block
{
public:
    Air() : Block(false) {}
    ~Air() = default;
    virtual const QImage *acquireModel(Texture::Ptr texture) const override;
    BlockType get() const override;
    bool checkCanPass() override;
};
