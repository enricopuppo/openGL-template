#include "DisplayManager.hpp"
#include "ModelLoader.hpp"
#include "Texture.hpp"
#include "TexturedModel.hpp"
#include "MasterRenderer.hpp"
#include "Entity.hpp"
#include "Camera.hpp"
#include "OBJLoader.hpp"
#include "ProcModelGenerator.hpp"
#include "Player.hpp"
#include "Terrain.hpp"
#include "TerrainTexture.hpp"
#include "TerrainTexturePack.hpp"
#include "WaterShader.hpp"
#include "WaterTile.hpp"
#include "WaterRenderer.hpp"
#include "WaterFrameBuffers.hpp"
#include <stdio.h>
#include "./glm/glm.hpp"

// --> global variables edited by the controls callbacks <--
float scrollDelta = 0.0;
double pointerPositionX = 0.0;
double pointerPositionY = 0.0;
float pointerDeltaX = 0.0;
float pointerDeltaY = 0.0;
bool isSpacePressed = false;
bool clicked = false;
float sunAmount = 1.0;

// --> ImGui window definition and rendering <--
void loadInterface()
{
    ImGuiWindowFlags flags = 0;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("CONTROLS", NULL, flags);
    ImGui::BulletText("W, S: move forward and backward");
    ImGui::BulletText("A, D: rotate left and right");
    ImGui::BulletText("E: emerge");
    ImGui::BulletText("Q: submerge");
    ImGui::BulletText("Space: toggle 1st/3rd person view");
    ImGui::BulletText("Mouse wheel: zoom");
    ImGui::BulletText("Mouse right + drag: rotate view");
    ImGui::BulletText("Mouse left + slider: select day/night");
    ImGui::BulletText("Esc: close the program");
    ImGui::SliderFloat("Night-Day", &sunAmount, 0.0, 1.0, "");
    ImGui::End();
}

void drawInterface()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// --> GLFW callbacks <--
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    scrollDelta = yoffset;
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if ((button == GLFW_MOUSE_BUTTON_RIGHT) && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &pointerPositionX, &pointerPositionY);
        clicked = true;
    }
}

void pointerPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        pointerDeltaX = xpos - pointerPositionX;
        pointerPositionX = xpos;
        pointerDeltaY = ypos - pointerPositionY;
        pointerPositionY = ypos;
    }
}

void buttonPressCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        isSpacePressed = true;
}

void setupGlfwCallbacks(GLFWwindow *window)
{
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, pointerPositionCallback);
    glfwSetKeyCallback(window, buttonPressCallback);
}

