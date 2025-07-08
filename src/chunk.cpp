#include "chunk.h"
#include "world.h"
#include "FastNoiseLite.h"

const float faceVertices[6][6][3] = {
    // Back face (z-)
    { {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 0, 0}, {1, 1, 0}, {0, 1, 0} },
    // Front face (z+)
    { {1, 0, 1}, {0, 0, 1}, {0, 1, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1} },
    // Left face (x-)
    { {0, 0, 1}, {0, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1} },
    // Right face (x+)
    { {1, 0, 0}, {1, 0, 1}, {1, 1, 1}, {1, 0, 0}, {1, 1, 1}, {1, 1, 0} },
    // Top face (y+)
    { {0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 0}, {1, 1, 1}, {0, 1, 1} },
    // Bottom face (y-)
    { {0, 0, 1}, {1, 0, 1}, {1, 0, 0}, {0, 0, 1}, {1, 0, 0}, {0, 0, 0} }
};

const glm::ivec3 faceNormals[6] = {
    {  0,  0, -1 }, // Back
    {  0,  0,  1 }, // Front
    { -1,  0,  0 }, // Left
    {  1,  0,  0 }, // Right
    {  0,  1,  0 }, // Top
    {  0, -1,  0 }  // Bottom
};

const float baseUVs[6][2] = {
    {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f},
    {0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
};

const int ATLAS_SIZE = 4;

const float tileSize = 1.0f / static_cast<float>(4);

Chunk::Chunk(World* world, int x, int z) {
    vao = 0;
    vbo = 0;
    chunkPos = glm::ivec2(x, z);
    initializeChunk();

    worldRef = world;
    model = glm::translate(glm::mat4(1.f), glm::vec3(x * CHUNK_WIDTH, 0, z * CHUNK_DEPTH));
    dirty = true;
}


std::vector<float> Chunk::buildMesh() {
    std::vector<float> vertices;
    std::unordered_map<BlockType, BlockTexture> blockTextures = {
        { Dirt,  { 0, 0, 0 } },
        { Grass, { 2, 1, 0 } },
        { Stone, { 3, 3, 3 } },
    };

    for (int x = 0; x < CHUNK_WIDTH; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_DEPTH; z++) {
                BlockType block = getBlock(x, y, z);
                if (block == Air)
                    continue;
                for (int face = 0; face < 6; face++) {
                    glm::ivec3 globalPos = glm::ivec3(chunkPos.x * CHUNK_WIDTH + x, y, chunkPos.y * CHUNK_DEPTH + z);
                    glm::ivec3 neighborPos = globalPos + faceNormals[face];
                    BlockType neighborBlock = worldRef->getWorldBlock(neighborPos.x, neighborPos.y, neighborPos.z);

                    if (neighborBlock != Air)
                        continue;


                    int textureIndex;
                    switch (face) {
                        case 4: textureIndex = blockTextures[block].top;    break;
                        case 5: textureIndex = blockTextures[block].bottom; break;
                        default: textureIndex = blockTextures[block].side;  break;
                    }

                    int tileX = textureIndex % 4;
                    int tileY = textureIndex / 4;
                    float uOffset = tileX * tileSize;
                    float vOffset = tileY * tileSize;


                    for (int i = 0; i < 6; i++) {
                        float vx = faceVertices[face][i][0] + x;
                        float vy = faceVertices[face][i][1] + y;
                        float vz = faceVertices[face][i][2] + z;

                        float u = uOffset + baseUVs[i][0] * tileSize;
                        float v = vOffset + baseUVs[i][1] * tileSize;

                        vertices.push_back(vx);
                        vertices.push_back(vy);
                        vertices.push_back(vz);
                        vertices.push_back(u);
                        vertices.push_back(v);

                    }
                }
            }
        }
    }
    return vertices;
}

void Chunk::buildAndUploadMesh() {
    vertices = buildMesh();
    if (vertices.empty()) return;
    if (vao == 0)
        glGenVertexArrays(1, &vao);
    if (vbo == 0)
        glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    dirty = false;
}

void Chunk::initializeChunk() {

    for (int x = 0; x < CHUNK_WIDTH; x++) {
        for (int z = 0; z < CHUNK_DEPTH; z++) {
            for (int y = 0; y < CHUNK_HEIGHT; y++) {
                blocks[x][y][z] = Grass;
            }
        }
    }
}

bool Chunk::isInBounds(int x, int y, int z) {
    if (x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_DEPTH)
        return false;
    return true;
}

BlockType Chunk::getBlock(int x, int y, int z) {
    if (!isInBounds(x, y, z))
        return Air;
    return blocks[x][y][z];
}

void Chunk::setBlock(int x, int y, int z, BlockType type) {
    if (!isInBounds(x, y, z))
        return;
    blocks[x][y][z] = type;
    dirty = true;
}

void Chunk::setDirty(bool value) {
    dirty = value;
}

bool Chunk::isDirty() {
    return dirty;
}

void Chunk::draw(Shader& shader) {
    shader.use();
    shader.set_mat4("model", model);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size()/5);
    glBindVertexArray(0);
}

Chunk::~Chunk() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
