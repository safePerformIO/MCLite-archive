#pragma once

#include "Block.hpp"

class Glass : public Block
{
public:
    Glass();
    ~Glass() = default;

public:
    virtual const QImage *acquireModel(Texture::Ptr texture) const override;
    BlockType get() const override;
    bool checkCanPass() override;
};
