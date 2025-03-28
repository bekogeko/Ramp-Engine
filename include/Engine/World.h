//
// Created by Bekir Gulestan on 1/8/25.
//

#ifndef RAY_GAME_WORLD_H
#define RAY_GAME_WORLD_H

#include <memory>
#include <map>

#include "Engine/Object/Object.h"
#include "Layer.h"
#include "HighRenderer.h"


class World : public Layer {
public:
    explicit World(int index);

    ~World() {
        m_objects.clear();
    }

//    static unsigned int RegisterObject(const std::shared_ptr<Object> &object);
//
//    static unsigned int
//    RegisterObject(float *vertices, unsigned int size, unsigned int *indices, unsigned int indicesSize,
//                   const LayoutStack &stack);

    static unsigned int RegisterObject(const std::string &pathName);


    static std::weak_ptr<Object> getById(unsigned int id);

    static int GetNextId() {
        static int id = 1;
        return id++;
    }

    void Update(float deltaTime) override;

    void Draw() override;


private:

    // array of game objects
    static std::map<unsigned int, std::shared_ptr<Object>> m_objects;


};

#endif //RAY_GAME_WORLD_H
