#pragma once

#include <unordered_map>

enum BlockType {
    Air,
    Dirt,
    Grass,
    Stone,
    Log,
    Plank,
    Cobblestone,
    Water
};

struct BlockTexture {
    int side;
    int top;
    int bottom;
};

