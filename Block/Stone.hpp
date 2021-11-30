#pragma once

#include "Block.hpp"

class Stone : public Block
{
public:
    Stone();
    ~Stone() = default;

public:
    virtual const QImage *acquireModel(Texture::Ptr texture) const override;
    BlockType get() const override;
    bool checkCanPass() override;
};
