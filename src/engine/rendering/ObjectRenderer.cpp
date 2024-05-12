#include "ObjectRenderer.hpp"

void ObjectRenderer::renderObject(SimulationObject *obj, RenderList &renderList)
{

  RenderInstruction instruction;
  auto geometry = ressources->tryGetGeometry(obj->modelPath);

  instruction.geometry = geometry;

  //* ================= Texturing ====================

    for (int i = 0; i < obj->texPathVec.size(); ++i)
    {
        std::string texPath = obj->texPathVec[i]; // Texture's String ID
        auto texturePtr = ressources->tryGetTex(texPath);

        if (texturePtr == nullptr)
        {
            texturePtr = ressources->loadTex(texPath, texPath, false);
            if (texturePtr == nullptr)
            {
                std::cerr << "Failed to load texture: " << texPath << std::endl;
                continue;
            }
            
            //Loading of Tex Unit:

              std::string uniformName = "tex" + std::to_string(i); // e.g., "tex0", "tex1", etc.
              texturePtr->texUnit(gameShader, uniformName.c_str(), i); // ("tex0" , 0 ) , ("tex1", 1) etc.
        }

        instruction.texVec.push_back(texturePtr);
    }

  //* ================= Build Rest of Instruction ====================

  glm::vec3 scaleFactors = glm::vec3(1.0f, 1.0f, 1.0f); // Example scale factors, adjust as necessary

  //instruction.modelMatrix = glm::translate(glm::mat4(1.0f), obj->getPosition()) * glm::mat4_cast(obj->getRotation()) // Convert quaternion to rotation matrix * glm::scale(glm::mat4(1.0f), scaleFactors);

  instruction.modelMatrix = obj->objModelMatrix;

  renderList.push_back(instruction);
}

void ObjectRenderer::renderVehicle(VehicleObject *vehicleObj, RenderList &renderList)
{

  vehicleObj->UpdateModelMatrix(); //Update all transforms before rendering (Body and wheels)

  RenderInstruction instruction;
  
  auto vehicleGeometry = ressources->tryGetGeometry(vehicleObj->modelPath);

  auto wheelGeometry = ressources->tryGetGeometry(vehicleObj->wheelObjPath);

  //* ================= Texturing ====================

  std::string texID = vehicleObj->texPathVec[0]; // Texture's String ID

  auto texturePtr = ressources->tryGetTex(texID);

   if (texturePtr == nullptr)
    {
        texturePtr = ressources->loadTex(texID, texID, true);
        if (texturePtr == nullptr)
        {
            std::cerr << "Failed to load vehicle texture: " << texID << std::endl;
            return; // Exit the function if no texture could be loaded
        }
        texturePtr->texUnit(gameShader, "tex0", 0);
    }
  
  //* ================= Build Instructions ====================

  //!Assumption: Vehicle body has single texture
  instruction.texVec.push_back(texturePtr);

  instruction.geometry = vehicleGeometry;

  instruction.modelMatrix = vehicleObj->objModelMatrix;

  renderList.push_back(instruction);

  //* ================= Wheel Instructions ====================

  for (glm::mat4 wheelMatrix : vehicleObj->wheelMatrices)
  {
    RenderInstruction wheelInstruction;
    
    wheelInstruction.texVec.push_back(texturePtr); //!Same Assumption as above

    wheelInstruction.geometry = wheelGeometry;
    wheelInstruction.modelMatrix = wheelMatrix;

    renderList.push_back(wheelInstruction);
  }
}

void ObjectRenderer::addToBuildlist(SimulationObject *simObj, RenderList &renderList)
{

  switch (simObj->type)
  {
  case SimulationObject::Vehicle:
    renderVehicle(static_cast<VehicleObject *>(simObj), renderList);
    break;

  default:
    renderObject(simObj, renderList);
    break;
  }
}