#include "chunk.h"

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

const float tileSize = 1.0f / 4.0f;

Chunk::Chunk() {
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    initializeChunk();
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
                    glm::ivec3 neighbor = glm::ivec3(x, y, z) + faceNormals[face];

                    if (isInBounds(neighbor.x, neighbor.y, neighbor.z) && getBlock(neighbor.x, neighbor.y, neighbor.z) != Air)
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

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Chunk::initializeChunk() {
    for (int x = 0; x < CHUNK_WIDTH; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_DEPTH; z++) {
                if (y < 10) {
                    blocks[x][y][z] = Stone;
                } else if (y < CHUNK_HEIGHT - 2) {
                    blocks[x][y][z] = Dirt;
                } else {
                    blocks[x][y][z] = Grass;
                }
            }
        }
    }
    buildAndUploadMesh();
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
}

void Chunk::draw(Shader& shader) {
    shader.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size()/5);
    glBindVertexArray(0);
}

Chunk::~Chunk() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
