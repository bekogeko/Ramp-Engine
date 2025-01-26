//
// Created by Bekir Gulestan on 1/8/25.
//

#ifndef RAY_GAME_LAYER_H
#define RAY_GAME_LAYER_H

class Layer {
private:
    unsigned int m_Index;
protected:
    explicit Layer(int index) {
        m_Index = index;
    }

    [[nodiscard]] unsigned int getLayerIndex() const { return m_Index; }


public:
    virtual void Update(float deltaTime) = 0;

    virtual void Draw() = 0;


    virtual ~Layer() = default;
};

#endif //RAY_GAME_LAYER_H
