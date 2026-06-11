#include "hot_reload.hpp"
#include <cstddef>
#include <raylib.h>
#include <sys/stat.h>

long GetFileModTime(const char* fileName);

HotShader::HotShader(const char* fsPath)
    :fsPath(fsPath)
{
    this->shader = LoadShader(NULL, fsPath);
    this->fsModTime = GetFileModTime(fsPath);
}


long GetFileModTime(const char *fileName)
{
    if(!fileName)
        return 0;

    struct stat s;
    if(stat(fileName, &s) == 0)
    {
        return s.st_mtime;
    }

    return 0;
}


void HotShader::update()
{
    long lastModTime = GetFileModTime(this->fsPath);
    
    if(lastModTime != this->fsModTime)
    {
        UnloadShader(this->shader);
        Shader newShader = LoadShader(NULL, this->fsPath);

        if(newShader.id != 0)
        {
            this->shader = newShader;
        }

        this->fsModTime = lastModTime;
    }
}

void HotShader::unload()
{
    UnloadShader(this->shader);
}