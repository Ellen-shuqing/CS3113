//
//  Menu.cpp
//  Project5
//
//  Created by Shuqing Li on 4/17/20.
//  Copyright © 2020 Shuqing Li. All rights reserved.
//


#include "Menu.h"
#define Menu_WIDTH 14
#define Menu_HEIGHT 8

#define Menu_ENEMY_COUNT 0
unsigned int Menu_data[] =
{
    0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0,
    0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0,
    0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0,
    0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0,
    0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0,
    0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0,
    0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0,
    0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0
};



void Menu::Initialize() {
    state.nextScene = -3;
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(Menu_WIDTH, Menu_HEIGHT, Menu_data, mapTextureID, 1.0f, 4, 1);
    // Move over all of the player and enemy code from initialization.
    // Initialize Game Objects
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(3, -2, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -1.5f, 0);
    state.player->speed = 2.5f;
    state.player->textureID = Util::LoadTexture("gudetama.png");
    state.player->jumpPower = 1.5f;
    state.player->height = 1.0f;
    state.player->width = 1.0f;
    
    
    //initialize font
    state.font = new Entity();
    state.font->position = glm::vec3(5, -3.25, 0);
    state.font->textureID = Util::LoadTexture("font1.png");
}

void Menu::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, NULL, state.map, Menu_ENEMY_COUNT);
}

void Menu::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
}
