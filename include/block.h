#pragma once

#include <unordered_map>

enum BlockType {
    Air,
    Dirt,
    Grass,
    Stone,
};

struct BlockTexture {
    int side;
    int top;
    int bottom;
};

