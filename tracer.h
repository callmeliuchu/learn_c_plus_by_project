//
// Created by 刘楚 on 6/1/22.
//

#ifndef TINYRAYCASTER_MAIN_H
#define TINYRAYCASTER_MAIN_H
#include <vector>

#include "map.h"
#include "player.h"
#include "spirte.h"
#include "framebuffer.h"
#include "textures.h"

struct GameState {
    Map map;
    Player player;
    std::vector<Sprite> monsters;
    Textures tex_walls;
    Textures tex_monst;
};

void render(FrameBuffer &fb, const GameState &gs);
#endif //TINYRAYCASTER_MAIN_H
