/**
 * @file Channel.hpp
 * @author H14
 * @brief 'Channel' helps different objects transmit data with each other
 * @version 0.1
 * @date 2021-11-30
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include "Map/Map.hpp"
#include "Player.hpp"
#include "World.hpp"
#include "Texture/Texture.hpp"
#include "Scene.hpp"
#include "Osd.hpp"

/**
 * @brief channel (public) for object communication
 * 
 * use smart unique pointer to avoid memory leak
 * 
 */
struct Channel
{
    std::unique_ptr<Map> map;       // smart unique pointer to the map
    std::unique_ptr<Player> player; // smart unique pointer to the player
    std::unique_ptr<World> world;   // smart unique pointer to the world
    Texture::Ptr texture;           // texture container
    std::unique_ptr<Osd> osd;       // smart unique pointer to the osd
    Scene *scene;                   // the scene, contains every QGraphicsItem objects, and will refresh once pre frame
    int dt;                         // the time elapsed since the previous frame, used to serve Variable Frame Rate
};
