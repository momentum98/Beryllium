#include "input_manager.h"
#include "SDL_keycode.h"

#include <core/memory/memory.h>
#include <logger/logger.h>

#define KEYBINDS_PER_ACTION 2

u32 IM_SetupInputManager(InputManager* const inputManager)
{
    inputManager->keybindCount = KEYBINDS_PER_ACTION * (NONE_ACTION - 1);
    inputManager->keybinds = M_MemAlloc(sizeof(Keybind) * inputManager->keybindCount);

    for (u32 i = 0; i < (NONE_ACTION - 1); ++i)
    {
        inputManager->actionStates[i].type = (ActionType) (i + 1);
        inputManager->actionStates[i].active = false;
    }

    if (inputManager->keybinds == NULL)
        return 1;

    for (u32 i = 0; i < inputManager->keybindCount; ++i)
    {
        inputManager->keybinds[i].actionState = NULL;
        inputManager->keybinds[i].key = 0;
    }

    inputManager->keybinds[0].actionState = &inputManager->actionStates[MOVE_FORWARD_ACTION - 1];
    inputManager->keybinds[0].key = SDLK_UP;

    inputManager->keybinds[1].actionState = &inputManager->actionStates[MOVE_LEFT_ACTION - 1];
    inputManager->keybinds[1].key = SDLK_LEFT;

    inputManager->keybinds[2].actionState = &inputManager->actionStates[MOVE_RIGHT_ACTION - 1];

    inputManager->keybinds[3].actionState = &inputManager->actionStates[MOVE_BACKWARD_ACTION - 1];
    inputManager->keybinds[3].key = SDLK_DOWN;

    return 0;
}

void IM_UpdateActionStates(InputManager* const inputManager, const i32 key, const bool down)
{
    for (u32 i = 0; i < inputManager->keybindCount; ++i)
    {
        Keybind* const keybind = &inputManager->keybinds[i];

        if (keybind->key == key)
        {
            if (keybind->actionState == NULL)
            {
                L_LogInfo("Keybind action state is invalid...", key, keybind->actionState->type);
                continue;
            }
            
            keybind->actionState->active = down;
            // L_LogInfo("Key down: \"%d\", Action ID: \"%u\"", key, keybind->actionState->type);
        }
    }
}

void IM_ShutdownInputManager(InputManager* const inputManager)
{
    if (inputManager->keybinds != NULL)
    {
        M_MemFree(inputManager->keybinds);
        inputManager->keybindCount = 0;
        inputManager->keybinds = NULL;
    }
}