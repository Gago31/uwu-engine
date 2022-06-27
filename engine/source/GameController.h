#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Grid.h"
#include "SceneGraph.h"
#include "Camera.h"


class GameController {
    public:
        inline static unsigned int SCR_WIDTH = 1280;
        inline static unsigned int SCR_HEIGHT = 720;
        /*static unsigned int SCR_WIDTH = 1920;
        static unsigned int SCR_HEIGHT = 1080;*/
        inline static float SCREEN_RATIO = float(SCR_WIDTH) / float(SCR_HEIGHT);;
        inline static bool run_turn = false;
        inline static float TURN_PERIOD = 0.5f;
        inline static float SQUARE_SIZE = 1.0f;
        inline static float TURN_PROGRESS = 0.0f;
        inline static float fillPolygon = true;
        inline static GridPtr currentGrid;
        inline static Scene *currentScene;
        inline static Camera *currentCamera;
        static void initialize() {
            SCR_WIDTH = 1280; // 1920
            SCR_HEIGHT = 720; // 1080
            SCREEN_RATIO = float(SCR_WIDTH) / float(SCR_HEIGHT);
            run_turn = false;
            TURN_PERIOD = 0.5f;
        }
        static void update(float dt) {
            if (run_turn) {
                TURN_TIME = glm::clamp(TURN_TIME + dt, 0.0f, TURN_PERIOD);
                TURN_PROGRESS = TURN_TIME / TURN_PERIOD;

                if (TURN_TIME == TURN_PERIOD) {
                    TURN_PROGRESS = 1.0f;
                    run_turn = false;
                    TURN_TIME = 0.0f;
                }
            }
        }
    private:
        inline static float TURN_TIME = 0.0f;
        GameController() { }
};

#endif // !GAME_CONTROLLER_H
