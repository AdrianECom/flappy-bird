#ifndef GAME_H
#define GAME_H

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <vector>
#include "Renderer.h"

class Game{
public:
    Game();
    virtual ~Game();

    void init();
    void resize(int w, int h);
    void render();

    Renderer* createPipe();

    /**
     * Called when screen is touched.
     */
    void touch();

    /**
     * @return A new game
     */
    static Game* createGame();

private:

    /**
     * One iteration of the world.
     */
    void step();

    /**
     * Checks if bird collides with pipes.
     */
    void checkCollisions();

    uint64_t mLastFrameNs;
    const EGLContext mEglContext;

    std::vector<Renderer*> upperPipes;

    std::vector<Renderer*> lowerPipes;
    Renderer* bird;
    bool touched;

    float timerTouched;
    float gravity;

    float velBirdY = 0.0f;
    float velPipesY = 0.0f;
    float collisionDetected;
};

#endif //GAME_H
