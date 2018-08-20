package com.android.gles2jni;

import android.app.Activity;
import android.os.Bundle;
import android.view.MotionEvent;

public class GLES2JNIActivity extends Activity {

    GLES2JNIView mView;

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        mView = new GLES2JNIView(getApplication());
        setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        return super.onTouchEvent(event) && mView.onTouchEvent(event);
    }
}
