//
//  Level2.h
//  Project5
//
//  Created by Shuqing Li on 4/16/20.
//  Copyright © 2020 Shuqing Li. All rights reserved.
//

#include "Scene.h"
class Level2 : public Scene {
    
public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};

