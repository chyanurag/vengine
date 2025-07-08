#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <memory>
#include "shader.h"
#include "block.h"


class Chunk;

struct ChunkTask {
    glm::ivec2 pos;
};

namespace std {
    template<>
    struct hash<glm::ivec2> {
        std::size_t operator()(const glm::ivec2& k) const {
            return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1);
        }
    };
}

class World {
    std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>> chunks;
    unsigned int renderDistance;
    glm::vec3 playerPos;
    glm::ivec2 lastPlayerChunk;
    public:
        World();
        glm::ivec2 getChunkCoords(int x, int z);
        BlockType getWorldBlock(int x, int y, int z);
        void setBlock(int x, int y, int z, BlockType type);
        void drawChunks(Shader& shade);
        void updateChunksAroundPlayer();
        void setRenderDistance(unsigned int dist);
        void setPlayerPos(glm::vec3 playerPos);
        void updateChunks();
        bool raycastBlock(glm::vec3 origin, glm::vec3 direction, int maxSteps, glm::ivec3& outBlock, glm::ivec3& outPrevBlock);
        void markChunkDirty(int x, int z);
        ~World();
};
