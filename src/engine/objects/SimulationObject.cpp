#include "SimulationObject.hpp"

SimulationObject::SimulationObject(std::string objPath, std::string texturePath)
{
    // Initialize texPathVec, and add single elm
    texPathVec.push_back(texturePath);
    modelPath = objPath;

}

// Overloaded constructor for Multi-texture support
SimulationObject::SimulationObject(std::string objPath, std::vector<std::string> texturePathVec)
{
    texPathVec = texturePathVec;
    modelPath = objPath;

}

