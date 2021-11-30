#pragma once

#include "Block.hpp"

class Plank : public Block
{
public:
    Plank();
    ~Plank() = default;

public:
    virtual const QImage *acquireModel(Texture::Ptr texture) const override;
    BlockType get() const override;
    bool checkCanPass() override;
};
