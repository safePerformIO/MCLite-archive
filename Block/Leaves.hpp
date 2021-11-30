#pragma once

#include "Block.hpp"

class Leaves : public Block
{
public:
    Leaves();
    ~Leaves() = default;

public:
    virtual const QImage *acquireModel(Texture::Ptr texture) const override;
    BlockType get() const override;
    bool checkCanPass() override;
};
