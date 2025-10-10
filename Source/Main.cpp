#include "Framework.h"

#include "Engine/Application.h"

int main()
{
    try
    {
        g_App.Initialize();
        g_App.Run();
        g_App.Deinitialize();
    }
    catch (const std::exception& ex)
    {
        Log(ex.what(), LogLevel::Critical);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Critical Error", ex.what(), nullptr);
        return EXIT_FAILURE;
    }

    DeinitializeDebug();
    return EXIT_SUCCESS;
}
