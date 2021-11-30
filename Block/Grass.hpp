#pragma once

#include "Block.hpp"

class Grass : public Block
{
public:
    Grass();
    ~Grass() = default;

public:
    virtual const QImage *acquireModel(Texture::Ptr texture) const override;
    BlockType get() const override;
    bool checkCanPass() override;
};
