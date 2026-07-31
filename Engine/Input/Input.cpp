#pragma once

#include "raylib.h"


/*
Usage:
Input input;
if (input.keyDown(KEY_D)) {//move right}
if (input.mousePressed(MOUSE_BUTTON_LEFT)) {Vector2 pos = input.mousePosition();}
*/
class Input {
public:
    // Teclado
    bool keyDown(int key) const {return IsKeyDown(key);}
    bool keyUp(int key) const {return IsKeyUp(key);}
    bool keyPressed(int key) const {return IsKeyPressed(key);}
    bool keyReleased(int key) const {return IsKeyReleased(key);}

    // Mouse
    bool mouseDown(int button) const {return IsMouseButtonDown(button);}
    bool mouseUp(int button) const {return IsMouseButtonUp(button);}
    bool mousePressed(int button) const {return IsMouseButtonPressed(button);}
    bool mouseReleased(int button) const {return IsMouseButtonReleased(button);}
    Vector2 mousePosition() const {return GetMousePosition();}
    Vector2 mouseDelta() const {return GetMouseDelta();}
    float mouseWheel() const {return GetMouseWheelMove();}

    // Controle
    bool gamepadAvailable(int gamepad) const {return IsGamepadAvailable(gamepad);}
    bool gamepadButtonDown(int gamepad, int button) const {return IsGamepadButtonDown(gamepad, button);}
    bool gamepadButtonPressed(int gamepad, int button) const {return IsGamepadButtonPressed(gamepad, button);}
    bool gamepadButtonReleased(int gamepad, int button) const {return IsGamepadButtonReleased(gamepad, button);}
    float gamepadAxis(int gamepad, int axis) const {return GetGamepadAxisMovement(gamepad, axis);}
};