#include "BlockList.hpp"

Block::Ptr newBlock(std::string name)
{
    if (name == "stone")
        return std::make_unique<Stone>();
    else if (name == "air")
        return std::make_unique<Air>();
    else if (name == "bedrock")
        return std::make_unique<Bedrock>();
    else if (name == "coalore")
        return std::make_unique<CoalOre>();
    else if (name == "cobblestone")
        return std::make_unique<CobbleStone>();
    else if (name == "dirt")
        return std::make_unique<Dirt>();
    else if (name == "grass")
        return std::make_unique<Grass>();
    else if (name == "log")
        return std::make_unique<Log>();
    else if (name == "leaves")
        return std::make_unique<Leaves>();
    else if (name == "glass")
        return std::make_unique<Glass>();
    else if (name == "plank")
        return std::make_unique<Plank>();

    return nullptr;
}

Block::Ptr newBlock(Block::BlockType ty)
{
    using T = Block::BlockType;
    switch (ty) {
    case T::AIR: return std::make_unique<Air>();
    case T::EMPTY: return std::make_unique<Air>();
    case T::BEDROCK: return std::make_unique<Bedrock>();
    case T::COALORE: return std::make_unique<CoalOre>();
    case T::COBBLESTONE: return std::make_unique <CobbleStone>();
    case T::DIRT: return std::make_unique <Dirt>();
    case T::GRASS: return std::make_unique <Grass>();
    case T::LEAVES: return std::make_unique <Leaves>();
    case T::LOG: return std::make_unique <Log>();
    case T::STONE: return std::make_unique <Stone>();
    case T::GLASS: return std::make_unique<Glass>();
    case T::PLANK: return std::make_unique<Plank>();
    }

    return std::make_unique<Plank>();
}

