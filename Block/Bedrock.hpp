#pragma once

#include "Block.hpp"

class Bedrock : public Block
{
public:
    Bedrock();
    ~Bedrock() = default;

public:
    virtual const QImage *acquireModel(Texture::Ptr texture) const override;
    BlockType get() const override;
    bool checkCanPass() override;
};
