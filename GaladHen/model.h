
// Model class

// Model and Mesh classes follow RAII principles (https://en.cppreference.com/w/cpp/language/raii).
// Model is a "move-only" class. A move-only class ensures that you always have a 1:1 relationship between the total number of resources being created and the total number of actual instantiations occurring.

#pragma once

#include <string>
#include <vector>

class Mesh;
class Material;
class aiNode;
class aiScene;
class aiMesh;

class Model
{

public:

    std::vector<Mesh> Meshes;

    // We want Model to be a move-only class. We delete copy constructor and copy assignment
    // see:
    // https://docs.microsoft.com/en-us/cpp/cpp/constructors-cpp?view=vs-2019
    // https://en.cppreference.com/w/cpp/language/copy_constructor
    // https://en.cppreference.com/w/cpp/language/copy_assignment
    // https://www.geeksforgeeks.org/preventing-object-copy-in-cpp-3-different-ways/
    // Section 4.6 of the "A Tour in C++" book
    Model(const Model& model) = delete; //disallow copy
    Model& operator=(const Model& copy) = delete;

    // For the Model class, a default move constructor and move assignment is sufficient
    // see:
    // https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=vs-2019
    // https://en.cppreference.com/w/cpp/language/move_constructor
    // https://en.cppreference.com/w/cpp/language/move_assignment
    // https://www.learncpp.com/cpp-tutorial/15-1-intro-to-smart-pointers-move-semantics/
    // https://www.learncpp.com/cpp-tutorial/15-3-move-constructors-and-move-assignment/
    // Section 4.6 of the "A Tour in C++" book
    Model& operator=(Model&& move) noexcept = default;
    Model(Model&& model) = default; //internally does a memberwise std::move

    Model();

    // @brief
    // Model creation by file loading
    Model(const std::string& modelPath);

    // @brief
    // Drawcall on all the meshes of the model
    void Draw() const;

    // @brief
    // Drawcall on all the meshes of the model, using specific materials
    void Draw(const std::vector<Material*>& materials) const;

    // @brief
    // Drawcall on all the meshes of the model, using specific materials
    void Draw(const std::vector<Material*>&& materials) const;

protected:

    // @brief
    // Load 3D model from a file using assimp
    void LoadModelFile(const std::string& modelPath);

    // @brief
    // Util function for assimp model loading
    void ProcessAssimpNode(aiNode* node, const aiScene* scene);

    // @brief
    // Convert assimp data structure into ezengine data structure
    Mesh ProcessMesh(aiMesh* mesh);
};