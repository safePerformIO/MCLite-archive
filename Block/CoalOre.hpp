#pragma once

#include "Block.hpp"

class CoalOre : public Block
{
public:
    CoalOre();
    ~CoalOre() = default;

public:
    virtual const QImage *acquireModel(Texture::Ptr texture) const override;
    BlockType get() const override;
    bool checkCanPass() override;
};
