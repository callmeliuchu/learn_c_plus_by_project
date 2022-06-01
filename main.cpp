#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cassert>
#include "map.h"
#include "utils.h"
#include "framebuffer.h"
#include "textures.h"
#include "player.h"
#include "spirte.h"


int wall_x_texcoord(const float x,const  float y,Textures &tex_walls){
    float hitx = x - floor(x+.5); // hitx and hity contain (signed) fractional parts of x and y,
    float hity = y - floor(y+.5); // they vary between -0.5 and +0.5, and one of them is supposed to be very close to 0
    int tex = hitx*tex_walls.size;
    if (std::abs(hity)>std::abs(hitx)) // we need to determine whether we hit a "vertical" or a "horizontal" wall (w.r.t the map)
        tex = hity*tex_walls.size;
    if (tex<0) // do not forget x_texcoord can be negative, fix that
        tex += tex_walls.size;
    assert(tex>=0 && tex<(int)tex_walls.size);
    return tex;
}

void map_show_sprite(Sprite &sprite,FrameBuffer &fb,Map &map){
    const size_t rect_w = fb.w / (map.w * 2);
    const size_t rect_h = fb.h / map.h;
    fb.draw_rectangle(sprite.x*rect_w-3,sprite.y*rect_h-3,6,6,pack_color(255,0,0));
}


void draw_sprite(Sprite &sprite,std::vector<float> &depth_buffer,FrameBuffer &fb,Player &player,Textures &tex_sprites){
    float sprite_dir = atan2(sprite.y-player.y,sprite.x-player.x);
    while(sprite_dir-player.a > M_PI){
        sprite_dir -= 2 * M_PI;
    }
    while(sprite_dir - player.a < -M_PI){
        sprite_dir += 2 * M_PI;
    }
    float sprite_dist = std::sqrt(pow(player.x-sprite.x,2)+pow(player.y-sprite.y,2));
    size_t sprite_screen_size = std::min(1000, static_cast<int>(fb.h/sprite_dist));
    int h_offset = (sprite_dir - player.a)/player.fov*(fb.w/2) + (fb.w/2)/2 - tex_sprites.size/2;
    int v_offset = fb.h / 2 - sprite_screen_size/ 2;
    for(size_t i=0;i<sprite_screen_size;i++){
        if(h_offset+i<0 || h_offset+i>=fb.w/2){
            continue;
        }
        if(depth_buffer[h_offset+i]<sprite_dist)continue;
        for(size_t j=0;j<sprite_screen_size;j++){
            if(v_offset+j<0 || v_offset+j>=fb.h){
                continue;
            }
//            fb.set_pixel(fb.w/2+h_offset+i,v_offset+j,pack_color(0,0,0));
            if(v_offset+int(j)<0 || v_offset+j>=fb.h){
                continue;
            }
            uint32_t color = tex_sprites.get(i*tex_sprites.size/sprite_screen_size,
                    j*tex_sprites.size/sprite_screen_size,sprite.tex_id);
            uint8_t r,g,b,a;
            unpack_color(color,r,g,b,a);
            if(a>128){
                fb.set_pixel(fb.w/2+h_offset+i,v_offset+j,color);
            }
        }

    }
}






void render(FrameBuffer &fb,Map &map,Player &player,std::vector<Sprite>&sprites, Textures &tex_walls,Textures &text_monst){
    fb.clear(pack_color(255,255,255));
    const size_t rect_w = fb.w / (map.w*2);
    const size_t rect_h = fb.h / map.h;
    std::cout<< map.h << std::endl;
    std::cout<< map.w << std::endl;
    for(size_t j=0;j<map.h;j++){
        for(size_t i=0;i<map.w;i++){
            if(map.is_empty(i,j)){
                continue;
            }
            size_t rect_x = i * rect_w;
            size_t rect_y = j * rect_h;
            size_t texid = map.get(i,j);
            assert(texid<tex_walls.count);
            fb.draw_rectangle(rect_x,rect_y,rect_w,rect_h,tex_walls.get(0,0,texid));
        }
    }
    std::cout<< fb.w << std::endl;
    std::vector<float> depth_buffer(fb.w/2,1e3);
    for(size_t i=0;i<fb.w/2;i++){
        float angle = player.a - player.fov/2 + player.fov*i/float(fb.w/2);
        for(float t=0;t<20;t+=0.01){
            float x = player.x + t * cos(angle);
            float y = player.y + t * sin(angle);
            fb.set_pixel(x*rect_w,y*rect_h,pack_color(160,160,160));
            if(map.is_empty(x,y)){
                continue;
            }
            size_t texid = map.get(x,y);
            assert(texid<tex_walls.count);
            float dist = t*cos(angle-player.a);
            depth_buffer[i] = dist;
            size_t column_height = fb.h / dist;
            int x_texcoord = wall_x_texcoord(x,y,tex_walls);
            std::vector<uint32_t> column = tex_walls.get_scaled_column(texid,x_texcoord,column_height);
            int pix_x = i + fb.w / 2;
            for(size_t j=0;j<column_height;j++){
                int pix_y = j + fb.h / 2 - column_height/2;
                if(pix_y>=0 && pix_y<(int)fb.h){
                    fb.set_pixel(pix_x,pix_y,column[j]);
                }
            }
            break;
        }
    }
    for(size_t i=0;i<sprites.size();i++){
        map_show_sprite(sprites[i],fb,map);
        draw_sprite(sprites[i],depth_buffer,fb,player,text_monst);
    }
}





using namespace std;

int main(){
    FrameBuffer fb{1024,512,std::vector<uint32_t>(1024*512,pack_color(255,255,255))};
    Player player{3.456,2.345,1.523,M_PI/3.};
    Map map;
    Textures tex_walls("../walltext.png");
    if(!tex_walls.count){
        std::cerr<<"Failed to load wall textures" << std::endl;
        return -1;
    }
    Textures tex_monst("../monsters.png");
    if(!tex_walls.count || !tex_monst.count){
        std::cerr << "Failed to load textures" <<  std::endl;
        return -1;
    }
    std::vector<Sprite> sprites {{1.834,8.765,0},{5.323,5,1}};

    std::cout << "begin" << std::endl;
    for(size_t frame=0;frame<1;frame++){
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(5) << frame << ".ppm";
        player.a += 2*M_PI/360;
        render(fb,map,player,sprites,tex_walls,tex_monst);
        drop_ppm_image(ss.str(),fb.img,fb.w,fb.h);
    }
}