#include "Game.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

static const int SEED = 128;
static const int NUM_PIPES = 10;
static const float TOUCHED_TIME = 50.0f;
static const float MAX_GRAVITY = 0.01f;
static const float DELTA_GRAVITY = 0.0005f;
static const float IMPULSE = 0.05f;
static const float PIPE_Y_COORD = 0.55f;
static const float BIRD_X_COORD = -0.5f;
static const float PIPE_X_VELOCITY = 0.01f;

Game::Game()
        :   mLastFrameNs(0),
            mEglContext(eglGetCurrentContext()),
            bird(NULL),
            touched(false),
            timerTouched(0.0f),
            gravity(0.0f),
            collisionDetected(false)
{
}

void Game::init() {

    /*
     * Create pipes and bird.
     */

    upperPipes.reserve(NUM_PIPES);
    lowerPipes.reserve(NUM_PIPES);

    for (int indexPipes = 0; indexPipes < NUM_PIPES; ++indexPipes) {
        upperPipes[indexPipes] = createPipe();
        lowerPipes[indexPipes] = createPipe();
    }

    bird = new Renderer();
    bird->init();
}

Renderer *Game::createPipe() {
    Renderer* pipe = new Renderer();
    pipe->init();
    pipe->scale(1.0f, 8.0f);

    return pipe;
}

Game::~Game() {
    for (int indexPipes = 0; indexPipes < NUM_PIPES; ++indexPipes) {
        delete upperPipes[indexPipes];
        delete lowerPipes[indexPipes];
    }
}

void Game::resize(int w, int h) {
    mLastFrameNs = 0;
    glViewport(0, 0, w, h);
}

void Game::step() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    auto nowMs = now.tv_sec*1000.0f + now.tv_nsec*0.000001;

    srand(SEED);

    float dt = nowMs - mLastFrameNs; // DELTA TIME,

    timerTouched += dt; // Increase timer.

    // IF touch screen THEN add impulse to bird ELSE gravity affects bird.
    if(touched)
        velBirdY += IMPULSE;
    else {
        if(gravity < MAX_GRAVITY) // clamp gravity
            gravity += DELTA_GRAVITY ;

        velBirdY -= gravity;
    }

    if(timerTouched >= TOUCHED_TIME) {
        touched = false;
        timerTouched = 0.0f;
    }

    velPipesY -= PIPE_X_VELOCITY;

    if (mLastFrameNs > 0) {

        bird->render(BIRD_X_COORD, velBirdY);

        for (int indexPipes = 0; indexPipes < NUM_PIPES; ++indexPipes) {
            float upperRandomOffset = (rand()%5 + 1)/10.0f;
            upperPipes[indexPipes]->render(1.0f + indexPipes + velPipesY + upperRandomOffset,PIPE_Y_COORD);

            float lowerRandomOffset = (rand()%5 + 1)/10.0f;
            lowerPipes[indexPipes]->render(1.0f + indexPipes + velPipesY + lowerRandomOffset,-PIPE_Y_COORD);
        }

        checkCollisions();
    }


    mLastFrameNs = nowMs;
}

void Game::render() {
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(!collisionDetected)
        step();
}

Game* Game::createGame() {
    Game* game = new Game;
    game->init();
    return game;
}

void Game::touch() {
    if(!touched) {
        touched = true;
        timerTouched = 0.0f;
    }
}

void Game::checkCollisions() {
    for (int i = 0; i < NUM_PIPES && !collisionDetected; ++i)
        collisionDetected = lowerPipes[i]->checkPointRectangleCollision(bird);

    for (int i = 0; i < NUM_PIPES && !collisionDetected; ++i)
        collisionDetected = upperPipes[i]->checkPointRectangleCollision(bird);
}