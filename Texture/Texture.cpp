#include "Texture.hpp"

Texture::Texture()
{
    textures = std::unordered_map<std::string, QImage>();
    textures["stone"] = QImage(":/blocks/res/stone.png");
    textures["bedrock"] = QImage(":/blocks/res/bedrock.png");
    textures["coalore"] = QImage(":/blocks/res/coal_ore.png");
    textures["cobblestone"] = QImage(":/blocks/res/cobblestone.png");
    textures["grass"] = QImage(":/blocks/res/grass_block_side.png");
    textures["dirt"] = QImage(":/blocks/res/dirt.png");
    textures["log"] = QImage(":/blocks/res/oak_log.png");
    textures["leaves"] = QImage(":/blocks/res/oak_leaves.png");
    textures["glass"] = QImage(":/blocks/res/glass.png");
    textures["plank"] = QImage(":/blocks/res/oak_planks.png");
}

const QImage* Texture::getTexture(const std::string id) const
{
    if (textures.count(id))
        return &textures.at(id);
    else
        return nullptr;
}
