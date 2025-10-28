#include "Engine/Physics/PhysicsSystem.h"

#include "Engine/HighRenderer.h"
#include "Engine/Window.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/RigidBody2D.h"
#include "Engine/Components/BoxCollider2D.h"

PhysicsSystem::PhysicsSystem(entt::registry &registry, const glm::vec2 &gravity): m_registry(registry) {

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity.x = gravity.x;
    worldDef.gravity.y = gravity.y;
        // maybe use userData?
        // worldDef.userData = this???;
    m_world = b2CreateWorld(&worldDef);
    connectSignals();
}

void PhysicsSystem::Step(float dt)
{
    // Tune velocity/position iterations for your needs
    int subSteps = 4;
    b2World_Step(m_world,dt,subSteps);
    syncTransformsFromPhysics();
}


void PhysicsSystem::connectSignals() {
    // Create body when RigidBody2D added
    m_registry.on_construct<RigidBody2D>().connect<&PhysicsSystem::onRigidBodyConstruct>(*this);
    // Destroy body when RigidBody2D removed
    m_registry.on_destroy<RigidBody2D>().connect<&PhysicsSystem::onRigidBodyDestroy>(*this);
    // Also listen to collider creation to refresh fixtures if RigidBody already exists
    m_registry.on_construct<BoxCollider2D>().connect<&PhysicsSystem::onRigidBodyConstruct>(*this);
    //
    m_registry.on_update<Transform>().connect<&PhysicsSystem::onTransformUpdated>(*this);
}

void PhysicsSystem::onRigidBodyConstruct(entt::registry& reg, entt::entity e) const {
    createOrRefreshBody(e);
}

void PhysicsSystem::onRigidBodyDestroy(entt::registry& /*reg*/, entt::entity e) const {
    destroyBodyIfAny(e);
}

void PhysicsSystem::onTransformUpdated(entt::registry &, entt::entity e) {
    // If entity has a RigidBody2D, rebuild body/fixtures to reflect transform changes (scale/position/rotation).
    // This is called when Transform is updated (including scale).
    if (m_registry.try_get<RigidBody2D>(e)) {
        createOrRefreshBody(e);
    }
}

void PhysicsSystem::createOrRefreshBody(entt::entity e) const {
    auto* rb = m_registry.try_get<RigidBody2D>(e);
    const auto* tr = m_registry.try_get<Transform>(e);
    if (!rb || !tr) return;

    // Destroy existing body if any (so we can rebuild fixtures)
    if (b2Body_IsValid(rb->body)) {
        b2DestroyBody(rb->body);
        rb->body = b2_nullBodyId;
    }

    b2BodyDef bd = b2DefaultBodyDef();
    bd.type = rb->type;
    bd.position.x = tr->position.x;
    bd.position.y = tr->position.y;

    bd.rotation = b2MakeRot(tr->rotation);
    bd.fixedRotation = rb->fixedRotation;
    bd.linearDamping = rb->linearDamping;
    bd.angularDamping = rb->angularDamping;


    rb->body = b2CreateBody(m_world,&bd);
    if (b2Body_IsValid(rb->body)) {
        // rb->body->SetLinearVelocity(b2Vec2(rb->vx, rb->vy));
        b2Body_SetLinearVelocity(rb->body, b2Vec2{rb->vx, rb->vy});
        // rb->body->SetAngularVelocity(rb->av);
        b2Body_SetAngularVelocity(rb->body, rb->av);
    }

    // Add fixtures from colliders (here a single BoxCollider2D, adjust if you support many)
    if (auto* box = m_registry.try_get<BoxCollider2D>(e)) {
        // b2Polygon polygon = b2MakePolygon(&hull,1);
        auto bxpoly = b2MakeBox(box->hx,box->hy);
        b2ShapeDef sd = b2DefaultShapeDef();
        sd.density = box->density;
        sd.friction = box->friction;
        sd.restitution = box->restitution;
        sd.isSensor = box->isSensor;

        b2CreatePolygonShape(rb->body,&sd,&bxpoly);
    }

    // TODO: Add MeshCollider
}

void PhysicsSystem::destroyBodyIfAny(entt::entity e) const {
    if (auto* rb = m_registry.try_get<RigidBody2D>(e)) {
        if (b2Body_IsValid(rb->body)) {
            b2DestroyBody(rb->body);
            rb->body = b2_nullBodyId;
        }
    }
}

void PhysicsSystem::syncTransformsFromPhysics() const {
    auto view = m_registry.view<Transform, RigidBody2D>();
    for (auto e : view) {
        auto& tr = view.get<Transform>(e);
        auto& rb = view.get<RigidBody2D>(e);
        if (!b2Body_IsValid(rb.body)) continue;
        // const b2Vec2 p = rb.body->GetPosition();
        const b2Vec2 p = b2Body_GetPosition(rb.body);
        tr.position.x = p.x;
        tr.position.y = p.y;
        tr.rotation = b2Rot_GetAngle(b2Body_GetRotation(rb.body));
        // Note: scale is not affected by physics; keep tr.sx, tr.sy as rendering scale
    }
}
