package com.suprahot;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.SurfaceHolder;

public class SupraHotGLView extends GLSurfaceView {

    private SupraHotRenderer renderer;
    protected static int surfaceWidth = 0;
    protected static int surfaceHeight = 0;

    public SupraHotGLView(Context context) {
        super(context);

        // set gles params
        setEGLContextClientVersion(3);

        renderer = new SupraHotRenderer();
        setRenderer(renderer);
    }


    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
		super.surfaceDestroyed(holder);
        // Call to native 'destroy' function
        SupraHotSandbox.instance.destroyApp();
    }

    @Override
    public void surfaceChanged (SurfaceHolder holder, int format, int w, int h) {
		super.surfaceChanged(holder, format, w, h);
    	surfaceWidth = w;
    	surfaceHeight = h;
    }

}
