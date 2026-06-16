#include "raylib.h"

struct Player {
    Vector2 position;
    int width;
    int height;
    int velocity;
};

struct Bullet {
    Vector2 position;
    int width;
    int height;
    int velocity;
    int active;
};

void update_player(struct Player *player, int screenWidth) {
    if (IsKeyDown(KEY_RIGHT)) player -> position.x += player->velocity;
    if (IsKeyDown(KEY_LEFT)) player -> position.x -= player->velocity;

    if (player->position.x < 0) {
        player->position.x = screenWidth - player->width;
    } else if (player->position.x > screenWidth - player->width) {
        player->position.x = 0;
    }
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 400;
    const char *text = "Welcome Invader";
    float shootTimer = 0;
    struct Bullet bullets[10] = {0};

    struct Player player = {
        {0, 0},
        20,
        20,
        2
    };

    
    InitWindow(screenWidth, screenHeight, "Space Invaders");
    
    SetTargetFPS(60);

    int textWidth = MeasureText(text, 16);
    player.position.x = screenWidth / 2.0f - player.width / 2.0f;
    player.position.y = screenHeight;

    while(!WindowShouldClose()) {
        if (player.position.y > 350) {
            player.position.y -= player.velocity;
        } else {
            update_player(&player, screenWidth);
        }

        shootTimer += GetFrameTime();

        if (shootTimer > 1) {
            for (int i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++) {
                if (bullets[i].active == 0) {
                    bullets[i].position.x = player.position.x + 8;
                    bullets[i].position.y = player.position.y - 20;
                    bullets[i].width = 5;
                    bullets[i].height = 20;
                    bullets[i].velocity = 1;
                    bullets[i].active = 1;
                    break;
                }
            }
            
            shootTimer = 0;
        }

        for (int i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++) {
            if (bullets[i].active == 1) {

                if (bullets[i].position.y > 0) {
                    bullets[i].position.y -= bullets[i].velocity;
                } else {
                    bullets[i].active = 0;
                }
            }
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText(text, screenWidth / 2 - textWidth / 2, 200, 16, BLACK);
            
            DrawRectangle(player.position.x, player.position.y, player.width, player.height, BLUE);

            for (int i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++) {
                if (bullets[i].active == 1) {
                    DrawRectangle(bullets[i].position.x, bullets[i].position.y, bullets[i].width, bullets[i].height, GREEN);
                }
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
