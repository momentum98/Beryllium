#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <types.h>

typedef enum
{
    MOVE_BACKWARD_ACTION = 1,
    MOVE_FORWARD_ACTION     ,
    MOVE_LEFT_ACTION        ,
    MOVE_RIGHT_ACTION       ,
    NONE_ACTION         
} ActionType;

typedef struct
{
    ActionType type;
    bool       active;
} ActionState;

typedef struct
{
    i32          key;
    ActionState* actionState;
} Keybind;

typedef struct
{
    Keybind*    keybinds;
    u32         keybindCount;

    ActionState actionStates[NONE_ACTION - 1];
} InputManager;

void IM_UpdateActionStates(InputManager* const inputManager, const int key, const bool down);
void IM_ShutdownInputManager(InputManager* const inputManager);
u32 IM_SetupInputManager(InputManager* const inputManager);

#endif