#include "raylib.h"

int main(void)
{
    InitWindow(800, 450, "Aetheria");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello World", 300, 200, 40, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}