#pragma once

#include "block.h"
#include "shader.h"
#include <vector>

class World;

constexpr int CHUNK_WIDTH = 16;
constexpr int CHUNK_HEIGHT = 64;
constexpr int CHUNK_DEPTH = 16;

class Chunk {
    BlockType blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
    GLuint vbo, vao;
    std::vector<float> vertices;
    glm::ivec2 chunkPos;
    glm::mat4 model;
    bool dirty;
    World* worldRef;
    public:
        Chunk(World* world, int x, int z);
        BlockType getBlock(int x, int y, int z);
        bool isInBounds(int x, int y, int z);
        void setBlock(int x, int y, int z, BlockType type);
        void initializeChunk();
        void buildAndUploadMesh();
        bool isDirty();
        void setDirty(bool value);
        std::vector<float> buildMesh();
        void draw(Shader &shader);
        ~Chunk();
};
