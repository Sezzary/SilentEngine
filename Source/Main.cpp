#include "Framework.h"

#include "Application.h"

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
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Critical Error", ex.what(), nullptr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