int main()
{
    printf("Loading...\n");
    DisplayManager dm = DisplayManager(1280, 720);
    dm.init();
    setupGlfwCallbacks(dm.getWindow());
    glfwSetCursorPos(dm.getWindow(), 0, 0);

    OBJLoader objloader = OBJLoader();
    ModelLoader loader = ModelLoader();
    ProcModelGenerator procGenerator = ProcModelGenerator(loader);
    MasterRenderer renderer = MasterRenderer("../assets/shaders/shader.vs", "../assets/shaders/shader.fs", "../assets/shaders/terrainShader.vs", "../assets/shaders/terrainShader.fs", dm.getAspectRatio());

    // --> raw models load <--
    printf(" -> Models... ");
    Model poolModel = procGenerator.genPool();
    Model submarineModel = objloader.loadObjModel("../assets/obj/submarine.obj", loader);
    Model poolChairModel = objloader.loadObjModel("../assets/obj/chair.obj", loader);
    Model tikibarTopModel = objloader.loadObjModel("../assets/obj/tikibarTop.obj", loader);
    Model tikibarStructModel = objloader.loadObjModel("../assets/obj/tikibarStruct.obj", loader);
    Model tikibarBottomModel = objloader.loadObjModel("../assets/obj/tikibarBottom.obj", loader);
    Model lanternModel = objloader.loadObjModel("../assets/obj/lantern.obj", loader);
    Model tableModel = objloader.loadObjModel("../assets/obj/table.obj", loader);
    Model plantModel = objloader.loadObjModel("../assets/obj/plant.obj", loader);
    Model ladderModel = objloader.loadObjModel("../assets/obj/ladder.obj", loader);
    Model poolLadderModel = objloader.loadObjModel("../assets/obj/pool_ladder.obj", loader);
    Model umbrellaModel = objloader.loadObjModel("../assets/obj/umbrella.obj", loader);
    Model trampolineModel = objloader.loadObjModel("../assets/obj/trampoline.obj", loader);
    Model jellyfishModel = objloader.loadObjModel("../assets/obj/jellyfish.obj", loader);
    Model skyModel = procGenerator.genQuad();
    printf("done.\n");

    // --> texture load <--
    printf(" -> Textures... ");
    Texture textureTiles = Texture(loader.loadTexture("../assets/textures/pool_tiles.jpg", GL_RGB));
    Texture textureSub_body = Texture(loader.loadTexture("../assets/textures/submarine_body.png", GL_RGB));
    Texture textureTrunk = Texture(loader.loadTexture("../assets/textures/brown.jpg", GL_RGB));
    Texture textureFoliage = Texture(loader.loadTexture("../assets/textures/green.jpg", GL_RGB));
    Texture textureWhite = Texture(loader.loadTexture("../assets/textures/white.jpg", GL_RGB));
    Texture textureLantern = Texture(loader.loadTexture("../assets/textures/lantern.jpg", GL_RGB));
    Texture textureTable = Texture(loader.loadTexture("../assets/textures/table.jpg", GL_RGB));
    Texture texturePlant = Texture(loader.loadTexture("../assets/textures/plant.png", GL_RGBA));
    Texture textureTrampoline = Texture(loader.loadTexture("../assets/textures/trampoline.png", GL_RGB));
    Texture textureLadder = Texture(loader.loadTexture("../assets/textures/ladder.png", GL_RGB));
    Texture textureUmbrella = Texture(loader.loadTexture("../assets/textures/umbrella.jpg", GL_RGB));
    Texture textureTikibarTop = Texture(loader.loadTexture("../assets/textures/tikibar_roof.jpg", GL_RGB));
    Texture textureTikibarStruct = Texture(loader.loadTexture("../assets/textures/tikibar_struct.jpg", GL_RGB));
    Texture textureTikibarBottom = Texture(loader.loadTexture("../assets/textures/tikibar_bottom.jpg", GL_RGB));
    Texture textureJellyfish = Texture(loader.loadTexture("../assets/textures/jellyfish.png", GL_RGBA));
    Texture textureSky = Texture(loader.loadTexture("../assets/textures/sky.jpg", GL_RGB));

    // --> texture parameters <--
    textureTiles.setDamper(4.0);
    textureTiles.setReflectivity(1.0);
    textureSub_body.setDamper(200.0);
    textureSub_body.setReflectivity(1.0);
    texturePlant.setTransparency(true);
    texturePlant.setReflectivity(0.1);
    texturePlant.setDamper(10.0);
    textureTable.setReflectivity(0.05);
    textureTable.setDamper(10.0);
    textureJellyfish.setReflectivity(0.2);
    textureSky.setTransparency(true); // just to avoid culling
    printf("done.\n");

    printf(" -> Objects and lights generation... ");
    // --> pool <--
    glm::vec3 poolScale = glm::vec3(8.0f, 4.0f, 6.0f);
    Entity pool = Entity(TexturedModel(poolModel, textureTiles), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), poolScale);
    pool.editPosition(glm::vec3(0.0, -2.0, 0.0));

    // --> complete models assembly <--
    std::vector<Entity> tree1 = procGenerator.genTree(120, 8.0, -8.0, 2.5, textureFoliage, textureTrunk);
    std::vector<Entity> tree2 = procGenerator.genTree(120, 3.3, -7.7, 2.0, textureFoliage, textureTrunk);
    std::vector<Entity> tree3 = procGenerator.genTree(120, 8.4, -4.8, 1.0, textureFoliage, textureTrunk);
    std::vector<Entity> tree4 = procGenerator.genTree(120, 5.5, -5.0, 1.25, textureFoliage, textureTrunk);
    std::vector<Entity> chairs = {
        Entity(TexturedModel(poolChairModel, textureWhite), glm::vec3(-4.0, 0.0, -4.0), glm::vec3(0.0, 45.0, 0.0), glm::vec3(0.025, 0.025, 0.025)),
        Entity(TexturedModel(poolChairModel, textureWhite), glm::vec3(-2.0, 0.0, -4.0), glm::vec3(0.0, -35.0, 0.0), glm::vec3(0.025, 0.025, 0.025)),
        Entity(TexturedModel(poolChairModel, textureWhite), glm::vec3(2.0, 0.0, 4.0), glm::vec3(0.0, 190.0, 0.0), glm::vec3(0.025, 0.025, 0.025))};
    std::vector<Entity> plants = {
        // pool right
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(9.0, 0.0, -2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(9.0, 0.0, -0.5), glm::vec3(0.0, 0.2, 0.0), glm::vec3(2.0, 2.0, 1.0)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(9.25, 0.0, 0.5), glm::vec3(0.0, -0.3, 0.0), glm::vec3(0.5, 0.25, 0.5)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(9.5, 0.0, 1.0), glm::vec3(0.0, 0.8, 0.0), glm::vec3(0.5, 1.5, 0.8)),
        // lower-right corner
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(9.5, 0.0, 8.0), glm::vec3(0.0, 0.1, 0.0), glm::vec3(1.0, 0.7, 1.0)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(9.6, 0.0, 9.0), glm::vec3(0.0, 0.6, 0.0), glm::vec3(1.3, 1.2, 1.1)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(8.0, 0.0, 9.5), glm::vec3(0.0, 0.26, 0.0), glm::vec3(0.8, 0.9, 0.6)),
        // lower-left corner
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(-9.6, 0.0, 9.0), glm::vec3(0.0, 0.5, 0.0), glm::vec3(1.5, 1.2, 1.7)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(-8.0, 0.0, 9.5), glm::vec3(0.0, 0.46, 0.0), glm::vec3(0.8, 0.6, 0.9)),
        // higher-left corner
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(-9.8, 0.0, -2.6), glm::vec3(0.0, 0.12, 0.0), glm::vec3(0.4, 0.4, 0.4)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(-9.3, 0.0, -3.4), glm::vec3(0.0, 0.34, 0.0), glm::vec3(0.8, 0.7, 0.7)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(-9.5, 0.0, -4.0), glm::vec3(0.0, 0.6, 0.0), glm::vec3(1.0, 1.1, 2.0)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(-9.8, 0.0, -5.5), glm::vec3(0.0, 0.46, 0.0), glm::vec3(1.0, 0.6, 0.8)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(-9.6, 0.0, -6.3), glm::vec3(0.0, 0.77, 0.0), glm::vec3(1.3, 1.2, 1.1)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(-9.3, 0.0, -7.6), glm::vec3(0.0, 0.32, 0.0), glm::vec3(1.5, 1.0, 1.3)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(-9.7, 0.0, -8.8), glm::vec3(0.0, 0.06, 0.0), glm::vec3(1.9, 1.8, 2.0)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(-9.0, 0.0, -9.8), glm::vec3(0.0, 0.45, 0.0), glm::vec3(2.0, 1.6, 2.0)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(-6.6, 0.0, -9.3), glm::vec3(0.0, 0.22, 0.0), glm::vec3(1.0, 1.5, 1.5)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(-5.5, 0.0, -9.5), glm::vec3(0.0, 0.76, 0.0), glm::vec3(0.8, 1.5, 1.2)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(-4.1, 0.0, -9.1), glm::vec3(0.0, 0.83, 0.0), glm::vec3(1.7, 0.8, 1.3)),
        Entity(TexturedModel(plantModel, texturePlant), glm::vec3(-2.6, 0.0, -9.6), glm::vec3(0.0, 0.43, 0.0), glm::vec3(1.1, 0.6, 1.5)),
    };
    std::vector<Entity> tikibar = {
        Entity(TexturedModel(tikibarTopModel, textureTikibarTop), glm::vec3(-6.0, 3.5, 6.5), glm::vec3(0.0, 90.0, 0.0), glm::vec3(4.0, 4.0, 4.0)),
        Entity(TexturedModel(tikibarStructModel, textureTikibarStruct), glm::vec3(-6.0, 0.0, 6.5), glm::vec3(0.0, 110.0, 0.0), glm::vec3(3.0, 6.0, 4.0)),
        Entity(TexturedModel(tikibarBottomModel, textureTikibarBottom), glm::vec3(-6.0, 0.0, 6.5), glm::vec3(0.0, 110.0, 0.0), glm::vec3(3.0, 6.0, 4.0))};
    std::vector<Entity> sphere = procGenerator.genSphere(4, glm::vec3(-1.0, -3.0, -1.0), 1.0, textureWhite);
    Entity lantern = Entity(TexturedModel(lanternModel, textureLantern), glm::vec3(6.0, 1.0, 7.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.35, 0.35, 0.35));
    Entity table = Entity(TexturedModel(tableModel, textureTable), glm::vec3(6.0, 0.0, 7.0), glm::vec3(0.0, 15.0, 0.0), glm::vec3(2.0, 1.3, 2.0));
    Entity trampoline = Entity(TexturedModel(trampolineModel, textureTrampoline), glm::vec3(-3.2, 2.5, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.3, 0.1, 0.3));
    Entity ladder = Entity(TexturedModel(ladderModel, textureLadder), glm::vec3(-5.0, 0.0, 0.0), glm::vec3(0.0, -90.0, 0.0), glm::vec3(0.75, 1.2, 1.0));
    Entity poolLadder = Entity(TexturedModel(poolLadderModel, textureWhite), glm::vec3(3.0, -0.6, -3.05), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
    Entity umbrella = Entity(TexturedModel(umbrellaModel, textureUmbrella), glm::vec3(-3.0, 0.0, -4.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.05, 0.035, 0.05));
    Entity jellyfish = Entity(TexturedModel(jellyfishModel, textureJellyfish), glm::vec3(1.5, -0.1, 1.5), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.25, 0.25, 0.25));
    Entity sky = Entity(TexturedModel(skyModel, textureSky), glm::vec3(0.0, 50.0, 0.0), glm::vec3(0.0, 90.0, 0.0), glm::vec3(500.0, 1.0, 500.0));

    // --> submarine <--
    TexturedModel tm = TexturedModel(submarineModel, textureSub_body);
    Player player = Player(tm, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.025, 0.025, 0.025), poolScale);

    // --> lights <--
    std::vector<Light> lights = {
        // sun
        Light(glm::vec3(0.0, 100.0, 0.0), glm::vec3(1.0, 0.9, 0.8), 1.0),
        // moon
        Light(glm::vec3(0.0, 100.0, 0.0), glm::vec3(0.2, 0.3, 0.4), 1.0),
        // lantern
        Light(glm::vec3(6.0, 1.5, 7.0), glm::vec3(1.0, 0.8, 0.8), 1.0, glm::vec3(1.0, 0.7, 0.1)),
        // tiki bar
        Light(glm::vec3(-6.0, 3.0, 6.5), glm::vec3(1.0, 1.0, 1.0), 1.0, glm::vec3(0.8, 0.5, 0.02))};

    // --> camera generation <--
    Camera camera = Camera(&player, dm, &scrollDelta, &pointerPositionX, &pointerPositionY, &pointerDeltaX, &pointerDeltaY, &isSpacePressed, &clicked);

    // --> terrain and terrain textures <--
    TerrainTexture rTexture = TerrainTexture(loader.loadTexture("../assets/textures/grass.jpg", GL_RGB));
    TerrainTexture gTexture = TerrainTexture(loader.loadTexture("../assets/textures/wood_floor.jpg", GL_RGB));
    TerrainTexture bTexture = TerrainTexture(loader.loadTexture("../assets/textures/road.jpg", GL_RGB));
    TerrainTexturePack texturePack = TerrainTexturePack(rTexture, gTexture, bTexture);
    TerrainTexture blendMap = TerrainTexture(loader.loadTexture("../assets/textures/blend_map.png", GL_RGBA));
    Terrain terrain = Terrain(0.5, 0.5, loader, texturePack, blendMap);
    printf("done.\n");

    // --> water <--
    printf(" -> Water... ");
    WaterFrameBuffers fbos = WaterFrameBuffers(dm.getScreenWidth(), dm.getScreenHeight());
    WaterShader waterShader = WaterShader("../assets/shaders/waterShader.vs", "../assets/shaders/waterShader.fs");
    waterShader.getUniformLocations();
    WaterRenderer waterRenderer = WaterRenderer(loader, waterShader, renderer.getProjectionMatrix(), fbos, &dm);
    std::vector<WaterTile> waterTiles = {WaterTile(0.0, -0.001, 0.0, 1.5)};
    printf("done.\n");

    // --> model collection <--
    printf(" -> Filling up the structures... ");
    std::vector<Entity> entities = {lantern, pool, table, trampoline, ladder, poolLadder, umbrella, jellyfish, sky};
    for (auto part : tree1)
        entities.push_back(part);
    for (auto part : tree2)
        entities.push_back(part);
    for (auto part : tree3)
        entities.push_back(part);
    for (auto part : tree4)
        entities.push_back(part);
    for (auto plant : plants)
        entities.push_back(plant);
    for (auto chair : chairs)
        entities.push_back(chair);
    for (auto arc : sphere)
        entities.push_back(arc);
    for (auto part : tikibar)
        entities.push_back(part);
    std::vector<Terrain> terrains = {terrain};
    printf("done.\n");
    printf("Load completed. \nStart rendering...\n");

    // --> main loop <--
    do
    {
        glfwPollEvents();
        loadInterface();

        // day/night tuning
        lights[0].setGain(sunAmount);
        lights[1].setGain(1.0 - sunAmount);

        // camera and submarine update
        camera.move();
        player.move(dm);

        // render reflection below the water level on reflection FB
        glEnable(GL_CLIP_DISTANCE0);
        fbos.bindReflectionFB();
        camera.invertPitch();
        renderer.renderScene(entities, terrains, lights, camera, player, glm::vec4(0.0, 1.0, 0.0, -waterTiles[0].getY()));
        camera.resetPitch();

        // render refraction above the water level on refraction FB, no water
        fbos.bindRefractionFB();
        renderer.renderScene(entities, terrains, lights, camera, player, glm::vec4(0.0, -1.0, 0.0, waterTiles[0].getY()));

        // render regular scene, with water
        glDisable(GL_CLIP_DISTANCE0);
        fbos.unbindCurrentFB();
        renderer.renderScene(entities, terrains, lights, camera, player, glm::vec4(0.0));
        waterRenderer.render(waterTiles, camera, lights[0]);

        // imgui window show
        drawInterface();

        glfwSwapBuffers(dm.getWindow());
        dm.updateTime();
    } while (glfwGetKey(dm.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS);

    // --> cleanup <--
    printf("Rendering stopped.\n");
    fbos.clean();
    waterShader.clean();
    renderer.clean();
    loader.clean();
    dm.clean();
    return 0;
}
