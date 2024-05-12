#include "GameRenderer.hpp"

#define BULLET_DEBUG_DRAW 1

const std::string SHADER_PATH = "../src/game/shader/";

GameRenderer::GameRenderer(int winWidth, int winHeight, Camera *cam, SimulationWorld *worldArg)
    : camera(cam), world(worldArg)
{ // world is now a raw pointer

  glViewport(0, 0, winWidth, winHeight); // Set viewport size

  // Initialize the camera's look-at vector
  glm::vec3 initCamLookAt = glm::vec3(1, 1, 1);
  camera->LookAt = initCamLookAt;

  // Initialize shaders
  std::string vertexShaderPath = SHADER_PATH + "default.vert";
  std::string fragmentShaderPath = SHADER_PATH + "default.frag";
  mainShader = std::make_shared<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

  // Get locations of uniforms in the shader
  modelMatrixLOC = glGetUniformLocation(mainShader.get()->ID, "modelMatrix");
  useTextureLOC = glGetUniformLocation(mainShader.get()->ID, "useTexture");

  colorUniformLocation = glGetUniformLocation(mainShader.get()->ID, "FragColor");

  // Initialize Object Renderer
  objectRender = std::make_unique<ObjectRenderer>(world, &ressources, mainShader);

  if (BULLET_DEBUG_DRAW == 1)
    debugDrawer = new BulletDebugDrawer(mainShader.get()->ID);

  world->physicsWorld->dynamicsWorld->setDebugDrawer(debugDrawer);
}

RenderRsrcManager &GameRenderer::getRessourcePtr()
{
  return ressources;
}

void GameRenderer::RenderALL(bool bulletDebugDraw)
{

  glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

  // Clean the back buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mainShader.get()->Activate();

  glUniform1i(useTextureLOC, GL_FALSE);
  // Explicitly setting the model matrix to an identity matrix
  glm::mat4 identityMatrix = glm::mat4(1.0f);
  glUniformMatrix4fv(modelMatrixLOC, 1, GL_FALSE, glm::value_ptr(identityMatrix));

  if (bulletDebugDraw)
  {
    world->physicsWorld->dynamicsWorld->debugDrawWorld();
    debugDrawer->flushLines();
  }

  glUniform1i(useTextureLOC, GL_TRUE);

  renderObjects();

  //glCullFace(GL_BACK);
  //glEnable(GL_CULL_FACE);

  camera->Matrix(45.0f, 0.1f, 1000.0f, mainShader, "camMatrix"); //! IMPORTANT
}

void GameRenderer::renderObjects()
{
  // terrainGeom.Draw(modelMatrixLocation, terrainModelMatrix, colorUniformLocation, false);

  auto drawList = createObjectRenderList();

  glUniform1i(useTextureLOC, GL_TRUE);

  for (const auto &instruction : drawList)
  {

    if (instruction.texVec.empty() || !instruction.texVec[0]) {
            std::cerr << "Texture pointer is null or vector is empty, skipping texture bind." << std::endl;
            continue; // Skip this iteration if there's no valid texture to bind
    }

	GLuint numTexUnit = glGetUniformLocation(mainShader->ID, "numTextures");
	mainShader->Activate();
	glUniform1i(numTexUnit, instruction.texVec.size());

  printf("Num Textures: %d\n", instruction.texVec.size());

    for (int i = 0; i < instruction.texVec.size(); i++) {
    
      auto tex = instruction.texVec[i];
      glActiveTexture(GL_TEXTURE0+i); // Active proper texture unit before binding
      tex.get()->Bind(); 

    }

        GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << std::hex << error << std::endl;
    }

    glm::mat4 tempMatrix = instruction.modelMatrix; // Create a non-const copy
    instruction.geometry->Draw(modelMatrixLOC, tempMatrix, colorUniformLocation, false);

    for(auto tex : instruction.texVec) { tex.get()->Unbind(); }

    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << std::hex << error << std::endl;
    }

  }
}

RenderList GameRenderer::createObjectRenderList()
{
  RenderList drawList;

  for (auto &entity : world->entities)
  { // Simulation object entities
    objectRender.get()->addToBuildlist(entity.get(), drawList);
  }

  return drawList;
}
