#pragma once

#include <unordered_map>

enum BlockType {
    Air,
    Dirt,
    Grass,
    Stone,
    Log,
    Plank,
    Cobblestone
};

struct BlockTexture {
    int side;
    int top;
    int bottom;
};

