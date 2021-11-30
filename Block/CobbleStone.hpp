#pragma once

#include "Block.hpp"

class CobbleStone : public Block
{
public:
    CobbleStone();
    ~CobbleStone() = default;

public:
    virtual const QImage *acquireModel(Texture::Ptr texture) const override;
    BlockType get() const override;
    bool checkCanPass() override;
};
