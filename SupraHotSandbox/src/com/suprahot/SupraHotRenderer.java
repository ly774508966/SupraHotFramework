package com.suprahot;

import android.opengl.GLES30;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class SupraHotRenderer implements GLSurfaceView.Renderer {

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        // call to native 'app::init' function
        SupraHotSandbox.instance.initApp(SupraHotGLView.surfaceWidth, SupraHotGLView.surfaceHeight);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int i, int i1) {
        // call to native 'app::resize' function
        SupraHotGLView.surfaceWidth = i;
        SupraHotGLView.surfaceHeight = i1;
        SupraHotSandbox.instance.resizeApp(i, i1);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        // call to native 'app::progress' function
        SupraHotSandbox.instance.progressApp();
    }
}
