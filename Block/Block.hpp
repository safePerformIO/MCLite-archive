/**
 * @file Block.hpp
 * @author H14
 * @brief This file defines the Block Class, which will be a abstract base class. it consists of common block related datamembers memberfunction for all types of blocks.
 *        This Class will be inherited by different classes of specefic type of blocks.
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include "../Texture/IHasModel.hpp"

/* harvest for blocks */
enum class HarvestLevel {
    Unbreakable,    // Unbreakable block: bedrock
    Hand,           // Most blocks
    Pickaxe,        // Stone, Ore Mine etc
    Shovel,         // dirt
    Axe,            // log
    Sword           // leaves
};

/* base class of the blocks */
class Block: virtual public IHasModel {
public:
    static constexpr int32_t SIZE = 32;
    static constexpr QRect RECT = QRect(0, 0, 32, 32);
    // Every block is uniquely owned by the chunk 
    using Ptr = std::unique_ptr<Block>;

protected:
    const std::string textureId;                        // texture id/name for the block
    const double maxDurability = 64.0;                  // max durability of the block
    HarvestLevel harvestLevel = HarvestLevel::Hand;     // harvest level of the block
    double resistance = 1.0;                            // breaking speed
    double durability = 64.0;                           // "HP" of the block
    const bool tangible = true;                         // if the block has a hit box

public:
    enum class BlockType
    {
        EMPTY = -1,
        AIR = 0,
        BEDROCK = 1,
        COALORE = 2,
        COBBLESTONE = 3,
        DIRT = 4,
        GRASS = 5,
        STONE = 6,
        LOG = 7,
        LEAVES = 8,
        GLASS = 9,
        PLANK = 10
    };

    Block() = default;
    explicit Block(std::string id): textureId(id) {}
    Block(std::string id, double md): textureId(id), maxDurability(md) {}
    Block(bool tng): tangible(tng) {}   // constructor used by Air
    virtual ~Block() = default;

    // return current durability
    virtual double getCurrentDurability() const;
    // whether the block is destoryed
    virtual bool isDestoryed() const;
    // if block is tangible
    virtual bool isTangible() const;
    // update block state (durability) according to what tool the player is using
    virtual void updateState(HarvestLevel h , const int &dt);
    // check whether the block can be directly passed (it will not stop the player)
    virtual bool checkCanPass() = 0;

public:
    // returns the pointer to the Qimage which will be used to display the images of different blocks on screen
    virtual const QImage* acquireModel(Texture::Ptr texture) const override;
    // returns the type of this block
    virtual BlockType get() const = 0;
    // returns the durability of this block
    virtual int getHp() const;
};
