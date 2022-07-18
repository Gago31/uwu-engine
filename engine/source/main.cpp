#include <iostream>
#include <string>
#include "Grid.h"
#include "Enemy.h"
#include "Sprite.h"
#include "Player.h"
#include "SceneGraph.h"
#include "Node3D.h"
#include "MeshNode.h"
#include "GridNode.h"
#include "SpriteNode.h"
#include "TextBox.h"
#include "SoundEmitter.h"
#include "SoundEmitter3D.h"

#include "uwu.h"


void loadResources() {
    // Shaders
    uwuLoadShader("assets/shaders/sprite_shader.vsh", "assets/shaders/sprite_shader.fs", nullptr, "spriteShader");
    uwuLoadShader("assets/shaders/text_shader.vsh", "assets/shaders/text_shader.fs", nullptr, "textShader");
    uwuLoadShader("assets/shaders/model_shader.vsh", "assets/shaders/model_shader.fs", nullptr, "modelShader");
    uwuLoadShader("assets/shaders/model_lighting_shader.vsh", "assets/shaders/model_lighting_shader.fs", nullptr, "modelShaderL");
    
    // Textures
    uwuLoadTexture("assets/imgs/uwu.png", true, "uwu");
    uwuLoadTexture("assets/imgs/textbox.png", true, "textBox");
    
    // Models
    uwuLoadModel("assets/models/dcp.obj", "DCP");
    uwuLoadModel("assets/models/greenCube.obj", "greenCube");
    uwuLoadModel("assets/models/greenPlane.obj", "greenPlane");

    // Sounds
    uwuLoadSoundWAV("assets/music/moths_bgm_mono.wav", "bgm");
    uwuLoadSoundWAV("assets/sounds/buenos_dias.wav", "buenosDias");
    uwuLoadSoundWAV("assets/sounds/te_deseo_buen_dia.wav", "muyBuenDia");
}

void setInputBindings() {
    uwu::setKeyboardBindings({
        { "left",  KEYBOARD_KEY_LEFT },
        { "right", KEYBOARD_KEY_RIGHT },
        { "up",    KEYBOARD_KEY_UP },
        { "down",  KEYBOARD_KEY_DOWN },
        { "a",     KEYBOARD_KEY_Z },
        { "b",     KEYBOARD_KEY_X }
    });
    uwu::setJoystickBindings({
        { "left",  DPAD_LEFT },
        { "right", DPAD_RIGHT },
        { "up",    DPAD_UP },
        { "down",  DPAD_DOWN },
        { "a",     XBOX_A },
        { "b",     XBOX_B }
    });
    uwu::setAxisBindings({
        { "left",  LEFT_STICK_LEFT },
        { "right", LEFT_STICK_RIGHT },
        { "up",    LEFT_STICK_UP },
        { "down",  LEFT_STICK_DOWN },
    });
}

Scene buildScene() {
    // Create scene
    Scene scene;
    Node* root = scene.root;

    // Player
    Player* player = new Player(0.0f, 3.0f, 0.25f, 90);
    player->camera->makeCurrent();
    root->addChild(player);

    // Map layout
    int w = 10;
    int h = 10;

    Grid g(w, h, {
        1, 1, 1, 1, 1, 1, 0, 1, 1, 1,
        1, 0, 0, 0, 1, 0, 0, 1, 1, 1,
        1, 0, 1, 0, 0, 0, 1, 0, 0, 0,
        1, 0, 0, 0, 1, 0, 0, 0, 1, 1,
        1, 1, 0, 1, 1, 1, 1, 0, 0, 1,
        1, 1, 0, 1, 1, 0, 1, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
        1, 1, 0, 1, 0, 1, 0, 0, 0, 1,
        1, 1, 0, 1, 0, 0, 1, 0, 0, 1,
        1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
    });

    g.print();
    GridNode* gridNode = new GridNode(g);
    root->addChild(gridNode);

    // Map enemy
    //std::vector<int> enemyPath = { LEFT, UP, UP, RIGHT, RIGHT, DOWN, DOWN, LEFT };
    std::vector<int> enemyPath = { LEFT, WAIT | UP, UP, UP, WAIT | RIGHT, RIGHT, RIGHT, WAIT | DOWN, DOWN, DOWN, WAIT | LEFT, LEFT };
    //std::vector<int> enemyPath = { WAIT | UP, DOUBLE | UP, WAIT | RIGHT, DOUBLE | RIGHT, WAIT | DOWN, DOUBLE | DOWN, WAIT | LEFT, DOUBLE | LEFT };
    Enemy* enemy = new Enemy({ 2, 6, 0 }, enemyPath);
    MeshNode* dcpNode = new MeshNode("DCP", "modelShader");
    dcpNode->transform.scale(0.2f);
    dcpNode->transform.rotateX(glm::pi<float>() * 0.5);
    enemy->addChild(dcpNode);
    SoundEmitter3D* dolphinSongNode = new SoundEmitter3D({ 0, 0, 0 }, "muyBuenDia", false);
    dolphinSongNode->setLooping(false);
    dolphinSongNode->name = "songNode";
    enemy->addChild(dolphinSongNode);
    root->addChild(enemy);

    // Sprite
    Sprite gpuwu("uwu");
    gpuwu.transform.translate(16.0f, 620.0f);
    root->addChild(new SpriteNode("gpuwu", gpuwu, "spriteShader"));

    // Text box
    Sprite textBox("textBox");
    textBox.transform.translate(40.0f, 500.0f);
    TextBox* textBoxNode = new TextBox(textBox, "assets/fonts/DelaGothicOne-Regular.ttf");
    textBoxNode->text = "Esto no es un texto en espagnol.";
    //textBoxNode->text = "Esto es un texto en español.";
    textBoxNode->transform.translate(70.0f, 164.0f);
    textBoxNode->hide();
    root->addChild(textBoxNode);

    // Background music
    SoundEmitter* bgmNode = new SoundEmitter("bgm", true);
    root->addChild(bgmNode);

    // save scene
    scene.save("scene.json");

    return scene;
}

int main() {
    uwu::initialize();
    setInputBindings();

    // --- Build scene tree ---
    loadResources();
    Scene scene = buildScene();

    // Load scene tree from file
    //Scene scene = Scene::load("scene.json");

    uwu::setMainScene(scene);

    uwu::run();

    uwu::kill();

    return 0;
}
