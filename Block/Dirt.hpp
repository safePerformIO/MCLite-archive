#pragma once

#include "Block.hpp"

class Dirt : public Block
{
public:
    Dirt();
    ~Dirt() = default;

public:
    virtual const QImage *acquireModel(Texture::Ptr texture) const override;
    BlockType get() const override;
    bool checkCanPass() override;
};
