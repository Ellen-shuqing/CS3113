//
//  main.cpp
//  Project2
//
//  Created by Shuqing Li on 2/26/20.
//  Copyright © 2020 Shuqing Li. All rights reserved.
//

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, heartMatrix, pika1Matrix, pika2Matrix, projectionMatrix;

//heart start position(0, 0)
glm::vec3 heart_position = glm::vec3(0, 0, 0);

//heart movement
float heart_direction_x = 1.0f;
float heart_direction_y = 1.0f;

// Don't go anywhere (yet).
glm::vec3 heart_movement = glm::vec3(1, 0, 0);

//pika1 start position(-4.2, 0)
glm::vec3 pika1_position = glm::vec3(-4.2, 0, 0);

// Don't go anywhere (yet).
glm::vec3 pika1_movement = glm::vec3(1, 0, 0);

//pika2 start position(4.2, 0)
glm::vec3 pika2_position = glm::vec3(4.2, 0, 0);

// Don't go anywhere (yet).
glm::vec3 pika2_movement = glm::vec3(1, 0, 0);

//speed
float pika1_speed = 1.0f;
float pika2_speed = 1.0f;
float heart_speed_x = 1.0f;
float heart_speed_y = 0.8f;

//width
float w1 = 0.3f;
float w2 = 0.3f;
float w_heart = 0.7f;

//height
float h_heart = 0.7f;
float h1 = 0.3f;
float h2 = 0.3f;

GLuint heartTextureID;
GLuint pika1TextureID;
GLuint pika2TextureID;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    if (image == NULL) {
        std::cerr << "Unable to load.\n";
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(image);
    return textureID;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    heartMatrix = glm::mat4(1.0f);
    pika1Matrix = glm::mat4(1.0f);
    pika2Matrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(0.5f, 0.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(1.0f, 0.498039f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    
    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    heartTextureID = LoadTexture("heart.png");
    pika1TextureID = LoadTexture("pika.png");
    pika2TextureID = LoadTexture("pika.png");
}

void ProcessInput() {
    pika1_movement = glm::vec3(0);
    pika2_movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_A]) {
        pika1_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_Z]) {
        pika1_movement.y = -1.0f;
    }
    if (keys[SDL_SCANCODE_K]) {
        pika2_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_M]) {
        pika2_movement.y = -1.0f;
    }
}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    // add (direction * units per second * elapsed time)
    pika1_position += pika1_movement * pika1_speed * deltaTime;
    pika2_position += pika2_movement * pika2_speed * deltaTime;
   
    heart_position.x += heart_speed_x * heart_direction_x * deltaTime;
    heart_position.y += heart_speed_y * heart_direction_y * deltaTime;
    
    pika1Matrix = glm::mat4(1.0f);
    pika1Matrix = glm::translate(pika1Matrix, pika1_position);

    pika2Matrix = glm::mat4(1.0f);
    pika2Matrix = glm::translate(pika2Matrix, pika2_position);

    heartMatrix = glm::mat4(1.0f);
    heartMatrix = glm::translate(heartMatrix, heart_position);

    //pika1 hits the wall
    if(pika1_position.y + h1 >= 3.75f){
        pika1_position.y = 3.75f - h1;
    }else if(pika1_position.y + h1 <= -3.75f) {
        pika1_position.y = -3.75f + h1;
    }
    
    //pika2 hits the wall
    if(pika2_position.y + h2 >= 3.75f){
        pika2_position.y = 3.75f - h2;
    }else if(pika2_position.y + h2 <= -3.75f) {
        pika2_position.y = -3.75f + h2;
    }
    
    //game setting
    if(heart_position.x - w_heart/2.0f >= 5.0f){                          //heart hits wall on right
        std::cout << "Pika on the left won!\n";
        heart_position = glm::vec3(0, 0, 0);                              // reset position
        pika1_position = glm::vec3(-4.2, 0, 0);
        pika2_position = glm::vec3(4.2, 0, 0);
    }else if(heart_position.x - w_heart/2.0f <= -5.0f){                   //heart hits wall on left
        std::cout << "Pika on the right won!\n";
        heart_position = glm::vec3(0, 0, 0);                              //reset position
        pika1_position = glm::vec3(-4.2, 0, 0);
        pika2_position = glm::vec3(4.2, 0, 0);
    }else if(fabs(heart_position.y) + h_heart/2 >= 3.75f){                //heart hits top/bottom wall
        heart_direction_y = - heart_direction_y;
        heart_position.x += heart_speed_x * heart_direction_x * deltaTime;
        heart_position.y += heart_speed_y * heart_direction_y * deltaTime;
    }else if(fabs(pika1_position.y - heart_position.y) - ((h_heart + h1)/ 2.0f) < 0 &&
             fabs(pika1_position.x- heart_position.x) - ((w_heart + w1)/2.0f) < 0) {
        //heart hits pika
        heart_direction_x = - heart_direction_x;
        heart_position.x += heart_speed_x * heart_direction_x * deltaTime;
        heart_position.y += heart_speed_y * heart_direction_y * deltaTime;
    }else if (fabs(pika2_position.y - heart_position.y) - ((h_heart + h2)/2.0f) < 0 &&
              fabs(pika2_position.x- heart_position.x) - ((w_heart + w2)/2.0f) < 0) {
        //heart hits pika
        heart_direction_x = - heart_direction_x;
        heart_position.x += heart_speed_x * heart_direction_x * deltaTime;
        heart_position.y += heart_speed_y * heart_direction_y * deltaTime;
    }
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    //texture of heart
    program.SetModelMatrix(heartMatrix);
    
    float vertices[] = { -0.3, -0.3, 0.3, -0.3, 0.3, 0.3, -0.3, -0.3, 0.3, 0.3, -0.3, 0.3 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, heartTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
    //texture of pika1
    program.SetModelMatrix(pika1Matrix);
    
    float vertices1[] = { -0.7, -0.7, 0.7, -0.7, 0.7, 0.7, -0.7, -0.7, 0.7, 0.7, -0.7, 0.7 };
    float texCoords1[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords1);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, pika1TextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
    //texture of pika2
    program.SetModelMatrix(pika2Matrix);
    
    float vertices2[] = { -0.7, -0.7, 0.7, -0.7, 0.7, 0.7, -0.7, -0.7, 0.7, 0.7, -0.7, 0.7 };
    float texCoords2[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, pika2TextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
