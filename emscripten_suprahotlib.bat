emcc -DPLATFORM_EMSCRIPTEN -I"Externals\include" "SupraHotLib\App.cpp" "SupraHotLib\Camera.cpp" "SupraHotLib\Component.cpp" "SupraHotLib\Entity.cpp" "SupraHotLib\Face.cpp" "SupraHotLib\FileReader.cpp" "SupraHotLib\FileSystem.cpp" "SupraHotLib\FrameBufferObject.cpp" "SupraHotLib\FullScreenQuad.cpp" "SupraHotLib\Mat4.cpp" "SupraHotLib\Material.cpp" "SupraHotLib\MaterialLoader.cpp" "SupraHotLib\Mesh.cpp" "SupraHotLib\MeshComponent.cpp" "SupraHotLib\MeshData.cpp" "SupraHotLib\MeshDataLoader.cpp" "SupraHotLib\Publisher.cpp" "SupraHotLib\Quat4.cpp" "SupraHotLib\Renderer.cpp" "SupraHotLib\Shader.cpp" "SupraHotLib\stb_image.c" "SupraHotLib\StringUtil.cpp" "SupraHotLib\Subscriber.cpp" "SupraHotLib\SubscriberComponent.cpp" "SupraHotLib\Texture2D.cpp" "SupraHotLib\TextureCube.cpp" "SupraHotLib\Transform.cpp" "SupraHotLib\Vec2.cpp" "SupraHotLib\Vec3.cpp" "SupraHotLib\Vertex.cpp" "SupraHotLib\WindowEmscripten.cpp" "SupraHotLib\WindowInterface.cpp" -s USE_GLFW=3 -s FULL_ES3=1 --memory-init-file 0 -O3 -o "Externals\Dependencies\libs\emscripten\SupraHotLib.bc"