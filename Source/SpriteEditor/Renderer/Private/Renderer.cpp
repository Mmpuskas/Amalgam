#include "Renderer.h"
#include "UserInterface.h"
#include "Log.h"
#include "Ignore.h"

#include <SDL2/SDL2_gfxPrimitives.h>

namespace AM
{
namespace SpriteEditor
{
Renderer::Renderer(SDL2pp::Renderer& inSdlRenderer, SDL2pp::Window& window, UserInterface& inUI)
: sdlRenderer(inSdlRenderer)
, ui(inUI)
{
    // TODO: This will eventually be used when we get to variable window sizes.
    ignore(window);
}

void Renderer::render()
{
    /* Render. */
    // Clear the current rendering target to prepare for rendering.
    sdlRenderer.Clear();

    // Render the current UI screen.
    ui.currentScreen->render();

    // Present the finished back buffer to the user's screen.
    sdlRenderer.Present();
}

bool Renderer::handleEvent(SDL_Event& event)
{
    switch (event.type) {
        case SDL_WINDOWEVENT:
            // TODO: Handle this.
            return true;
    }

    return false;
}

} // namespace SpriteEditor
} // namespace AM
