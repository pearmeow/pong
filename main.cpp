/**
 * Author: Perry Huang
 * Assignment: Pong Clone
 * Date due: 2025-10-13, 11:59pm
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 */

#include <cmath>
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
constexpr float VELOCITY = 350.0f;
constexpr float BALL_VELOCITY = 175.0f;

// Global Variables
AppStatus gAppStatus = RUNNING;
Texture2D gIsaac;
Vector2 gIsaacScale = {70.0f, 80.0f};
Vector2 gIsaacPos = {SCREEN_WIDTH / 8.0f, SCREEN_HEIGHT / 2.0};
Direction gIsaacDirection = NONE;
Texture2D gAzazel;
Vector2 gAzazelScale = gIsaacScale;
Vector2 gAzazelPos = {7.0f * SCREEN_WIDTH / 8.0f, SCREEN_HEIGHT / 2.0};
Direction gAzazelDirection = UP;
Texture2D gRoom;
Vector2 gRoomScale = {SCREEN_WIDTH + 250.0f, SCREEN_HEIGHT + 250.0f};
Vector2 gRoomPos = ORIGIN;
Texture2D gBomb;
Vector2 gBombScale = {40.0f, 40.0f};
Vector2 gBombPos = ORIGIN;
Direction gBombXDirection = LEFT;
Direction gBombYDirection = UP;

bool gBomb2Active = false;
Texture2D gBomb2;
Vector2 gBombScale2 = {40.0f, 40.0f};
Vector2 gBombPos2 = ORIGIN;
Direction gBombXDirection2 = RIGHT;
Direction gBombYDirection2 = UP;

bool gBomb3Active = false;
Texture2D gBomb3;
Vector2 gBombScale3 = {40.0f, 40.0f};
Vector2 gBombPos3 = ORIGIN;
Direction gBombXDirection3 = LEFT;
Direction gBombYDirection3 = DOWN;

bool gGameOver = false;
bool gMultiplayer = false;
float gPreviousTicks = 0.0f;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
bool isColliding(const Vector2* positionA, const Vector2* scaleA, const Vector2* positionB, const Vector2* scaleB);

// Function Definitions
void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello raylib!");

    gIsaac = LoadTexture(ISAAC_FP);
    gAzazel = LoadTexture(AZAZEL_FP);
    gRoom = LoadTexture(ROOM_FP);
    gBomb = LoadTexture(BOMB_FP);
    gBomb2 = LoadTexture(BOMB_FP);
    gBomb3 = LoadTexture(BOMB_FP);

    SetTargetFPS(FPS);
}

