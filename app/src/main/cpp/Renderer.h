#ifndef RENDERER_H
#define RENDERER_H

#include <GLES2/gl2.h>

#define DEBUG 1

#define LOG_TAG "GLES2JNI"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif


class Renderer{
public:
    Renderer();
    virtual ~Renderer();

    bool init();

    /**
     * Renders object in a position x,y.
     * @param x
     * @param y
     */
    void render(float x, float y);

    /**
     * Sets a scale for object.
     * @param x
     * @param y
     */
    void scale(float x, float y);

    /**
     * Checks if point collides with rectangle.
     * @param circle
     * @return
     */
    bool checkPointRectangleCollision(Renderer *circle);

private:
    GLuint mProgram;

    GLuint mVB;

    GLint mPosAttrib;
    GLint mColorAttrib;

    GLint mOffsetUniform;
    GLint mScaleUniform;

    float mScale[2];

    float mPosition[2];

};

#endif // RENDERER_H
