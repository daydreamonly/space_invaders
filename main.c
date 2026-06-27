#include "raylib.h"
#include <stdio.h>
#define MAX_IN_ROW 15
#define ARR_LEN(array) sizeof(array)/sizeof(array[0])

const int screenWidth = 800;
const int screenHeight = 400;

typedef struct Player {
    Vector2 position;
    Vector2 size;
    float velocity;
}Player;

typedef struct Bullet {
    Vector2 position;
    Vector2 size;
    float velocity;
    int damage;
    int active;
}Bullet;

typedef struct Foe {
    Vector2 position;
    Vector2 size;
    float velocity;
    int health;
}Foe;

Rectangle RectangleFromVector2(Vector2 position, Vector2 size){
    Rectangle rect = {
        .x = position.x,
        .y = position.y,
        .width = size.x,
        .height = size.y
    };
    return rect;
};

void update_player(Player *player, int screenWidth) {
    if (IsKeyDown(KEY_RIGHT)) player -> position.x += player->velocity;
    if (IsKeyDown(KEY_LEFT)) player -> position.x -= player->velocity;
    if (player->position.x < 0) {
        player->position.x = screenWidth - player->size.x;
    } else if (player->position.x > screenWidth - player->size.x) {
        player->position.x = 0;
    }
}

void update_foe(Foe *arr, int len, int screenWidth, int *foe_direction, float delta) {
    int min;
    if (len < MAX_IN_ROW) {
        min = len;
    } else {
        min = MAX_IN_ROW - 1;
    }
    if (arr[0].position.x < 100 || arr[min].position.x > 700) {
        *foe_direction = -*foe_direction;
    }
    for (int i = 0; i < len; i++) {
        Foe *foe = &arr[i];
        foe->position.x += foe->velocity * (*foe_direction) * delta;
    }
}

void update_bullets(Bullet *bullets, Player *player, Foe *foes, int bullets_len, int foes_len, float bps, float *shootTimer){
    if (*shootTimer > (1 / bps)) {
        for (int i = 0; i < bullets_len; i++) {
            if (bullets[i].active == 0) {
                bullets[i].position.x = player->position.x + player->size.x / 2 - bullets[i].size.x / 2;
                bullets[i].position.y = player->position.y - 20;
                bullets[i].active = 1;
                break;
            }
        }
        *shootTimer = 0;
    }
    for (int i = 0; i < bullets_len ; i++) {
        Bullet *current_bullet = &bullets[i];
        if (current_bullet->active == 1) {
            if (current_bullet->position.y > 0) {
                current_bullet->position.y -= current_bullet->velocity;
            } else {
                current_bullet->active = 0;
            }

            for (int j = 0; j < foes_len; j++) {
                Foe *current_foe = &foes[j];
                int isHit = CheckCollisionRecs(RectangleFromVector2(current_bullet->position, current_bullet->size), RectangleFromVector2(current_foe->position, current_foe->size));
                if(isHit && current_foe->health >= 0){
                    current_foe->health -= current_bullet->damage;
                    current_bullet->active = 0;
                }
            }
        }
    }
}

void init_foes(Foe* arr, int len, Vector2 cords, int margin){
    int items_in_row;
    int padding = 50;
    if(len-1 > MAX_IN_ROW){
        items_in_row = MAX_IN_ROW;
    } else {
        items_in_row = len-1;
    }

    int foes_rec_width = items_in_row * (MAX_IN_ROW + margin) - margin;
    float centered = cords.x / 2.0f - foes_rec_width / 2.0f;
    int row = 0;

    for(int i = 0; i < len; i++){
        if(i % MAX_IN_ROW == 0 && i != 0) {
            row += 1;
        }
        arr[i] = (Foe){
            .size.x = 20,
            .size.y = 20,
            .health = 20,
            .position.x = (MAX_IN_ROW + margin) * (i % MAX_IN_ROW) + centered,
            .position.y = (40 * row) + padding,
            .velocity = 80.0
        };
    }
}

void init_bullets(Bullet* arr, int len){
    for (int i = 0; i < len; i++) {
        arr[i].size = (Vector2){.x = 5, .y = 20};
        arr[i].velocity = 5.0;
        arr[i].damage = 7;
    }
}

int main(void) {
    //const char *text = "Welcome Invader";
    float shootTimer = 0;
    Bullet bullets[10] = {0};
    Foe foes[60];
    int foe_direction = 1;
    
    Player player = {
        .position = {.x = screenWidth / 2.0f, .y = screenHeight },
        .size = {.x = 20, .y = 20},
        .velocity = 4.5
    };
    init_foes(foes, sizeof(foes) / sizeof(foes[0]), (Vector2){screenWidth, screenHeight}, 20);
    init_bullets(bullets, sizeof(bullets) / sizeof(bullets[0]));
    InitWindow(screenWidth, screenHeight, "Space Invaders");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        float delta = GetFrameTime();
        shootTimer += delta;

        if (player.position.y > 350) {
            player.position.y -= player.velocity;
        } else {
            update_player(&player, screenWidth);
        }

        update_foe(foes, sizeof(foes) / sizeof(foes[0]), screenWidth, &foe_direction, delta);
        update_bullets(bullets, &player, foes, ARR_LEN(bullets), ARR_LEN(foes), 2, &shootTimer);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        //DrawText(text, screenWidth / 2 - textWidth / 2, 200, 16, BLACK);

        DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, BLUE);

        for(int i = 0; i < sizeof(foes) / sizeof(foes[0]); i++) {
            if(foes[i].health > 0) {
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
