#include "UserInterface.h"
#include "Config.h"
#include "AUI/Core.h"
#include "Log.h"
#include "Ignore.h"
#include <SDL_filesystem.h>

namespace AM
{
namespace SpriteEditor
{
UserInterface::UserInterface(SDL_Renderer* renderer, SpriteDataModel& spriteDataModel)
: auiInitializer((std::string{SDL_GetBasePath()} + "Resources/"), renderer
              , {Config::LOGICAL_SCREEN_WIDTH, Config::LOGICAL_SCREEN_HEIGHT})
, currentScreen(&titleScreen)
, titleScreen(*this, spriteDataModel)
, mainScreen(spriteDataModel)
{
    AUI::Core::setActualScreenSize({Config::ACTUAL_SCREEN_WIDTH, Config::ACTUAL_SCREEN_HEIGHT});
}

void UserInterface::openTitleScreen()
{
    // Switch to the title screen.
    currentScreen = &titleScreen;
}

void UserInterface::openMainScreen()
{
    // Tell the main screen to load the current sprite data into its UI.
    mainScreen.loadSpriteData();

    // Switch to the main screen.
    currentScreen = &mainScreen;
}

bool UserInterface::handleEvent(SDL_Event& event)
{
    return currentScreen->handleEvent(event);
}

void UserInterface::tick(double timestepS)
{
    // Let AUI process the next tick.
    currentScreen->tick(timestepS);
}

} // End namespace SpriteEditor
} // End namespace AM
