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
enum Direction { UP, DOWN };

// Global Constants
constexpr int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 450, FPS = 60, SIDES = 4;

constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0};
constexpr char ISAAC_FP[] = "./assets/isaac.png";
constexpr char AZAZEL_FP[] = "./assets/azazel.png";
constexpr char ROOM_FP[] = "./assets/sac_room.png";
constexpr char BOMB_FP[] = "./assets/troll_bomb.png";
constexpr float VELOCITY = 6.0f;

// Global Variables
AppStatus gAppStatus = RUNNING;
Direction azazelDirection = UP;
Texture2D gIsaac;
Vector2 gIsaacScale = {100.0f, 110.0f};
Vector2 gIsaacPos = {SCREEN_WIDTH / 5.0f, SCREEN_HEIGHT / 2.0};
Texture2D gAzazel;
Vector2 gAzazelScale = gIsaacScale;
Vector2 gAzazelPos = {4.0f * SCREEN_WIDTH / 5.0f, SCREEN_HEIGHT / 2.0};
Texture2D gRoom;
Vector2 gRoomScale = {SCREEN_WIDTH, SCREEN_HEIGHT};
Vector2 gRoomPos = ORIGIN;
Texture2D gBomb;
Vector2 gBombScale = {50.0f, 50.0f};
Vector2 gBombPos = ORIGIN;

bool gMultiplayer = false;

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
    gAzazel = LoadTexture(AZAZEL_FP);
    gRoom = LoadTexture(ROOM_FP);
    gBomb = LoadTexture(BOMB_FP);

    SetTargetFPS(FPS);
}

void processInput() {
    if (WindowShouldClose()) gAppStatus = TERMINATED;
    if (IsKeyPressed(KEY_T)) {
        gMultiplayer = !gMultiplayer;
    }

    if (IsKeyDown(KEY_W) && gIsaacPos.y - gIsaacScale.y / 2.0 > 0) {
        gIsaacPos.y -= VELOCITY;
    } else if (IsKeyDown(KEY_S) && gIsaacPos.y + gIsaacScale.y / 2.0 < SCREEN_HEIGHT) {
        gIsaacPos.y += VELOCITY;
    }

    if (gMultiplayer) {
        if (IsKeyDown(KEY_UP) && gAzazelPos.y - gAzazelScale.y / 2.0 > 0) {
            gAzazelPos.y -= VELOCITY;
        } else if (IsKeyDown(KEY_DOWN) && gAzazelPos.y + gAzazelScale.y / 2.0 < SCREEN_HEIGHT) {
            gAzazelPos.y += VELOCITY;
        }
    } else {
        if (gAzazelPos.y - gAzazelScale.y / 2.0 < 0) {
            azazelDirection = DOWN;
        } else if (gAzazelPos.y + gAzazelScale.y / 2.0 > SCREEN_HEIGHT) {
            azazelDirection = UP;
        }
        if (azazelDirection == UP) {
            gAzazelPos.y -= VELOCITY;
        } else {
            gAzazelPos.y += VELOCITY;
        }
    }
}

void update() {
}

void render() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    Rectangle isaacArea = {0.0f, 0.0f, static_cast<float>(gIsaac.width), static_cast<float>(gIsaac.height)};
    Rectangle azazelArea = {0.0f, 0.0f, static_cast<float>(gAzazel.width), static_cast<float>(gAzazel.height)};
    Rectangle roomArea = {0.0f, 0.0f, static_cast<float>(gRoom.width), static_cast<float>(gRoom.height)};
    Rectangle bombArea = {0.0f, 0.0f, static_cast<float>(gBomb.width), static_cast<float>(gBomb.height)};

    Rectangle isaacDest = {gIsaacPos.x, gIsaacPos.y, static_cast<float>(gIsaacScale.x),
                           static_cast<float>(gIsaacScale.y)};
    Rectangle azazelDest = {gAzazelPos.x, gAzazelPos.y, static_cast<float>(gAzazelScale.x),
                            static_cast<float>(gAzazelScale.y)};
    Rectangle roomDest = {gRoomPos.x, gRoomPos.y, static_cast<float>(gRoomScale.x),
                          static_cast<float>(gRoomScale.y)};
    Rectangle bombDest = {gBombPos.x, gBombPos.y, static_cast<float>(gBombScale.x),
                          static_cast<float>(gBombScale.y)};

    Vector2 isaacOrigin = {static_cast<float>(gIsaacScale.x) / 2.0f, static_cast<float>(gIsaacScale.y) / 2.0f};
    Vector2 azazelOrigin = {static_cast<float>(gAzazelScale.x) / 2.0f, static_cast<float>(gAzazelScale.y) / 2.0f};
    Vector2 roomOrigin = {static_cast<float>(gRoomScale.x) / 2.0f, static_cast<float>(gRoomScale.y) / 2.0f};
    Vector2 bombOrigin = {static_cast<float>(gBombScale.x) / 2.0f, static_cast<float>(gBombScale.y) / 2.0f};

    // Draw something
    DrawTexturePro(gRoom, roomArea, roomDest, roomOrigin, 0.0f, WHITE);
    DrawTexturePro(gIsaac, isaacArea, isaacDest, isaacOrigin, 0.0f, WHITE);
    DrawTexturePro(gAzazel, azazelArea, azazelDest, azazelOrigin, 0.0f, WHITE);
    DrawTexturePro(gBomb, bombArea, bombDest, bombOrigin, 0.0f, WHITE);

    EndDrawing();
}

void shutdown() {
    CloseWindow();  // Close window and OpenGL context
    UnloadTexture(gIsaac);
    UnloadTexture(gAzazel);
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
