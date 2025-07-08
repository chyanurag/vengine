#include "world.h"
#include "chunk.h"

World::World() {
    setRenderDistance(2);
}

void World::setRenderDistance(unsigned int dist) {
    if (dist != renderDistance) {
        renderDistance = dist;
        updateChunksAroundPlayer();
    }
}

glm::ivec2 World::getChunkCoords(int x, int z) {
    int chunkX = x / CHUNK_WIDTH;
    int chunkZ = z / CHUNK_DEPTH;
    if (x < 0) chunkX--;
    if (z < 0) chunkZ--;
    return glm::ivec2(chunkX, chunkZ);
}

void World::updateChunksAroundPlayer() {
    glm::ivec2 centerChunk = getChunkCoords(playerPos.x, playerPos.z);
    std::unordered_set<glm::ivec2> neededChunks;

    for (int dx = -static_cast<int>(renderDistance); dx <= static_cast<int>(renderDistance); dx++) {
        for (int dz = -static_cast<int>(renderDistance); dz <= static_cast<int>(renderDistance); dz++) {
            glm::ivec2 pos = centerChunk + glm::ivec2(dx, dz);
            neededChunks.insert(pos);

            if (chunks.find(pos) == chunks.end()) {
                chunks[pos] = std::make_unique<Chunk>(this, pos.x, pos.y);
            }
        }
    }

    for (auto it = chunks.begin(); it != chunks.end(); ) {
        if (neededChunks.find(it->first) == neededChunks.end()) {
            it = chunks.erase(it);
        } else {
            ++it;
        }
    }
}

void World::drawChunks(Shader& shader) {
    for (auto& [pos, chunk] : chunks) {
        if (chunk->isDirty()) {
            chunk->buildAndUploadMesh();
        }
        chunk->draw(shader);
    }
}

void World::setPlayerPos(glm::vec3 playerPos) {
    this->playerPos = playerPos;
}

void World::updateChunks() {
    int built = 0;
    const int maxChunksPerFrame = 2;

    for (auto& [pos, chunk] : chunks) {
        if (chunk->isDirty()) {
            chunk->buildAndUploadMesh();
            built++;
            if (built >= maxChunksPerFrame)
                break;
        }
    }
}

BlockType World::getWorldBlock(int x, int y, int z) {
    glm::ivec2 chunkPos = getChunkCoords(x, z);
    auto it = chunks.find(chunkPos);
    if (it == chunks.end()) {
        return Air;
    }

    Chunk* chunk = it->second.get();
    int localX = x % CHUNK_WIDTH;
    int localZ = z % CHUNK_DEPTH;
    if (localX < 0) localX += CHUNK_WIDTH;
    if (localZ < 0) localZ += CHUNK_DEPTH;
    return chunk->getBlock(localX, y, localZ);
}

World::~World() {

}
