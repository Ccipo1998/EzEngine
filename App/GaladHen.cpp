
#include <Renderer/Renderer.h>
#include <Renderer/Window.h>
#include <Core/Scene.h>
#include <Core/ShaderProgram.h>
#include <Core/Shader.h>
#include <Core/AssetsManager.h>
#include <Core/Model.h>
#include <Core/Material.h>
#include <Core/Texture.h>

#include <Core/BVH/BVH.h>
#include <Core/BVH/BVHNode.h>
#include <Core/AABB/AABB.h>

#include <Math/Ray.h>
#include <Math/Math.h>

#include <Core/Color.h>

#include <glm/glm.hpp>

#include <string>

using namespace GaladHen;

int main()
{
    // create opengl renderer
    Renderer renderer{ API::OpenGL };
    renderer.Init();

    // make window
    Window window{ API::OpenGL, "GaladHen" };
    window.SetColorBufferClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    window.EnableDepthTest(true);

    // create scene
    Scene scene{};

    // load textures
    Texture* texAlbedo = AssetsManager::LoadAndStoreTexture(
        "../Assets/Textures/StuccoRoughCast001_COL_2K_METALNESS.png",
        "StuccoAlbedo",
        TextureFormat::SRGB8);
    texAlbedo->NumberOfMipMaps = 4;
    Texture* texNormal = AssetsManager::LoadAndStoreTexture(
        "../Assets/Textures/StuccoRoughCast001_NRM_2K_METALNESS.png",
        "StuccoNormal",
        TextureFormat::RGB8);
    texNormal->NumberOfMipMaps = 4;
    Texture* texRoughness = AssetsManager::LoadAndStoreTexture(
        "../Assets/Textures/StuccoRoughCast001_ROUGHNESS_2K_METALNESS.png",
        "StuccoRoughness",
        TextureFormat::RGB);
    texRoughness->NumberOfMipMaps = 4;

    // get pbr shader pipeline
    ShaderPipeline pbr = AssetsManager::GetPipelinePBR();

    // materials
    Material bunnyMat{ &pbr, ShadingMode::SmoothShading };
    PBRMaterialData bunnyMatData{};
    TextureParameters diffuse{};
    diffuse.TextureSource = texAlbedo;
    TextureParameters normal{};
    normal.TextureSource = texNormal;
    bunnyMatData.Metallic = 0.0f;
    bunnyMatData.Roughness = 0.5f;
    /*TextureParameters roughness{};
    roughness.TextureSource = texRoughness;*/
    bunnyMatData.DiffuseTexture = diffuse;
    bunnyMatData.Diffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    bunnyMatData.NormalTexture = normal;
    //bunnyMatData.RoughnessTexture = roughness;
    bunnyMat.Data = &bunnyMatData;

    Material planeMat{ &pbr, ShadingMode::SmoothShading };
    PBRMaterialData planeMatData{};
    planeMatData.Diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    planeMatData.Roughness = 0.1f;
    planeMatData.Metallic = 0.0f;
    planeMat.Data = &planeMatData;

    // load models
    Model* bunny = AssetsManager::LoadAndStoreModel("../Assets/Models/bunny.glb", "Bunny");
    Model* plane = AssetsManager::LoadAndStoreModel("../Assets/Models/plane.glb", "Plane");

    // bvh
    bunny->BuildModelBVH(AABBSplitMethod::PlaneCandidates, AABBSplitMethod::PlaneCandidates);

    // shaders and materials
    Shader vUnlit{ "../Shaders/Unlit/Unlit.vert", ShaderStage::Vertex };
    Shader fUnlit{ "../Shaders/Unlit/Unlit.frag", ShaderStage::Fragment };
    ShaderPipeline unlit{};
    unlit.VertexShader = &vUnlit;
    unlit.FragmentShader = &fUnlit;
    Material aabbMat{ &unlit, ShadingMode::SmoothShading };
    UnlitMaterialData aabbMatData{};
    aabbMatData.DiffuseColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    aabbMat.Data = &aabbMatData;
    renderer.CompileShaderPipeline(unlit);

    // ray
    Ray ray
    {
        scene.MainCamera.Transform.GetPosition(),
        glm::vec3(0.0f, 0.0f, -1.0f),
        1000.0f
    };

    // load into scene
    std::vector<Material*> bunnyMats;
    bunnyMats.push_back(&bunnyMat);
    SceneObject objBunny{ bunny, bunnyMats };
    objBunny.Transform.SetPosition(glm::vec3(0.0f, 1.5f, 0.0f));
    objBunny.Transform.SetYaw(50.0f);
    scene.SceneObjects.push_back(objBunny);
    std::vector<Material*> planeMats;
    planeMats.push_back(&planeMat);
    SceneObject objPlane{ plane, planeMats };
    objPlane.Transform.SetScale(glm::vec3(5.0f, 1.0f, 5.0f));
    //scene.SceneObjects.push_back(objPlane);

    RayModelHitInfo hit = Math::RaySceneObjectIntersection(ray, objBunny, BVHTraversalMethod::FrontToBack);
    Mesh aabbMesh{};
    aabbMesh.Indices = { 0, 1, 2 };
    aabbMesh.Vertices.push_back(bunny->Meshes[0].Vertices[hit.VertexIndex0]);
    aabbMesh.Vertices[0].Position = glm::vec3(objBunny.Transform.GetModelMatrix() * glm::vec4(aabbMesh.Vertices[0].Position, 1.0f)) + glm::vec3(0.0f, 0.0f, 0.1f);
    aabbMesh.Vertices.push_back(bunny->Meshes[0].Vertices[hit.VertexIndex1]);
    aabbMesh.Vertices[1].Position = glm::vec3(objBunny.Transform.GetModelMatrix() * glm::vec4(aabbMesh.Vertices[1].Position, 1.0f)) + glm::vec3(0.0f, 0.0f, 0.1f);
    aabbMesh.Vertices.push_back(bunny->Meshes[0].Vertices[hit.VertexIndex2]);
    aabbMesh.Vertices[2].Position = glm::vec3(objBunny.Transform.GetModelMatrix() * glm::vec4(aabbMesh.Vertices[2].Position, 1.0f)) + glm::vec3(0.0f, 0.0f, 0.1f);
    renderer.LoadMesh(aabbMesh);

    // Gizmos
    Mesh gizmos{};
    gizmos.PrimitiveType = Primitive::Line;
    VertexData v{};
    v.Position = glm::vec3(0.0f);
    v.Color = Color::Red;
    gizmos.Vertices.push_back(v);
    v.Position = glm::vec3(1.0f, 0.0f, 0.0f);
    gizmos.Vertices.push_back(v);
    v.Color = Color::Green;
    v.Position = glm::vec3(0.0f);
    gizmos.Vertices.push_back(v);
    v.Position = glm::vec3(0.0f, 1.0f, 0.0f);
    gizmos.Vertices.push_back(v);
    v.Color = Color::Blue;
    v.Position = glm::vec3(0.0f);
    gizmos.Vertices.push_back(v);
    v.Position = glm::vec3(0.0f, 0.0f, 1.0f);
    gizmos.Vertices.push_back(v);
    gizmos.Indices = std::vector<unsigned int>{ 0, 1, 2, 3, 4, 5 };

    Material gizmosMat{ &unlit, ShadingMode::SmoothShading };
    UnlitMaterialData gizmosMatData{};
    gizmosMatData.UseVertexColor = true;
    gizmosMat.Data = &gizmosMatData;

    // init scene for renderer
    renderer.LoadModels(scene);
    renderer.LoadLightingData(scene);
    renderer.CompileShaders(scene);
    renderer.LoadCameraData(scene.MainCamera);
    renderer.LoadTexture(*texAlbedo);
    renderer.LoadTexture(*texNormal);
    renderer.LoadTexture(*texRoughness);
    renderer.LoadMesh(gizmos);
    renderer.LoadTransformData();

    while (!window.IsCloseWindowRequested())
    {
        window.BeginFrame();

        glm::vec3 cameraMov = glm::vec3(0.0f);
        if (window.IsKeyPressed(KeyboardKey::W))
        {
            cameraMov.z += 1.0f;
        }
        if (window.IsKeyPressed(KeyboardKey::S))
        {
            cameraMov.z += -1.0f;
        }
        if (window.IsKeyPressed(KeyboardKey::D))
        {
            cameraMov.x += 1.0f;
        }
        if (window.IsKeyPressed(KeyboardKey::A))
        {
            cameraMov.x += -1.0f;
        }
        if (window.IsKeyPressed(KeyboardKey::E))
        {
            cameraMov.y += 1.0f;
        }
        if (window.IsKeyPressed(KeyboardKey::Q))
        {
            cameraMov.y += -1.0f;
        }
        glm::vec2 cameraRot = glm::vec2(0.0f);
        if (window.IsKeyPressed(MouseKey::Right))
        {
            window.GetMousePositionDelta(cameraRot.x, cameraRot.y);
        }
        scene.MainCamera.ApplyCameraMovements(cameraMov, cameraRot, 0.0001f);
        renderer.UpdateCameraData(scene.MainCamera);

        //renderer.Draw(scene);
        //renderer.Draw(aabbMesh, aabbMat);
        renderer.Draw(gizmos, gizmosMat);

        window.EndFrame();
    }

    window.CloseWindow();

    return 0;
}
