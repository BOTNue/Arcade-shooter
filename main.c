#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT  800
#define PLAYER_SIZE 20.0f
#define MAX_BULLETS 10

typedef struct {
    Vector2 position;
    Vector2 velocity;
    float acceleration;
    float rotation;
    float move_speed;
} Player;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    bool lifetime;
} Bullet;

void Shoot_bullet(Bullet *bullet, Player player);

int main() 
{
    int ship_height = (PLAYER_SIZE/2)/tanf(20*DEG2RAD);

    Player player = {
        {SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - ship_height/2},
        {0, 0},
        0.0f,
        0.0f,
        5.0f
    };

    Bullet bullet [MAX_BULLETS];

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Arcade-Shooter");
    SetTargetFPS(60);
    while (!WindowShouldClose()) 
    {

        if (IsKeyDown(KEY_LEFT)) 
        {
            player.rotation -= 5;
        }
        if (IsKeyDown(KEY_RIGHT)) 
        {
            player.rotation += 5;
        }
        player.velocity.x = sin(player.rotation*DEG2RAD)*player.move_speed;
        player.velocity.y = cos(player.rotation*DEG2RAD)*player.move_speed;

        if (IsKeyDown(KEY_UP)) 
        {
            if (player.acceleration < 1)
            {
                player.acceleration += 0.04f;
            }
        }
        else 
        {
            if (player.acceleration > 0)
            {
                player.acceleration -= 0.02f;
            }
            else if (player.acceleration < 0)
            {
                player.acceleration = 0;
            }
            
        }

        if (IsKeyDown(KEY_DOWN)) 
        {
            if (player.acceleration > 0)
            {
                player.acceleration -= 0.04f;
            }
            else if (player.acceleration < 0)
            {
                player.acceleration = 0;
            }
            
        }

        player.position.x += (player.velocity.x*player.acceleration);
        player.position.y -= (player.velocity.y*player.acceleration);

        if (IsKeyDown(KEY_SPACE))
        {
            Shoot_bullet(bullet, player);
        }

        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            if (bullet[i].lifetime)
            {
                bullet[i].position.x += bullet[i].velocity.x;
                bullet[i].position.y -= bullet[i].velocity.y;

                if (bullet[i].position.x < 0 || bullet[i].position.x > SCREEN_WIDTH)
                {
                    bullet[i].lifetime = false;
                }
                if (bullet[i].position.y < 0 || bullet[i].position.y > SCREEN_HEIGHT)
                {
                    bullet[i].lifetime = false;
                }
            }
        }

        if (player.position.x > SCREEN_WIDTH)
        {
            player.position.x = 0;
        }
        if (player.position.x < 0)
        {
            player.position.x = SCREEN_WIDTH;
        }
        if (player.position.y > SCREEN_HEIGHT)
        {
            player.position.y = 0;
        }
        if (player.position.y < 0)
        {
            player.position.y = SCREEN_HEIGHT;
        }

        Vector2 v1 = {player.position.x + sinf(player.rotation*DEG2RAD)*(ship_height), player.position.y - cosf(player.rotation*DEG2RAD)*(ship_height)};
        Vector2 v2 = {player.position.x - cosf(player.rotation*DEG2RAD)*(PLAYER_SIZE/2), player.position.y - sinf(player.rotation*DEG2RAD)*(PLAYER_SIZE/2)};
        Vector2 v3 = {player.position.x + cosf(player.rotation*DEG2RAD)*(PLAYER_SIZE/2), player.position.y - sinf(player.rotation*DEG2RAD)*(PLAYER_SIZE/2)};

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTriangle(v1, v2, v3, WHITE);

        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            if (bullet[i].lifetime)
            {
                DrawCircleV(bullet[i].position, 5, WHITE);
            }
        }

        EndDrawing();
    }
}

void Shoot_bullet (Bullet *bullet, Player player)
{
    for (int i = 0; i < MAX_BULLETS; ++i)
    {
        if (!bullet[i].lifetime)
        {
            bullet[i].position = player.position;
            bullet[i].velocity.x = sin(player.rotation * DEG2RAD) * 10;
            bullet[i].velocity.y = cos(player.rotation * DEG2RAD) * 10;
            bullet[i].lifetime = true;
            break;
        }
    }
}