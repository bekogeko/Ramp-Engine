//
// Created by Bekir Gulestan on 1/19/25.
//

#ifndef RAY_GAME_LAYOUTSTACK_H
#define RAY_GAME_LAYOUTSTACK_H


#include <vector>

class VertexLayout {
public:
    template<typename T = float>
    VertexLayout(int count, bool isInstanced = false) {
        m_size = sizeof(T) * count;
        m_Dimension = count;
        m_IsInstanced = isInstanced;

    }

    bool isInstanced() const {
        return m_IsInstanced;
    }

    int getDimension() const {
        return m_Dimension;
    }

    int size() const {
        return m_size;
    }

private:

    int m_size; // size in bytes
    int m_Dimension;
    bool m_IsInstanced;

};

class LayoutStack {
public:
    // list constructor
    LayoutStack(std::initializer_list<VertexLayout> layouts) : m_layout(layouts) {}

    /// Returns total dimentsion count
    int getDimentionCount() {
        int totalDimension = 0;
        for (auto layout: *this) {
            totalDimension += layout.getDimension();
        }
        return totalDimension;
    }

    int getSize() {
        int totalSize = 0;
        for (auto layout: *this) {
            totalSize += layout.size();
        }
        return totalSize;
    }


    int getOffsetOfIndex(int index) {
        int offset = 0;
        for (int i = 0; i < index; ++i) {
            offset += m_layout[i].size();
        }
        return offset;
    }

    bool IsInstanced() const {
        for (auto layout: m_layout) {
            if (layout.isInstanced())
                return true;
        }
        return false;
    }

    // will be able to used in for loop (auto a : stack)
    std::vector<VertexLayout>::iterator begin() { return m_layout.begin(); }

    // will be able to used in for loop (auto a: stack)
    std::vector<VertexLayout>::iterator end() { return m_layout.end(); }

private:
    std::vector<VertexLayout> m_layout;
};

#endif //RAY_GAME_LAYOUTSTACK_H
