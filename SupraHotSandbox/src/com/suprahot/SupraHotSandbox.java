package com.suprahot;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;
import android.util.Log;

import android.view.Window;
import android.view.WindowManager;
import android.content.res.AssetManager;

public class SupraHotSandbox extends Activity
{
	protected static SupraHotSandbox instance;
    private SupraHotGLView glView;
    private AssetManager assetManager;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

		instance = this;

        assetManager = getAssets();
        setAssetManager(assetManager);
        
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        glView = new SupraHotGLView(this);
        setContentView(glView);
    }

    @Override
    protected void onDestroy() {
        // call to native 'destroy' function
		//destroyApp();
    }

    // -----------------
    // Native functions
    // -----------------

    // sets up the appliction.
    // this include loading assets, scripts, creating gpu content, etc.
    // so this function must run in an GL-Thread
    public native void initApp(int width, int height);

    // progresses the current state of the app.
    // this is essentially the gameloop which gets automatically called the glsurface
    public native void progressApp();

    // tells the native app to resize.
    // this function resizes framebuffers and so on
    public native void resizeApp(int width, int height);

    // tells the app to de-allocate all resources
    public native void destroyApp();

    public native void setAssetManager(AssetManager assetManager);

    static {
        System.loadLibrary("SupraHotSandbox");
    }
}
