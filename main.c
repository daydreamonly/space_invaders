#include "raylib.h"

typedef struct Player {
    Vector2 position;
    Vector2 size;
    int velocity;
}Player;

typedef struct Bullet {
    Vector2 position;
    Vector2 size;
    int velocity;
    int damage;
    int active;
}Bullet;

typedef struct Foe {
    Vector2 position;
    Vector2 size;
    int velocity;
    int health;
}Foe;

void update_player(Player *player, int screenWidth) {
    if (IsKeyDown(KEY_RIGHT)) player -> position.x += player->velocity;
    if (IsKeyDown(KEY_LEFT)) player -> position.x -= player->velocity;

    if (player->position.x < 0) {
        player->position.x = screenWidth - player->size.x;
    } else if (player->position.x > screenWidth - player->size.x) {
        player->position.x = 0;
    }
}

void update_foe(Foe *foe, int screenWidth) {
    foe -> position.x += foe->velocity;
    if (foe->position.x > screenWidth - foe->size.x){
        foe->position.x = 0;
    }
}

Rectangle RectangleFromVector2(Vector2 position, Vector2 size){
    Rectangle rect = {
        .x = position.x,
        .y = position.y,
        .width = size.x,
        .height = size.y
    };
    return rect;
};

void init_foes(Foe* a, int len, Vector2 cords, int margin, int foes_in_row){
    int items_in_row;
    if(len-1 > foes_in_row){
        items_in_row = foes_in_row;
    } else {
        items_in_row = len-1;
    }

    int foes_rec_width = items_in_row * (foes_in_row + margin) - margin;
    float centered = cords.x / 2.0f - foes_rec_width / 2.0f;
    int row = 0;

    for(int i = 0; i < len; i++){
        if(i % foes_in_row == 0 && i != 0) {
            row += 1;
        }
        a[i] = (Foe){
            .size.x = 20,
            .size.y = 20,
            .health = 20,
            .position.x = (foes_in_row + margin) * (i % foes_in_row) + centered,
            .position.y = 40 * row,
            .velocity = 1
        };
    }
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 400;
    //const char *text = "Welcome Invader";
    float shootTimer = 0;
    Bullet bullets[100] = {0};
    Foe foes[60];

    init_foes(foes, sizeof(foes) / sizeof(foes[0]), (Vector2){screenWidth, screenHeight}, 20, 15);

    Player player = {
        .position = {.x = 0, .y = 0},
        .size = {.x = 20, .y = 20},
        .velocity = 5
    };

    Foe target1 = {
        .position = {.x = 400, .y = 200},
        .size = {.x = 20, .y = 20},
        .velocity = 2,
        .health = 10
    };

    InitWindow(screenWidth, screenHeight, "Space Invaders");
    
    SetTargetFPS(60);

    player.position.x = screenWidth / 2.0f - player.size.x / 2.0f;
    player.position.y = screenHeight;

    

    while(!WindowShouldClose()) {
        if (player.position.y > 350) {
            player.position.y -= player.velocity;
        } else {
            update_player(&player, screenWidth);
        }

        shootTimer += GetFrameTime();

        if (shootTimer > 0.1) {
            for (int i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++) {
                if (bullets[i].active == 0) {
                    bullets[i].position.x = player.position.x + player.size.x / 2 - bullets[i].size.x / 2;
                    bullets[i].position.y = player.position.y - 20;
                    bullets[i].size = (Vector2){.x = 5, .y = 20};
                    bullets[i].velocity = 5;
                    bullets[i].damage = 5;
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
                
                
                //int isHit = CheckCollisionRecs(RectangleFromVector2(bullets[i].position, bullets[i].size), RectangleFromVector2(foes[i].position, target1.size));
                
                //if(isHit && foes[i].health != 0){
                    //foes[i].health -= bullets[i].damage;
                    //bullets[i].active = 0;
                //}
            }
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
            //DrawText(text, screenWidth / 2 - textWidth / 2, 200, 16, BLACK);
            
            DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, BLUE);
           
            for(int i = 0; i < sizeof(foes) / sizeof(foes[0]); i++) {
                if(foes[i].health > 0) {
                    //DrawRectangle(foes[i].position.x, foes[i].position.y, foes[i].size.x, ., Color color)
                    DrawRectangleV(foes[i].position, foes[i].size, RED);
                }
            }

            for (int i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++) {
                if (bullets[i].active == 1) {
                    DrawRectangle(bullets[i].position.x, bullets[i].position.y, bullets[i].size.x, bullets[i].size.y, GREEN);
                }
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
