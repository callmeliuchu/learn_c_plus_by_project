//
// Created by 刘楚 on 5/30/22.
//

#ifndef MYPROJECT_FRAMEBUFFER_H
#define MYPROJECT_FRAMEBUFFER_H


#include <cstdlib>
#include <vector>

struct FrameBuffer {
    size_t w,h;
    std::vector<uint32_t>img;
    void clear(const uint32_t color);
    void set_pixel(const size_t x,const size_t y,const uint32_t color);
    void draw_rectangle(const size_t x,const size_t y,const size_t w,size_t h,const uint32_t color);
};


#endif //MYPROJECT_FRAMEBUFFER_H
