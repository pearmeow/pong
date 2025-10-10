/**
 * Author: Perry Huang
 * Assignment: Pong Clone
 * Date due: 2025-10-13, 11:59pm
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 */

#include <cstdio>

#include "raylib.h"

// Enums
enum AppStatus { TERMINATED, RUNNING };
enum Direction { NONE, UP, DOWN, LEFT, RIGHT };

// Global Constants
constexpr int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 450, FPS = 60, SIDES = 4;

constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0};
constexpr char ISAAC_FP[] = "./assets/isaac.png";
constexpr char AZAZEL_FP[] = "./assets/azazel.png";
constexpr char ROOM_FP[] = "./assets/sac_room.png";
constexpr char BOMB_FP[] = "./assets/troll_bomb.png";
constexpr float VELOCITY = 300.0f;
constexpr float BALL_VELOCITY = 175.0f;

// Global Variables
AppStatus gAppStatus = RUNNING;
Texture2D gIsaac;
Vector2 gIsaacScale = {70.0f, 80.0f};
Vector2 gIsaacPos = {SCREEN_WIDTH / 7.0f, SCREEN_HEIGHT / 2.0};
Direction gIsaacDirection = NONE;
Texture2D gAzazel;
Vector2 gAzazelScale = gIsaacScale;
Vector2 gAzazelPos = {6.0f * SCREEN_WIDTH / 7.0f, SCREEN_HEIGHT / 2.0};
Direction gAzazelDirection = UP;
Texture2D gRoom;
Vector2 gRoomScale = {SCREEN_WIDTH + 250.0f, SCREEN_HEIGHT + 250.0f};
Vector2 gRoomPos = ORIGIN;
Texture2D gBomb;
Vector2 gBombScale = {40.0f, 40.0f};
Vector2 gBombPos = ORIGIN;
Direction gBombXDirection = LEFT;
Direction gBombYDirection = UP;

bool gMultiplayer = false;
float gPreviousTicks = 0.0f;

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
    // maybe add "R" as a key to reset game won status
    // if the game is over, return early to stop inputs
    if (IsKeyPressed(KEY_T)) {
        gMultiplayer = !gMultiplayer;
    }
    gIsaacDirection = NONE;
    if (IsKeyDown(KEY_W) && gIsaacPos.y - gIsaacScale.y / 2.0 > 0) {
        gIsaacDirection = UP;
    } else if (IsKeyDown(KEY_S) && gIsaacPos.y + gIsaacScale.y / 2.0 < SCREEN_HEIGHT) {
        gIsaacDirection = DOWN;
    }

    if (gMultiplayer) {
        gAzazelDirection = NONE;
        if (IsKeyDown(KEY_UP) && gAzazelPos.y - gAzazelScale.y / 2.0 > 0) {
            gAzazelDirection = UP;
        } else if (IsKeyDown(KEY_DOWN) && gAzazelPos.y + gAzazelScale.y / 2.0 < SCREEN_HEIGHT) {
            gAzazelDirection = DOWN;
        }
    } else {
        if (gAzazelPos.y - gAzazelScale.y / 2.0 < 0) {
            gAzazelDirection = DOWN;
        } else if (gAzazelPos.y + gAzazelScale.y / 2.0 > SCREEN_HEIGHT) {
            gAzazelDirection = UP;
        }
    }
}

void update() {
    // if the game is over, return early because there are no more calculations

    float ticks = (float)GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    if (gIsaacDirection == UP) {
        gIsaacPos.y -= VELOCITY * deltaTime;
    } else if (gIsaacDirection == DOWN) {
        gIsaacPos.y += VELOCITY * deltaTime;
    }

    if (gAzazelDirection == UP) {
        gAzazelPos.y -= VELOCITY * deltaTime;
    } else if (gAzazelDirection == DOWN) {
        gAzazelPos.y += VELOCITY * deltaTime;
    }

    // if ball is colliding with top or bottom
    if (gBombPos.y - gBombScale.y / 2.0 < 0) {
        gBombYDirection = DOWN;
    } else if (gBombPos.y + gBombScale.y / 2.0 > SCREEN_HEIGHT) {
        gBombYDirection = UP;
    }

    // if ball is colliding with left or right
    // end game
    if (gBombPos.x < 0) {
        // azazel wins
        printf("Azazel wins\n");
        // and then put some text on the screen depending on who won
    } else if (gBombPos.x > SCREEN_WIDTH) {
        // isaac wins
        printf("Isaac wins\n");
        // and then put some text on the screen depending on who won
    }

    // if (true /* ball is colliding with players */) {
    //     if (gBombXDirection == LEFT) {
    //         gBombXDirection = RIGHT;
    //     } else {
    //         gBombXDirection = LEFT;
    //     }
    //     if (gBombYDirection == UP) {
    //         gBombYDirection = DOWN;
    //     } else {
    //         gBombYDirection = UP;
    //     }
    // }

    if (gBombXDirection == LEFT) {
        gBombPos.x -= BALL_VELOCITY * deltaTime;
    } else {
        gBombPos.x += BALL_VELOCITY * deltaTime;
    }
    if (gBombYDirection == UP) {
        gBombPos.y -= BALL_VELOCITY * deltaTime;
    } else {
        gBombPos.y += BALL_VELOCITY * deltaTime;
    }
}

void render() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // if the game is over, render a texture and return early

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

int main() {
    initialise();

    while (gAppStatus == RUNNING) {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}
