//
// Created by 刘楚 on 5/31/22.
//

#include "spirte.h"
#include <cstdlib>

bool Sprite::operator < (const Sprite& s) const{
    return playrt_dist > s.playrt_dist;
}