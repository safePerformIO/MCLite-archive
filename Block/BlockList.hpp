/**
 * @file BlockList.hpp
 * @author H14
 * @brief This file is include only. Only include specific block here. It consists of factory functions for making new blocks
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include "Air.hpp"
#include "Stone.hpp"
#include "Bedrock.hpp"
#include "Block.hpp"
#include "CoalOre.hpp"
#include "CobbleStone.hpp"
#include "Dirt.hpp"
#include "Grass.hpp"
#include "Log.hpp"
#include "Leaves.hpp"
#include "Glass.hpp"
#include "Plank.hpp"

// factory function for making new blocks
Block::Ptr newBlock(std::string name);
// create block from block type
Block::Ptr newBlock(Block::BlockType ty);
