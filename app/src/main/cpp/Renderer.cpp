
#include <malloc.h>
#include <android/log.h>
#include <EGL/egl.h>
#include "Renderer.h"

static const char VERTEX_SHADER[] =
        "#version 100\n"
        "uniform vec2 offset;\n"
        "uniform vec2 scale;\n"
        "attribute vec2 pos;\n"
        "attribute vec4 color;\n"
        "varying vec4 vColor;\n"
        "void main() {\n"
        "    gl_Position = vec4(pos*scale + offset, 0.0, 1.0);\n"
        "    vColor = color;\n"
        "}\n";

static const char FRAGMENT_SHADER[] =
        "#version 100\n"
        "precision mediump float;\n"
        "varying vec4 vColor;\n"
        "void main() {\n"
        "    gl_FragColor = vColor;\n"
        "}\n";

struct Vertex {
    GLfloat pos[2];
    GLubyte rgba[4];
};

static const Vertex QUAD[4] = {
        {{-0.1f, -0.05f}, {0x00, 0xFF, 0x00}},
        {{ 0.1f, -0.05f}, {0x00, 0x00, 0xFF}},
        {{-0.1f,  0.05f}, {0xFF, 0x00, 0x00}},
        {{ 0.1f,  0.05f}, {0xFF, 0xFF, 0xFF}},
};

bool checkGlError(const char* funcName) {
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        ALOGE("GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}

GLuint createShader(GLenum shaderType, const char* src) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        checkGlError("glCreateShader");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                ALOGE("Could not compile %s shader:\n%s\n",
                      shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                      infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createProgram(const char* vtxSrc, const char* fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    bool error = false;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader)
        error = true;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader)
        error = true;

    program = glCreateProgram();

    if (!program) {
        checkGlError("glCreateProgram");
        error = true;
    }

    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        ALOGE("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                ALOGE("Could not link program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

    if(error) {
        glDeleteShader(vtxShader);
        glDeleteShader(fragShader);
    }

    return program;
}

Renderer::Renderer()
        :   mProgram(0),
            mVB(0),
            mPosAttrib(-1),
            mColorAttrib(-1),
            mOffsetUniform(-1)
{

}

Renderer::~Renderer() {
    glDeleteBuffers(1, &mVB);
    glDeleteProgram(mProgram);
}

bool Renderer::init() {
    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (!mProgram)
        return false;
    mPosAttrib = glGetAttribLocation(mProgram, "pos");
    mColorAttrib = glGetAttribLocation(mProgram, "color");
    mOffsetUniform = glGetUniformLocation(mProgram, "offset");
    mScaleUniform = glGetUniformLocation(mProgram, "scale");

    mScale[0] = 1.0f;
    mScale[1] = 1.0f;

    glGenBuffers(1, &mVB);
    glBindBuffer(GL_ARRAY_BUFFER, mVB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD), &QUAD[0], GL_STATIC_DRAW);

    return true;
}

void Renderer::render(float x, float y) {

    glUseProgram(mProgram);

    glBindBuffer(GL_ARRAY_BUFFER, mVB);
    glVertexAttribPointer(mPosAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, pos));
    glVertexAttribPointer(mColorAttrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, rgba));
    glEnableVertexAttribArray(mPosAttrib);
    glEnableVertexAttribArray(mColorAttrib);

    mPosition[0] = x;
    mPosition[1] = y;
    glUniform2fv(mOffsetUniform, 1, mPosition);
    glUniform2fv(mScaleUniform, 1, mScale);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    checkGlError("Renderer::render");
}

void Renderer::scale(float x, float y) {
    mScale[0] = x;
    mScale[1] = y;
}

bool Renderer::checkPointRectangleCollision(Renderer *circle) {

    float circleX = circle->mPosition[0];
    float circleY = circle->mPosition[1];

    float rectMinX = (QUAD[0].pos[0] * mScale[0]) + mPosition[0];
    float rectMaxX = (QUAD[3].pos[0] * mScale[0]) + mPosition[0];

    float rectMinY = (QUAD[0].pos[1] * mScale[1]) + mPosition[1];
    float rectMaxY = (QUAD[3].pos[1] * mScale[1]) + mPosition[1];

    return circleX >= rectMinX && circleX <= rectMaxX && circleY >= rectMinY && circleY <= rectMaxY;
}
