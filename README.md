SupraHotFramework
====
The SHF is a cross-platform 3d rendering framework, which at the moment runs on Windows, macOS and Android.
It is currently in it's early infancy. It features already an interesting shader and material build pipeline. 

# Notes
## Compiling for Android
The libSupraHotLib.so must be moved to this location: "x:\SupraHotFramework\Intermediates\SupraHotSandbox\Tegra-Android\Debug\libs\armeabi-v7a\" prior to compiling the sandbox app.
Otherwise it won't find the library on app start

# Supported DDS formats
+ 128 bpp float (RGBA32f)
+ 64 bpp half (RGBA16f)
+ 64 bpp uint (RGBA16 / Short) [not working for android.]
+ 32 bpp rgba (RGBA8)
+ 32 bpp bgra (RGBA8)
+ 32 bpp bgr -> (RGBA8)

## Compressed dds files won't be supported
But then even use .dds??
+ cube maps.

# Supported platforms
## Windows

## Android

## mac OS (WIP)
+ WindowMac.cpp needs to be implemented
+ SandBoxApp.mm needs to be created
+ XCode project needs to be created. This will also help with compiling the lib for iOs

## iOS	(Comming after mac)

## Web	
+ This will eventually be done after all other builds are good to go.