void processInput() {
    if (WindowShouldClose()) gAppStatus = TERMINATED;
    if (IsKeyPressed(KEY_T)) {
        gMultiplayer = !gMultiplayer;
    }
    if (IsKeyPressed(KEY_R)) {
        gBombPos = ORIGIN;
        gBombPos2 = ORIGIN;
        gBombPos3 = ORIGIN;
        gGameOver = false;
        gIsaacPos = {SCREEN_WIDTH / 8.0f, SCREEN_HEIGHT / 2.0};
        gAzazelPos = {7.0f * SCREEN_WIDTH / 8.0f, SCREEN_HEIGHT / 2.0};
        // and reset the time so azazel doesn't teleport!
        gPreviousTicks = (float)GetTime();
    }
    if (gGameOver) {
        return;
    }
    if (IsKeyPressed(KEY_ONE)) {
        gBomb2Active = false;
        gBomb3Active = false;
    }
    if (IsKeyPressed(KEY_TWO)) {
        if (!gBomb2Active) {
            gBombPos2 = ORIGIN;
        }
        gBomb2Active = true;
        gBomb3Active = false;
    }
    if (IsKeyPressed(KEY_THREE)) {
        if (!gBomb2Active) {
            gBombPos2 = ORIGIN;
        }
        if (!gBomb3Active) {
            gBombPos3 = ORIGIN;
        }
        gBomb2Active = true;
        gBomb3Active = true;
    }
    if (IsKeyDown(KEY_W) && gIsaacPos.y - gIsaacScale.y / 2.0 > 0) {
        gIsaacDirection = UP;
    } else if (IsKeyDown(KEY_S) && gIsaacPos.y + gIsaacScale.y / 2.0 < SCREEN_HEIGHT) {
        gIsaacDirection = DOWN;
    } else {
        gIsaacDirection = NONE;
    }

    if (gMultiplayer) {
        if (IsKeyDown(KEY_K) && gAzazelPos.y - gAzazelScale.y / 2.0 > 0) {
            gAzazelDirection = UP;
        } else if (IsKeyDown(KEY_J) && gAzazelPos.y + gAzazelScale.y / 2.0 < SCREEN_HEIGHT) {
            gAzazelDirection = DOWN;
        } else {
            gAzazelDirection = NONE;
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
    if (gGameOver) {
        return;
    }
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

    if (gBomb2Active) {
        if (gBombPos2.y - gBombScale2.y / 2.0 < 0) {
            gBombYDirection2 = DOWN;
        } else if (gBombPos2.y + gBombScale2.y / 2.0 > SCREEN_HEIGHT) {
            gBombYDirection2 = UP;
        }
    }

    if (gBomb3Active) {
        if (gBombPos3.y - gBombScale3.y / 2.0 < 0) {
            gBombYDirection3 = DOWN;
        } else if (gBombPos3.y + gBombScale3.y / 2.0 > SCREEN_HEIGHT) {
            gBombYDirection3 = UP;
        }
    }

    // if ball is colliding with left or right
    // end game
    if (gBombPos.x < 0 || gBombPos2.x < 0 || gBombPos3.x < 0) {
        printf("Azazel wins\n");
        gGameOver = true;
        // set some texture to display azazel wins
    } else if (gBombPos.x > SCREEN_WIDTH || gBombPos2.x > SCREEN_WIDTH || gBombPos3.x > SCREEN_WIDTH) {
        printf("Isaac wins\n");
        gGameOver = true;
        // set some texture to display isaac wins
    }

    if (isColliding(&gBombPos, &gBombScale, &gIsaacPos, &gIsaacScale) ||
        isColliding(&gBombPos, &gBombScale, &gAzazelPos, &gAzazelScale)) {
        if (gBombXDirection == LEFT) {
            gBombXDirection = RIGHT;
        } else {
            gBombXDirection = LEFT;
        }
    }

    if (gBomb2Active) {
        if (isColliding(&gBombPos2, &gBombScale2, &gIsaacPos, &gIsaacScale) ||
            isColliding(&gBombPos2, &gBombScale2, &gAzazelPos, &gAzazelScale)) {
            if (gBombXDirection2 == LEFT) {
                gBombXDirection2 = RIGHT;
            } else {
                gBombXDirection2 = LEFT;
            }
        }
    }

    if (gBomb3Active) {
        if (isColliding(&gBombPos3, &gBombScale3, &gIsaacPos, &gIsaacScale) ||
            isColliding(&gBombPos3, &gBombScale3, &gAzazelPos, &gAzazelScale)) {
            if (gBombXDirection3 == LEFT) {
                gBombXDirection3 = RIGHT;
            } else {
                gBombXDirection3 = LEFT;
            }
        }
    }

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

    if (gBomb2Active) {
        if (gBombXDirection2 == LEFT) {
            gBombPos2.x -= BALL_VELOCITY * deltaTime;
        } else {
            gBombPos2.x += BALL_VELOCITY * deltaTime;
        }
        if (gBombYDirection2 == UP) {
            gBombPos2.y -= BALL_VELOCITY * deltaTime;
        } else {
            gBombPos2.y += BALL_VELOCITY * deltaTime;
        }
    }

    if (gBomb3Active) {
        if (gBombXDirection3 == LEFT) {
            gBombPos3.x -= BALL_VELOCITY * deltaTime;
        } else {
            gBombPos3.x += BALL_VELOCITY * deltaTime;
        }
        if (gBombYDirection3 == UP) {
            gBombPos3.y -= BALL_VELOCITY * deltaTime;
        } else {
            gBombPos3.y += BALL_VELOCITY * deltaTime;
        }
    }
}

void render() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // if the game is over, render a texture and return early

    Rectangle isaacArea = {0.0f, 0.0f, static_cast<float>(gIsaac.width), static_cast<float>(gIsaac.height)};
    Rectangle isaacDest = {gIsaacPos.x, gIsaacPos.y, static_cast<float>(gIsaacScale.x),
                           static_cast<float>(gIsaacScale.y)};
    Vector2 isaacOrigin = {static_cast<float>(gIsaacScale.x) / 2.0f, static_cast<float>(gIsaacScale.y) / 2.0f};

    Rectangle azazelArea = {0.0f, 0.0f, static_cast<float>(gAzazel.width), static_cast<float>(gAzazel.height)};
    Rectangle azazelDest = {gAzazelPos.x, gAzazelPos.y, static_cast<float>(gAzazelScale.x),
                            static_cast<float>(gAzazelScale.y)};
    Vector2 azazelOrigin = {static_cast<float>(gAzazelScale.x) / 2.0f, static_cast<float>(gAzazelScale.y) / 2.0f};

    Rectangle roomArea = {0.0f, 0.0f, static_cast<float>(gRoom.width), static_cast<float>(gRoom.height)};
    Rectangle roomDest = {gRoomPos.x, gRoomPos.y, static_cast<float>(gRoomScale.x),
                          static_cast<float>(gRoomScale.y)};
    Vector2 roomOrigin = {static_cast<float>(gRoomScale.x) / 2.0f, static_cast<float>(gRoomScale.y) / 2.0f};

    Rectangle bombArea = {0.0f, 0.0f, static_cast<float>(gBomb.width), static_cast<float>(gBomb.height)};
    Rectangle bombDest = {gBombPos.x, gBombPos.y, static_cast<float>(gBombScale.x),
                          static_cast<float>(gBombScale.y)};
    Vector2 bombOrigin = {static_cast<float>(gBombScale.x) / 2.0f, static_cast<float>(gBombScale.y) / 2.0f};

    Rectangle bombArea2 = {0.0f, 0.0f, static_cast<float>(gBomb.width), static_cast<float>(gBomb.height)};
    Rectangle bombDest2 = {gBombPos2.x, gBombPos2.y, static_cast<float>(gBombScale2.x),
                           static_cast<float>(gBombScale2.y)};
    Vector2 bombOrigin2 = {static_cast<float>(gBombScale2.x) / 2.0f, static_cast<float>(gBombScale2.y) / 2.0f};

    Rectangle bombArea3 = {0.0f, 0.0f, static_cast<float>(gBomb.width), static_cast<float>(gBomb.height)};
    Rectangle bombDest3 = {gBombPos3.x, gBombPos3.y, static_cast<float>(gBombScale3.x),
                           static_cast<float>(gBombScale3.y)};
    Vector2 bombOrigin3 = {static_cast<float>(gBombScale3.x) / 2.0f, static_cast<float>(gBombScale3.y) / 2.0f};

    // Draw stuff in order
    DrawTexturePro(gRoom, roomArea, roomDest, roomOrigin, 0.0f, WHITE);
    DrawTexturePro(gIsaac, isaacArea, isaacDest, isaacOrigin, 0.0f, WHITE);
    DrawTexturePro(gAzazel, azazelArea, azazelDest, azazelOrigin, 0.0f, WHITE);
    DrawTexturePro(gBomb, bombArea, bombDest, bombOrigin, 0.0f, WHITE);
    if (gBomb2Active) {
        DrawTexturePro(gBomb2, bombArea2, bombDest2, bombOrigin2, 0.0f, WHITE);
    }
    if (gBomb3Active) {
        DrawTexturePro(gBomb3, bombArea3, bombDest3, bombOrigin3, 0.0f, WHITE);
    }

    EndDrawing();
}

void shutdown() {
    CloseWindow();  // Close window and OpenGL context
    UnloadTexture(gIsaac);
    UnloadTexture(gAzazel);
    UnloadTexture(gRoom);
    UnloadTexture(gBomb);
}

bool isColliding(const Vector2* positionA, const Vector2* scaleA, const Vector2* positionB,
                 const Vector2* scaleB) {
    float xDistance = fabs(positionA->x - positionB->x) - ((scaleA->x + scaleB->x) / 2.0f);
    float yDistance = fabs(positionA->y - positionB->y) - ((scaleA->y + scaleB->y) / 2.0f);
    if (xDistance < 0.0f && yDistance < 0.0f) {
        return true;
    }
    return false;
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
