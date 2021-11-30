#include "WorldLoader.hpp"
#include "Channel.hpp"
#include <fstream>

namespace WorldLoader {
    // save current world
    void saveWorld(const Channel& channel, const std::string path) {
        // init io
        std::ofstream output{path, std::ios_base::trunc};
        // save Player
        auto& player = channel.player;
        output << player->store();
        // save Map
        auto& map = channel.map;
        output << map->store().rdbuf();
        // finish io
        output.flush();
        output.close();
    }
}
