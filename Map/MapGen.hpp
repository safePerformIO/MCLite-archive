/**
 * @file MapGen.hpp
 * @author group H14
 * @brief
 * @version 0.1
 * @date 2021-11-29
 *
 * @copyright Copyright (c) 2021
 *
 */
#pragma once

#include "Map.hpp"
#include <list>

// generate map
class MapGen
{
private:
    const int seed; // seed random world generation
    unsigned int count = 0;
    int height = 0;
    std::list<Chunk::Ptr> chunkStream; // list holding the generated chunk
public:
    MapGen(int s) : seed(s) {}
    ~MapGen() = default;
    /**
     * @brief randomly generate the map
     *        (in terms of the horizontal distribution)
     *
     * @return Map
     */
    Map generate();

private:
    enum TerrType
    {
        CLIF,
        HILL,
        GROUND
    };

    /**
     * @brief general terrain generator, useful for hill, clif, and ground generation
     *
     * @param modNumber determines how fast the terrain variates vertially
     * @param ascendFromL2R determines the direction of altitude ascendence
     * @param terrType type of the generated terrain
     * @return Chunk::Ptr
     */
    Chunk::Ptr generateTerr(int modNumber, bool ascendFromL2R, TerrType terrType);
    /**
     * @brief special case terrain generator. Due to the special properties of cave, an spcefic function is written for cave generation
     *
     * @return Chunk::Ptr
     */
    Chunk::Ptr cave();
};
