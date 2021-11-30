/**
 * @file IHasModel.hpp
 * @author H14
 * @brief  model interface
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <QImage>
#include "Texture.hpp"

/**
 * @brief Interfacce for classes with models
 * 
 */
class IHasModel {
public:
    virtual ~IHasModel() = default;
    /**
     * @brief  return the model
     * Alert: return value is nullable !
     * 
     * @param texture         pointer to texture manager 
     * @return const QImage*  pointer to image
     */
    virtual const QImage* acquireModel(Texture::Ptr texture) const = 0;
};
