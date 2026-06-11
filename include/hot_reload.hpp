#pragma once

#include <raylib.h>

struct HotShader
{
    Shader shader;
    const char* fsPath;
    long fsModTime;

    void update();
    HotShader(const char* fsPath);
    void unload();
};


