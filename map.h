//
// Created by 刘楚 on 5/30/22.
//

#ifndef MYPROJECT_MAP_H
#define MYPROJECT_MAP_H


struct Map {
    size_t w,h;
    Map();
    int get(const size_t i,const size_t j);
    bool is_empty(const size_t i,const size_t j);
};


#endif //MYPROJECT_MAP_H
