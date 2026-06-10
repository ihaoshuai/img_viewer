
#include "raylib.h"
#include "spdlog/spdlog.h"
#include "toml.hpp"
#include <cstddef>
#include <spdlog/common.h>
#include "dload.hpp"

const char* CONFIG_PATH = "config.toml";
std::string fragmentShaderPath;
std::string libPath;
toml::table tbl;

bool LoadConfig(const char *configFilePath);
void AdjustWindow(Image img);

int main(int argc, char *argv[])
{
    spdlog::set_level(spdlog::level::debug);

    if (argc < 2)
    {
        spdlog::error("please provide a image file path");
        return -1;
    }

    //load config file
    LoadConfig(CONFIG_PATH);
    spdlog::debug("fs: {}", fragmentShaderPath);
    spdlog::debug("dynamic lib path: {}", libPath);
    LIB_HANDLE shaderLib = LOAD_LIB(libPath.c_str());
    if(!shaderLib)
    {
        spdlog::error("fail to load dynamic lib: {}", libPath);
        return -1;
    }

    UpdateUniformsFunc UpdateUniforms = (UpdateUniformsFunc)GET_FUNC(shaderLib, UPDATE_UNIFORM_FUNC_NAME);
    if(!UpdateUniforms)
    {
        spdlog::error("fail to find function in dynamic lib: {}", UPDATE_UNIFORM_FUNC_NAME);
        CLOSE_LIB(shaderLib);
        return -1;
    }

    InitUniformsFunc InitUniforms = (InitUniformsFunc)GET_FUNC(shaderLib, INIT_UNIFORM_FUNC_NAME);
    if(!InitUniforms)
    {
        spdlog::error("fail to find function in dynamic lib: {}", INIT_UNIFORM_FUNC_NAME);
        CLOSE_LIB(shaderLib);
        return -1;
    }

    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    InitWindow(100, 100, "Image Viewer");

    spdlog::debug("image path: {}", argv[1]);
    Image img = LoadImage(argv[1]);
    AdjustWindow(img);
    ClearWindowState(FLAG_WINDOW_HIDDEN);

    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);
    SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);
    SetTargetFPS(60);

    Shader shader = LoadShader(NULL, fragmentShaderPath.c_str());

    InitUniforms(shader, CONFIG_PATH);

    while (!WindowShouldClose())
    {
        //缩放图片使其放置在窗口内
        float screenWidth = (float)GetScreenWidth();
        float screenHeight = (float)GetScreenHeight();
        Rectangle source{0.0f, 0.0f, (float)texture.width, (float)texture.height};
        Rectangle dest{0.0f, 0.0f, screenWidth, screenHeight};
        Vector2 orgin{0.0f, 0.0f};

        UpdateUniforms(shader);

        BeginDrawing();
            ClearBackground(BLACK);

            BeginShaderMode(shader);
                DrawTexturePro(texture, source, dest, orgin, 0.0f, WHITE);
            EndShaderMode();
        EndDrawing();
    }

    UnloadTexture(texture);
    UnloadShader(shader);

    CLOSE_LIB(shaderLib);
    CloseWindow();

    return 0;
}

bool LoadConfig(const char *configFilePath)
{
    const char *SHADER_CONFIG_PREFIX = "shader";
    const char *FRAGMENT_SHADER = "fs";
    const char* LIB_PATH_PREFIX = "extend";

    try
    {
        tbl = toml::parse_file(configFilePath);
        if (auto shader = tbl[SHADER_CONFIG_PREFIX].as_table())
        {
            fragmentShaderPath = shader->get(FRAGMENT_SHADER)->as_string()->get();
            libPath = shader->get(LIB_PATH_PREFIX)->as_string()->get();
        }
    }
    catch (const toml::parse_error &err)
    {
        spdlog::error("fail to load config file: {}", configFilePath);
        return false;
    }
    return true;
}

void AdjustWindow(Image img)
{
    int monitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);
    
    const float scaleFactor = 0.8;

    enum class ImgType 
    {
        TooHeight, TooWidth, Fit
    };
    ImgType imgType;
    int newWidth, newHeight;

    float ratio = (float)img.height / (float)img.width;
    int maxLimit = (ratio>=0 ? monitorHeight : monitorWidth) * scaleFactor;
    int maxDimension = ratio>=0 ? img.height : img.width;

    if(maxDimension > maxLimit)
        imgType = ratio>=0 ? ImgType::TooHeight : ImgType::TooWidth;   
    else
        imgType = ImgType::Fit;
    
    switch (imgType) {
        case ImgType::TooHeight:
            newHeight = maxLimit;
            newWidth = maxLimit/ratio;
            break;
        case ImgType::TooWidth:
            newWidth = maxLimit;
            newHeight = maxLimit*ratio;
        case ImgType::Fit:
            newWidth = img.width;
            newHeight = img.height;
    }

    SetWindowSize(newWidth, newHeight);

    //居中显示
    int newX, newY;
    newX = (monitorWidth - newWidth)/2;
    newY = (monitorHeight - newHeight)/2;
    SetWindowPosition(newX, newY);
}
