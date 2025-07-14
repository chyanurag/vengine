#pragma once

#include <GLFW/glfw3.h>
#include "block.h"

class BlockSelector {
    BlockType type = Dirt;
    public:
        BlockType getCurrent();
        void update(GLFWwindow *window);
};
