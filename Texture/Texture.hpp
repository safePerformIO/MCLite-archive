/**
 * @file Texture.hpp
 * @author H14
 * @brief  texture manager
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <unordered_map>
#include <QImage>
#include <memory>

/* stores all the textures */
class Texture {
public:
    // texture should have multiple ownership
    using Ptr = std::shared_ptr<Texture>;
    
private:
    // stored in hashmap, indexed by "name"
    std::unordered_map<std::string, QImage> textures;

public:
    Texture();
    ~Texture() = default;
    /**
     * @brief Get the Texture object
     * 
     * @return const QImage*    pointer to image
     *                          Alert: ! nullable
     */
    const QImage* getTexture(const std::string) const;
};
