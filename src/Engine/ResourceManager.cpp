//
// Created by Bekir Gulestan on 1/12/25.
//

#include "Engine/ResourceManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::map<std::string, std::shared_ptr<ParsedObject>> ResourceManager::m_Objects;
std::map<std::string, std::shared_ptr<ShaderSource>> ResourceManager::m_Shaders;
std::map<std::string, std::shared_ptr<Texture>> ResourceManager::m_Textures;
std::map<std::string, std::shared_ptr<ShaderProgram>> ResourceManager::m_Programs;
std::map<std::string, std::shared_ptr<Font>> ResourceManager::m_Fonts;
std::hash<std::string> ResourceManager::hash;
std::map<unsigned short, std::string> ResourceManager::m_FontIdToCacheId;

ParsedObject ResourceManager::ParseObjectFromFile(const std::string &pathName) {

    ParsedObject obj;

#if PRODUCTION_BUILD == 1
    // solve for relative path
    std::string fullPath = std::filesystem::current_path().c_str();
    fullPath += &RESOURCES_PATH[1];
    fullPath += pathName;
#elif PRODUCTION_BUILD == 0
    std::string fullPath = RESOURCES_PATH + pathName;
#endif

    // 1- read file
    std::ifstream file(fullPath);

    if (!file.is_open()) {
        std::cerr << "Failed to open object file: " << fullPath << std::endl;
        throw;
    }

    std::string line;

    while (std::getline(file, line)) {

        if (line.empty())
            continue;
        // if line starts with -v
        // read the next 2 floats
        // floats are in the format of x,y
        // there are in model space [-1,1]


        // 1.1- read vertices
        if (strcmp(line.substr(0, 2).c_str(), "-v") == 0) {
            // remove -v from the line
            line = line.substr(2);

            // read the next 2 floats
            // separated by a comma
            std::string x, y;
            std::istringstream iss(line);
            std::getline(iss, x, ',');
            std::getline(iss, y);


            obj.vertices.emplace_back(std::stof(x), std::stof(y));
            continue;
        }

        // if line starts with -i
        // read the next 3 integers
        // integers are the indices of the vertices
        // there are in the order of the vertices

        // 1.2- read indices
        if (strcmp(line.substr(0, 2).c_str(), "-f") == 0) {
            // read the next 3 integers
            // separated by a comma

            // remove -i from the line
            line = line.substr(2);

            std::string i1, i2, i3;
            std::istringstream iss(line);
            std::getline(iss, i1, ',');

            std::getline(iss, i2, ',');
            std::getline(iss, i3);

            // add face
            obj.indices.push_back(std::stoi(i1));
            obj.indices.push_back(std::stoi(i2));
            obj.indices.push_back(std::stoi(i3));

            continue;
        }

        // TODO: add support for colors

        // support for texture Coordinates
        if (strcmp(line.substr(0, 2).c_str(), "-t") == 0) {
            obj.isTextured = true;
            // read the next 2 floats
            // separated by a comma

            // remove -t from the line
            line = line.substr(2);

            std::string f1, f2;

            std::istringstream iss(line);
            std::getline(iss, f1, ',');
            std::getline(iss, f2);

            obj.texCoords.emplace_back(std::stof(f1), std::stof(f2));

            continue;
        }

    }

    return obj;
}


std::shared_ptr<ShaderProgram> ResourceManager::LoadShader(const char *vertexPath, const char *fragmentPath) {


    //create id from paths
    // first vertex shader path
    std::string vertId = vertexPath;
    std::string fragId = fragmentPath;
    // check if program exists
    if (m_Programs.find(vertId + fragId) != m_Programs.end()) {
        return m_Programs[vertId + fragId];
    }


    std::shared_ptr<ShaderSource> vertex;
    // check if
    if (m_Shaders.find(vertId) != m_Shaders.end()) {
        vertex = m_Shaders[vertId];
    } else {
        vertex = std::make_shared<ShaderSource>(vertId, ShaderSource::Type::VERTEX);
        m_Shaders[vertId] = vertex;
    }


    std::shared_ptr<ShaderSource> fragment;
    // check if
    if (m_Shaders.find(fragId) != m_Shaders.end()) {
        fragment = m_Shaders[fragId];
    } else {
        fragment = std::make_shared<ShaderSource>(fragId, ShaderSource::Type::FRAGMENT);
        m_Shaders[fragId] = fragment;
    }

    auto program = std::make_shared<ShaderProgram>(vertex, fragment);
    m_Programs[vertId + fragId] = program;

    return program;
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(const std::string &path) {
    // check if string exists in the map
    if (m_Textures.find(path) != m_Textures.end()) {
        return m_Textures[path];
    }

    auto texture = std::make_shared<Texture>(path);
    m_Textures[path] = texture;
    return texture;

}


std::shared_ptr<Texture>
ResourceManager::LoadTextureFromBytes(const std::string &cacheId, const unsigned char *data, int w, int h) {
    // check if string exists in the map
    if (m_Textures.find(cacheId) != m_Textures.end()) {
        return m_Textures[cacheId];
    }

    auto texture = std::make_shared<Texture>(data, w, h);

    m_Textures[cacheId] = texture;

    return texture;

}


unsigned char ResourceManager::GetFontId(const std::string &path, int fontSize) {
    if (m_Fonts.find(path + "-" + std::to_string(fontSize)) != m_Fonts.end()) {
        return m_Fonts[path + "-" + std::to_string(fontSize)]->getHashId();
    }
    unsigned char hashId = hash(path + "-" + std::to_string(fontSize));
    auto font = std::make_shared<Font>(path, fontSize, hashId);

    m_Fonts[path + "-" + std::to_string(fontSize)] = font;

    m_FontIdToCacheId[hashId] = path + "-" + std::to_string(fontSize);


    return (unsigned char) hashId;
}

std::shared_ptr<Font> ResourceManager::LoadFont(const std::string &path, int fontSize) {
    if (m_Fonts.find(path + "-" + std::to_string(fontSize)) != m_Fonts.end()) {
        return m_Fonts[path + "-" + std::to_string(fontSize)];
    }
    auto hashId = hash(path + "-" + std::to_string(fontSize));
    auto font = std::make_shared<Font>(path, fontSize, hashId);

    m_Fonts[path + "-" + std::to_string(fontSize)] = font;
    return font;
}

ParsedObject &ResourceManager::LoadObject(const std::string &path) {
    if (m_Objects.find(path) != m_Objects.end()) {
        // return a copy of m_object[path]
        return *m_Objects[path];
    }


    auto objParsed = std::make_shared<ParsedObject>(ParseObjectFromFile(path));
    m_Objects[path] = objParsed;

    std::cout << "new Object read: " << path << "\n";
    return *objParsed;
}

std::shared_ptr<Font> ResourceManager::LoadFontById(unsigned short fontId) {

    assert(m_FontIdToCacheId.find(fontId) != m_FontIdToCacheId.end());

    return m_Fonts[m_FontIdToCacheId[fontId]];
}

void ResourceManager::Destroy() {
    m_Fonts.clear();
    m_Textures.clear();
    m_Shaders.clear();
    m_Objects.clear();
    m_Programs.clear();

}
