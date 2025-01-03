//
// Created by Bekir Gulestan on 12/31/24.
//
#include "Engine/Physics.h"
#include "Engine/HighRenderer.h"

b2WorldId Physics::worldId;

std::map<unsigned int, b2BodyId> Physics::m_Objects;


void Physics::Init() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2) {0.0f, -10.0f};
    worldId = b2CreateWorld(&worldDef);


    // setup ground box
    b2BodyDef groundBodyDef = b2DefaultBodyDef();
    groundBodyDef.position = (b2Vec2) {0.0f, -1.0f};
    b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);
    b2Polygon groundBox = b2MakeBox(10.0f, .5f);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    //TODO: add a ground object to the renderer

}


void Physics::Update() {
    // Warning: this is not the correct way to get the time step
    float timeStep = LowRenderer::getDeltaTime();
    int subStepCount = 4;


    b2World_Step(worldId, timeStep, subStepCount);

    // update obj positions according to the simulation
    for (auto &[id, bodyId]: m_Objects) {
        auto obj = HighRenderer::getById(id);
        b2Vec2 position = b2Body_GetPosition(bodyId);
        b2Rot rotation = b2Body_GetRotation(bodyId);
        obj->position.x = position.x;
        obj->position.y = position.y;
        obj->rotation = glm::degrees(b2Rot_GetAngle(rotation));
    }


}

void Physics::RemoveObject(unsigned int m_Id) {

    auto it = m_Objects.find(m_Id);
    if (it != m_Objects.end()) {
        b2DestroyBody(it->second);
        m_Objects.erase(it);
    }
}

b2BodyId Physics::AddObject(unsigned int m_Id) {

    // get Objects Position
    auto obj = HighRenderer::getById(m_Id);

    b2BodyDef groundBodyDef = b2DefaultBodyDef();

    groundBodyDef.type = b2BodyType::b2_dynamicBody;
    groundBodyDef.position = (b2Vec2) {obj->position.x, obj->position.y};

    b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);

    b2Polygon dynamicBox = b2MakeBox(0.5f, .5f);

    b2ShapeDef shapeDef = b2DefaultShapeDef();

    shapeDef.density = 1.0f;
    shapeDef.friction = 0.3f;

    b2CreatePolygonShape(groundId, &shapeDef, &dynamicBox);

    // keep this related with m_Id and groundId;
    m_Objects[m_Id] = groundId;

    return groundId;

}

Physics::~Physics() {
    b2DestroyWorld(worldId);

}
