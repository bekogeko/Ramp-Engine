//
// Created by Bekir Gulestan on 7/20/25.
//

#ifndef RAY_GAME_MESHCOMPONENT_H
#define RAY_GAME_MESHCOMPONENT_H

#include <utility>
#include <vector>

struct MeshComponent {
    // MeshComponent is a simple struct to hold mesh data
    // It can be used to store mesh data for rendering
    // This is a placeholder for future mesh component implementation

    // For now, it can hold vertices and indices
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Default constructor
    MeshComponent(std::vector<float> &verts, std::vector<unsigned int> &inds)
            : vertices(verts), indices(inds) {};

    // creates a MeshComponent with path a util function
    static MeshComponent LoadFromFile(const std::string &filePath);

    // Optional: Constructor to handle allocator (required by entt)
    template<typename Allocator>
    MeshComponent(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, const Allocator &alloc)
            : vertices(vertices.begin(), vertices.end(), alloc), indices(indices.begin(), indices.end(), alloc) {}


};

#endif //RAY_GAME_MESHCOMPONENT_H
