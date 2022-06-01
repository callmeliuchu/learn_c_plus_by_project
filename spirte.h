//
// Created by 刘楚 on 5/31/22.
//

#ifndef MYPROJECT_SPIRTE_H
#define MYPROJECT_SPIRTE_H
#include <cstdlib>

struct Sprite {
    float x,y;
    size_t tex_id;
    float playrt_dist;
    bool operator < (const Sprite& s) const;
};


#endif //MYPROJECT_SPIRTE_H
