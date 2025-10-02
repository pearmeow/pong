/**
 * Author: Perry Huang
 * Assignment: Pong Clone
 * Date due: 2025-10-13, 11:59pm
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 */

#include "raylib.h"

// Enums
enum AppStatus { TERMINATED, RUNNING };
enum Direction { LEFT, RIGHT };

// Global Constants
constexpr int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 450, FPS = 60, SIDES = 4;

constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0};
constexpr char ISAAC_FP[] = "./assets/isaac.png";
constexpr char ROOM_FP[] = "./assets/sac_room.png";
constexpr char BOMB_FP[] = "./assets/troll_bomb.png";

// Global Variables
AppStatus gAppStatus = RUNNING;
Texture2D gIsaac;
Vector2 gIsaacScale = {500.0f, 500.0f};
Vector2 gIsaacPos = ORIGIN;
Texture2D gRoom;
Texture2D gBomb;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello raylib!");

    gIsaac = LoadTexture(ISAAC_FP);
    gRoom = LoadTexture(ROOM_FP);
    gBomb = LoadTexture(BOMB_FP);

    SetTargetFPS(FPS);
}

void processInput() {
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
}

void render() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    Rectangle isaacArea = {0.0f, 0.0f, static_cast<float>(gIsaac.width), static_cast<float>(gIsaac.height)};
    Rectangle roomArea = {0.0f, 0.0f, static_cast<float>(gRoom.width), static_cast<float>(gRoom.height)};
    Rectangle bombArea = {0.0f, 0.0f, static_cast<float>(gBomb.width), static_cast<float>(gBomb.height)};

    Rectangle isaacDest = {gIsaacPos.x, gIsaacPos.y, static_cast<float>(gIsaacScale.x),
                           static_cast<float>(gIsaacScale.y)};
    Vector2 isaacOrigin = {static_cast<float>(gIsaacScale.x) / 2.0f, static_cast<float>(gIsaacScale.y) / 2.0f};

    // Draw something
    DrawTexturePro(gIsaac, isaacArea, isaacDest, isaacOrigin, 0.0f, WHITE);

    EndDrawing();
}

void shutdown() {
    CloseWindow();  // Close window and OpenGL context
    UnloadTexture(gIsaac);
    UnloadTexture(gRoom);
    UnloadTexture(gBomb);
}

int main(void) {
    initialise();

    while (gAppStatus == RUNNING) {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}
