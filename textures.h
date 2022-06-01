//
// Created by 刘楚 on 5/30/22.
//

#ifndef MYPROJECT_TEXTURES_H
#define MYPROJECT_TEXTURES_H



struct Textures {
    size_t img_w,img_h;
    size_t count,size;
    std::vector<uint32_t> img;
    Textures(const std::string filename, const uint32_t format);
    uint32_t get(const size_t i,const size_t h,const size_t idx) const;
    std::vector<uint32_t> get_scaled_column(const size_t texture_id,const size_t tex_coord,const size_t column_height) const;
};


#endif //MYPROJECT_TEXTURES_H
