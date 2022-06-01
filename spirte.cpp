//
// Created by 刘楚 on 5/31/22.
//

#include "spirte.h"
#include <cstdlib>

bool Sprite::operator < (const Sprite& s) const{
    return player_dist > s.player_dist;
}