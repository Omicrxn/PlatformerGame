#ifndef __SCENEENDING_H__
#define __SCENEENDING_H__

#include "Scene.h"

class SceneEnding : public Scene
{
public:

    SceneEnding(Window* win);
    virtual ~SceneEnding();

    bool Load(Textures* tex);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio);

private:
};

#endif // __SCENEENDING_H__
