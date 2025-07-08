#include "world.h"
#include "chunk.h"

World::World() {
    lastPlayerChunk = glm::ivec2(INT_MIN);
    setRenderDistance(3);
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

void World::markChunkDirty(int x, int z) {
    glm::ivec2 neighborCoords = getChunkCoords(x, z);
    auto it = chunks.find(neighborCoords);
    if (it != chunks.end()) {
        it->second->setDirty(true);
    }
}

void World::setBlock(int x, int y, int z, BlockType type) {
    glm::ivec2 chunkCoords = getChunkCoords(x, z);
    auto it = chunks.find(chunkCoords);
    if (it == chunks.end()) return;

    int localX = x % CHUNK_WIDTH;
    int localZ = z % CHUNK_DEPTH;
    if (localX < 0) localX += CHUNK_WIDTH;
    if (localZ < 0) localZ += CHUNK_DEPTH;

    Chunk* chunk = it->second.get();
    chunk->setBlock(localX, y, localZ, type);

    if (localX == 0)
        markChunkDirty(x - 1, z);
    else if (localX == CHUNK_WIDTH - 1)
        markChunkDirty(x + 1, z);

    if (localZ == 0) 
        markChunkDirty(x, z - 1);
    else if (localZ == CHUNK_DEPTH - 1)
        markChunkDirty(x, z + 1);
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
                glm::ivec2 surrouding = pos + glm::ivec2(0, 1);
                auto neighborIt = chunks.find(surrouding);
                if (neighborIt != chunks.end()) { neighborIt->second->setDirty(true); }
                surrouding = pos + glm::ivec2(0, -1);
                neighborIt = chunks.find(surrouding);
                if (neighborIt != chunks.end()) { neighborIt->second->setDirty(true); }
                surrouding = pos + glm::ivec2(1, 0);
                neighborIt = chunks.find(surrouding);
                if (neighborIt != chunks.end()) { neighborIt->second->setDirty(true); }
                surrouding = pos + glm::ivec2(-1, 0);
                neighborIt = chunks.find(surrouding);
                if (neighborIt != chunks.end()) { neighborIt->second->setDirty(true); }
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
        chunk->draw(shader);
    }
}

void World::setPlayerPos(glm::vec3 pos) {
    this->playerPos = pos;
    glm::ivec2 currentChunk = getChunkCoords(pos.x, pos.z);

    if (currentChunk != lastPlayerChunk) {
        updateChunksAroundPlayer();
        lastPlayerChunk = currentChunk;
    }
}

void World::updateChunks() {
    int built = 0;
    const int maxChunksPerFrame = 1;

    for (auto& [pos, chunk] : chunks) {
        if (chunk->isDirty()) {
            chunk->buildAndUploadMesh();
            built++;
            if (built >= maxChunksPerFrame)
                break;
        }
    }
}

bool World::raycastBlock(glm::vec3 origin, glm::vec3 direction, int maxSteps, glm::ivec3& outBlock, glm::ivec3& outPrevBlock) {
    glm::vec3 pos = glm::floor(origin);
    glm::ivec3 step = glm::sign(direction);
    glm::vec3 tMax;
    glm::vec3 tDelta;

    glm::vec3 invDir = 1.0f / direction;

    for (int i = 0; i < 3; ++i) {
        float rayOrigAxis = origin[i] - pos[i];
        tMax[i] = (step[i] > 0 ? (1.0f - rayOrigAxis) : rayOrigAxis) * std::abs(invDir[i]);
        tDelta[i] = std::abs(invDir[i]);
    }

    for (int i = 0; i < maxSteps; ++i) {
        BlockType block = getWorldBlock((int)pos.x, (int)pos.y, (int)pos.z);
        if (block != Air) {
            outBlock = glm::ivec3(pos);
            return true;
        }

        outPrevBlock = glm::ivec3(pos);

        if (tMax.x < tMax.y && tMax.x < tMax.z) {
            pos.x += step.x;
            tMax.x += tDelta.x;
        } else if (tMax.y < tMax.z) {
            pos.y += step.y;
            tMax.y += tDelta.y;
        } else {
            pos.z += step.z;
            tMax.z += tDelta.z;
        }
    }

    return false;
}

BlockType World::getWorldBlock(int x, int y, int z) {
    if (y < 0 || y >= CHUNK_HEIGHT) return Air;
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
