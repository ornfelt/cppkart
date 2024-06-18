#ifndef CHUNKEDMAPLOADER_CLASS_HPP
#define CHUNKEDMAPLOADER_CLASS_HPP

#include <vector>
#include <sstream>  // Include for std::istringstream
#include <iostream>
#include <fstream>
#include <string>

// Math & OpenGL specific includes
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "jx_engine/logs/Logger.hpp"

struct LoadedChunk {

    glm::vec3 centre_point;
    std::vector<glm::vec3> triangle_ordered_verts;

    float X_origin;
    float Z_origin;
};

class PhysChunkedMapLoader {
    public:
        static std::vector<LoadedChunk> loadChunks(const std::string& filename);

    private:
        static glm::vec3 parseVec3(const std::string& line);
        static std::vector<glm::vec3> parseVec3List(const std::string& line);
};

#endif