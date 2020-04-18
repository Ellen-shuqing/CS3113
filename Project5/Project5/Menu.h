//
//  Menu.h
//  Project5
//
//  Created by Shuqing Li on 4/17/20.
//  Copyright © 2020 Shuqing Li. All rights reserved.
//

#include "Scene.h"
class Menu : public Scene {
    
public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};


