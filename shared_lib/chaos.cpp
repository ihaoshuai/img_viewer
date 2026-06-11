#include "raylib.h"


#ifdef _WIN32
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __attribute__((visibility("default")))
#endif


extern "C" {


    DLL_EXPORT void UpdateShaderUniforms(Shader shader) 
    {

        int timeLoc = GetShaderLocation(shader, "uTime");
        int centerLoc = GetShaderLocation(shader, "uCenter");
        int texSizeLoc = GetShaderLocation(shader, "uTexSize");

        if(!IsCursorOnScreen())
        {
            float mousePos[2] = { -1.0f, -1.0f };
            SetShaderValue(shader, centerLoc, &mousePos, SHADER_UNIFORM_VEC2);
            return;
        }


        Vector2 mousePos = GetMousePosition();
        SetShaderValue(shader, centerLoc, &mousePos, SHADER_UNIFORM_VEC2);

        float texSize[2];
        texSize[0] = GetScreenWidth();
        texSize[1] = GetScreenHeight();
        SetShaderValue(shader, texSizeLoc, &texSize, SHADER_UNIFORM_VEC2);

        float time = GetTime();
        SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
    }
}