//
//  Level2.cpp
//  Project5
//
//  Created by Shuqing Li on 4/16/20.
//  Copyright © 2020 Shuqing Li. All rights reserved.
//

#include "Level2.h"
#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8

#define LEVEL2_ENEMY_COUNT 1
unsigned int level2_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

void Level2::Initialize() {
     state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);
    // Move over all of the player and enemy code from initialization.
    // Initialize Game Objects
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(2, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -1.5f, 0);
    state.player->speed = 2.5f;
    state.player->textureID = Util::LoadTexture("gudetama.png");
    state.player->jumpPower = 1.5f;
    state.player->height = 1.0f;
    state.player->width = 1.0f;
    
    
    
    // Initialize enemies
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID1 = Util::LoadTexture("toast.png");
    
    state.enemies[0].textureID = enemyTextureID1;
    state.enemies[0].position = glm::vec3(7.9f, -5, 0);
    state.enemies[0].acceleration = glm::vec3(0, -1.5f, 0);
    state.enemies[0].height = 1.0f;
    state.enemies[0].width = 1.0f;
    state.enemies[0].speed = 2.5f;
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = JUMPER;
    state.enemies[0].aiState = JUMPING;
    
    //initialize font
    state.font = new Entity();
    state.font->position = glm::vec3(5, -3.25, 0);
    state.font->textureID = Util::LoadTexture("font1.png");
}

void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, state.map, LEVEL2_ENEMY_COUNT);
    for (int i=0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, &state.enemies[i], state.map, LEVEL2_ENEMY_COUNT);
    }
    
    if (state.player->position.x >=12) {
        state.nextScene = 3;
    }
}

void Level2::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    state.enemies[0].Render(program);
}
