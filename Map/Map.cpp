#include "Map.hpp"
#include "../Block/BlockList.hpp"
#include <cmath>

/*
    example map of all Air
    use world generator to generate filled world
*/
Map::Map()
{
    internal = Internal();
    for (auto& c : internal) {
        // create chunk of all Air
        c = std::make_unique<Chunk>();
    }
    channel = nullptr;
}

Map::Map(Map&& map) noexcept
{
    internal = std::move(map.internal);
    channel = std::move(map.channel);
}

Map& Map::operator=(Map&& map) noexcept
{
    internal = std::move(map.internal);
    channel = std::move(map.channel);
    return *this;
}

void Map::setChannel(Channel* w)
{
    channel = w;
}

const Block::Ptr& Map::get(QPointF pos) const
{
    auto c = std::floor(pos.x() / Chunk::WIDTH);
    auto cx = int32_t(pos.x()) % Chunk::WIDTH;
    auto cy = int32_t(pos.y());
    return internal.at(c)->internal.at(cy)->at(cx);
}

Block::BlockType Map::get(const int &x, const int &y) const{
    if (inBound(QPointF(x, y)))
        return this->get(QPointF(x, y))->get();
    else
        return Block::BlockType::EMPTY;
}

void Map::updateBlock(QPoint pos, Block::Ptr block)
{
    if (inBound(pos)) {
        auto c = std::floor(pos.x() / Chunk::WIDTH);
        auto cx = pos.x() % Chunk::WIDTH;
        auto cy = pos.y();
        internal.at(c)->internal.at(cy)->at(cx) = std::move(block);
    }
}

void Map::updateBlock(QPoint pos)
{
    if (inBound(pos)) {
        auto c = std::floor(pos.x() / Chunk::WIDTH);
        auto cx = pos.x() % Chunk::WIDTH;
        auto cy = pos.y();
        auto& b = internal.at(c)->internal.at(cy)->at(cx);
        if (b->isDestoryed())
            internal.at(c)->internal.at(cy)->at(cx) = std::make_unique<Air>();
    }
}

bool Map::inBound(QPointF p) const
{
    return p.x() >= 0 && p.x() < MAXWIDTH && p.y() >= 0 && p.y() < Chunk::HEIGHT;
}

// example of a chunk with all air
Chunk::Chunk()
{
    // initialize the array
    internal = std::array<Layer, HEIGHT>();
    // initialize each layer
    for (auto& layer : internal) {
        // make new layer
        layer = std::make_unique<std::array<Block::Ptr, WIDTH>>();
        // initialize the layer
        for (auto& block : *layer) {
            block = std::make_unique<Air>();
        }
    }
}

Chunk::Chunk(Chunk&& chunk) noexcept
{
    internal = std::move(chunk.internal);
}

Chunk& Chunk::operator=(Chunk&& chunk) noexcept
{
    internal = std::move(chunk.internal);
    return *this;
}

QPoint Map::firstAir() const {
    for (int c = 60; c < 128; c++) {
        for (int l = 0; l < Chunk::HEIGHT; l++) {
            for (int b = 0; b < Chunk::WIDTH; b++) {
                if (internal.at(c)->internal.at(l)->at(b)->get() == Block::BlockType::AIR) {
                    return QPoint(c*Chunk::WIDTH + b ,l);
                }
            }
        }
    }
    return QPoint(0,0);
}

bool Map::blockNear(QPointF pos) const {
    if (inBound(pos + QPoint(1, 0)))
        if (get(pos+ QPoint(1, 0))->get() != Block::BlockType::AIR)
            return true;
    if (inBound(pos + QPoint(-1, 0)))
        if (get(pos + QPoint(-1, 0))->get() != Block::BlockType::AIR)
            return true;
    if (inBound(pos + QPoint(0, -1)))
        if (get(pos+ QPoint(0, -1))->get() != Block::BlockType::AIR)
            return true;
    if (inBound(pos + QPoint(0, 1)))
        if (get(pos+ QPoint(0, 1))->get() != Block::BlockType::AIR)
            return true;
    return false;
}

std::stringstream Map::store() const {
    std::stringstream fmt;

    for (auto &c : internal) {
        for (auto &l : c->internal) {
            for (auto &b : *l) {
                fmt << static_cast<int>(b->get()) << " ";
            }
        }
        fmt << std::endl;
    }
    return fmt;
}

void Map::load(std::stringstream& input) {
    for (auto &c : internal) {
        for (auto &l : c->internal) {
            for (auto &b : *l) {
                int i;
                input >> i;
                b = newBlock(static_cast<Block::BlockType>(i));
            }
        }
    }
}
