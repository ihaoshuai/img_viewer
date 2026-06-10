#include "raylib.h"
#include "toml.hpp"


#ifdef _WIN32
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __attribute__((visibility("default")))
#endif

namespace {
    float radius;
    float angle;
}

extern "C" {
    DLL_EXPORT bool InitShaderUniforms(Shader shader, const char *configPath)
    {
        try
        {
            auto tbl = toml::parse_file(configPath);
            if(auto params = tbl["chaos"].as_table())
            {
                radius = params->get("radius")->as_floating_point()->get();
                angle = params->get("angle")->as_floating_point()->get();
            }
        }
        catch (const toml::parse_error &err)
        {
            return false;
        }

        int radiusLoc = GetShaderLocation(shader, "uRadius");
        int angleLoc = GetShaderLocation(shader, "uAngle");

        SetShaderValue(shader, radiusLoc, &radius, SHADER_UNIFORM_FLOAT);
        SetShaderValue(shader, angleLoc, &angle, SHADER_UNIFORM_FLOAT);

        return true;
    }


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