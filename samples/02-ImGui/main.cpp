#include <imgui.h>
#include <graphics/Window.hpp>

constexpr int         SCREEN_WIDTH  = 1280;
constexpr int         SCREEN_HEIGHT = 720;
constexpr const char* TITLE         = "02 - ImGui";

using namespace rast;

int main()
{
    Window window( TITLE, SCREEN_WIDTH, SCREEN_HEIGHT );

    while ( window )
    {
        SDL_Event e;
        while ( SDL_PollEvent( &e ) )
        {
            switch ( e.type )
            {
            case SDL_EVENT_QUIT:
                window.close();
                break;
            case SDL_EVENT_KEY_DOWN:
                switch ( e.key.key )
                {
                case SDLK_ESCAPE:
                    window.close();
                    break;
                case SDLK_V:
                    window.toggleVSync();
                    break;
                case SDLK_RETURN:
                    if ((e.key.mod & SDL_KMOD_ALT) != 0)
                    {
                    case SDLK_F11:
                        window.toggleFullscreen();
                    }
                    break;
                }
                break;
            }
        }  // while SDL_PollEvent

        ImGui::ShowDemoWindow();

        window.clear( 154, 206, 235 );
        window.preset();

    }  // while (running)

    return 0;
}