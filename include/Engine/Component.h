//
// Created by Bekir Gulestan on 12/30/24.
//

#ifndef RAY_GAME_COMPONENT_H
#define RAY_GAME_COMPONENT_H

#include <memory>

class Object;

// Abstract class to be implemented in the client
// it will have Draw and update
class Component {
protected:
    unsigned int m_ObjectId = 0;

    [[nodiscard]] std::shared_ptr<Object> getObject() const;

private:
    bool isAttached = false;

public:

    virtual void attach(unsigned int objId) {
        if (isAttached)
            return;
        isAttached = true;
        m_ObjectId = objId;
    }

    virtual ~Component() = default;

    virtual void Draw() = 0;

    virtual void Update(float deltaTime) = 0;


};

#endif //RAY_GAME_COMPONENT_H
