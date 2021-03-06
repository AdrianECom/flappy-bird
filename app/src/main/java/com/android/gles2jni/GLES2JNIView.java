package com.android.gles2jni;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class GLES2JNIView extends GLSurfaceView {
    private static final String TAG = "GLES2JNI";
    private static final boolean DEBUG = true;

    public GLES2JNIView(Context context) {
        super(context);
        // Pick an EGLConfig with RGB8 color, 16-bit depth, no stencil,
        // supporting OpenGL ES 2.0 or later backwards-compatible versions.
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(2);
        setRenderer(new Renderer());
    }

    private static class Renderer implements GLSurfaceView.Renderer {
        public void onDrawFrame(GL10 gl) {
            GLES2JNILib.step();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            GLES2JNILib.resize(width, height);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            GLES2JNILib.init();
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        GLES2JNILib.touch();
        return super.onTouchEvent(event);
    }
}
