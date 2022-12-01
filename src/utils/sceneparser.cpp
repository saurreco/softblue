#include "sceneparser.h"
#include "scenefilereader.h"
#include "glm/gtx/transform.hpp"

#include <chrono>
#include <memory>
#include <iostream>

void dfs(SceneNode* node, glm::mat4 mat, std::vector<RenderShapeData> &stack) {

    for (SceneTransformation* transform : node->transformations) {
        switch (transform->type) {
            case TransformationType::TRANSFORMATION_TRANSLATE:
                mat = mat * glm::translate(transform->translate);
                break;
            case TransformationType::TRANSFORMATION_SCALE:
                mat = mat * glm::mat4(glm::scale(transform->scale));
                break;
            case TransformationType::TRANSFORMATION_ROTATE:
                mat = mat * glm::mat4(glm::rotate(transform->angle, transform->rotate));
                break;
            case TransformationType::TRANSFORMATION_MATRIX:
                mat = mat * transform->matrix;
                break;
        }
    }

    if (node->primitives.size() > 0) {

        for (ScenePrimitive* prim : node->primitives) {
            stack.push_back(RenderShapeData{*prim, mat});
        }
    }

    for (SceneNode* child : node->children) {
        dfs(child, mat, stack);
    }
}

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readXML();
    if (!success) {
        return false;
    }

    renderData.globalData = fileReader.getGlobalData();
    renderData.lights = fileReader.getLights();
    renderData.cameraData = fileReader.getCameraData();

    renderData.shapes.clear();

    dfs(fileReader.getRootNode(), glm::mat4(1.0f), renderData.shapes);

    return true;
}
