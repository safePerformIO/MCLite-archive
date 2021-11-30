/**
 * @file WorldLoader.hpp
 * @author H14
 * @brief helper functions to save the world to file
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "../Channel.hpp"

namespace WorldLoader {
    /**
     * @brief save the current world to a game save
     * 
     * @param channel   used to acccess player and map
     * @param path      path to the game save
     */
    void saveWorld(const Channel& channel, const std::string path);
}
