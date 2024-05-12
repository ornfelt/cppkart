#include "SimulationWorld.hpp"

SimulationWorld::SimulationWorld() {

    physicsWorld = PhysicsWorldSingleton::getInstance();

    //* Beta Testing 

    simObj = std::make_shared<VehicleObject>("../src/ressources/volga/volga.obj", "../src/ressources/first_car_wheel.obj", "../src/ressources/volga/volga.png");

    simObj->position = glm::vec3(50.0f, 0.0f, 0.0f);
    simObj->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    
    entities.push_back(simObj);

    auto simObj2 = std::make_shared<SimulationObject>("../src/ressources/DE_Map1/Landscape01.obj", "../src/ressources/DE_Map1/Map01_Albedo.png");

    //std::vector<LoadedChunk> chunks = ChunkedMapLoader::loadChunks("../src/ressources/chunk_map.txt");

    simObj2->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    // Set model matrix to 10x
    simObj2->objModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(40.0f, 40.0f, 40.0f));

    entities.push_back(simObj2);

    // **** MULTI TEXTURE TESTING ****

    std::vector<std::string> aztec_map_texs = {
        "../src/ressources/DE_Aztec/c_mossy_rocks.jpg",
        "../src/ressources/DE_Aztec/c_concrete_wall.jpg"
    };

    auto simObj3 = std::make_shared<SimulationObject>("../src/ressources/DE_Aztec/DE_AZTEC_MAP.obj", aztec_map_texs);

    simObj3->objModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    entities.push_back(simObj3);

}

void SimulationWorld::updateVehicleControls(GameInputState::Control accelerationControl, GameInputState::Control turnControl) {
    // Handle acceleration or braking based on the accelerationControl parameter
    switch (accelerationControl) {
        case GameInputState::VehicleAccelerate:
            simObj.get()->vehicle.ApplyEngineForce(2000);
            break;
        case GameInputState::VehicleBrake:
            simObj.get()->vehicle.ApplyEngineForce(-2500);
            break;
        default: // Covers GameInputState::Null and any other unspecified cases
            simObj.get()->vehicle.ApplyEngineForce(0);
            break;
    }

    // Handle turning based on the turnControl parameter
    switch (turnControl) {
        case GameInputState::VehicleTurnLeft:
            simObj.get()->vehicle.ApplySteer(0.13);
            break;
        case GameInputState::VehicleTurnRight:
            simObj.get()->vehicle.ApplySteer(-0.13);
            break;
        default: // Covers GameInputState::Null and any other unspecified cases
            simObj.get()->vehicle.ApplySteer(0);
            break;
    }
}