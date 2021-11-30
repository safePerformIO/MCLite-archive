/**
 * @file Map.hpp
 * @author H14
 * @brief the map, chunk class
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "../Block/Block.hpp"
#include <array>
#include <memory>
#include <sstream>

struct Channel;

/** 
    @brief  A chunk of blocks 
            this will be used by the map generator and the map
            this is simply a wrapper of the raw 2d array
*/
class Chunk {
public:
    static constexpr int HEIGHT = 128;      // chunk/world height
    static constexpr int WIDTH = 16;        // chunk width
    using Ptr = std::unique_ptr<Chunk>;     // smart pointer of chunk
    /*
        We are using layers here, so each layer represents 1 height in the chunk
        We can also go the other way
    */
    using Layer = std::unique_ptr<std::array<Block::Ptr, WIDTH>>;
public:
    // internal representation
    std::array<Layer, HEIGHT> internal;
public:
    Chunk();
    explicit Chunk(std::array<Layer, HEIGHT> i): internal(std::move(i)) {}
    ~Chunk() = default;
    Chunk(Chunk&& chunk) noexcept;
    Chunk(const Chunk& chunk) = delete;
    // Chunk(const Chunk& chunk) = delete;
    Chunk& operator=(Chunk&& chunk) noexcept;
    Chunk& operator=(const Chunk& chunk) = delete;
};


/**
 * @brief The map of the world, storing all the chunks
 * 
 */
class Map {
public:
    static constexpr int CHUNKCOUNT = 128;
    static constexpr int MAXWIDTH = Chunk::WIDTH * CHUNKCOUNT;
    using Internal = std::array<Chunk::Ptr, CHUNKCOUNT>;
private:
    // internal representation
    Internal internal;
    Channel* channel;
public:
    // empty map
    Map();
    // map from internal
    Map(Internal i, Channel* c): internal(std::move(i)), channel(c) {}

    ~Map() = default;
    Map(Map&& map) noexcept;
    Map(const Map& map) = delete;
    Map& operator=(Map&& map) noexcept;
    Map& operator=(const Map& map) = delete;

    // set channel
    void setChannel(Channel* c);
    // get block at the position, return block pointer reference
    const Block::Ptr& get(QPointF pos) const;
    // get block type, may not inbound, return block type
    Block::BlockType get(const int &x, const int &y) const;
    // replace a block with another block
    void updateBlock(QPoint pos, Block::Ptr block);
    // update durability of a blocks
    void updateBlock(QPoint pos);
    // place the player on a new map, return the position
    QPoint firstAir() const;
    // check if there are blocks nearby
    bool blockNear(QPointF pos) const;
    // check if pos is inside the map
    bool inBound(QPointF p) const;
    // store the map, return the string representation of the map
    std::stringstream store() const;
    // load the map from a string stream
    void load(std::stringstream& input);
};
