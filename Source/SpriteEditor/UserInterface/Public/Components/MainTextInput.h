#pragma once

#include "AUI/TextInput.h"

namespace AM
{
namespace SpriteEditor
{

/**
 * The text input style used for the main screen.
 */
class MainTextInput : public AUI::TextInput
{
public:
    MainTextInput(AUI::Screen& screen, const char* key, const SDL_Rect& screenExtent);
};

} // End namespace SpriteEditor
} // End namespace AM
