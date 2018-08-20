package com.android.gles2jni;

// Wrapper for native library

public class GLES2JNILib {

     static {
          System.loadLibrary("gles2jni");
     }

     public static native void init();
     public static native void resize(int width, int height);
     public static native void step();
     public static native void touch();
}
