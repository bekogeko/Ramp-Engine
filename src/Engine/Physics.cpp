//
// Created by Bekir Gulestan on 12/31/24.
//
#include "Engine/Physics.h"
#include "Engine/World.h"

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
    float timeStep = 1.0f / LowRenderer::getFPS();

    // for every 60 fps do a one iteration
    // no Vsync 360 fps = which means 6 iteration
    // with vSync we going to get 120fps = with 2 iteration
    // TODO: maybe add option of VSync with 4 iteration (for every 30 fps)
    int subStepCount = int(LowRenderer::getFPS() / 30);


    b2World_Step(worldId, timeStep, subStepCount);

    // update obj positions according to the simulation
    for (auto &[id, bodyId]: m_Objects) {
        auto obj = World::getById(id);
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
    auto obj = World::getById(m_Id);
    int vertexCount = obj->getVertexCount();

    // Create HULL
    auto *vertices = new b2Vec2[vertexCount];
    int deleted_vertexes = 0;
    for (int i = 0; i < vertexCount; ++i) {
        auto vert = obj->getVertices()[i];
        if (vert.x == 0 && vert.y == 0) {
            deleted_vertexes++;
            continue;
        }
        vertices[i].x = vert.x;
        vertices[i].y = vert.y;
    }


    b2Hull hull = b2ComputeHull(vertices, vertexCount - deleted_vertexes);

    b2BodyDef bodyDef = b2DefaultBodyDef();

    bodyDef.type = b2BodyType::b2_dynamicBody;
    bodyDef.position = (b2Vec2) {obj->position.x, obj->position.y};
    bodyDef.userData = obj.get();


    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.friction = 0.3f;



    // calculate radius
    // TODO calculate radius on Object upon vertices creation
    b2Polygon polygon = b2MakePolygon(&hull, 0);
    polygon.centroid = b2Vec2_zero;

    b2CreatePolygonShape(bodyId, &shapeDef, &polygon);


    // keep this related with m_Id and groundId;
    m_Objects[m_Id] = bodyId;

    return bodyId;
}

Physics::~Physics() {
    b2DestroyWorld(worldId);

}
