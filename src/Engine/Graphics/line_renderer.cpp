#include "Engine/Graphics/renderer.hpp"

void Renderer::CreateLineBuffers()
{
    glGenVertexArrays(1, &linesVAO);
    glBindVertexArray(linesVAO);

    glGenBuffers(1, &linesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
    glBufferData(GL_ARRAY_BUFFER, 10000 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Renderer::DrawLine(glm::vec3 a, glm::vec3 b, glm::vec3 lineColor)
{
    Vertex vA;
    vA.position = a;
    vA.color = lineColor;
    
    Vertex vB;
    vB.position = b;
    vB.color = lineColor;

    lineVertices.push_back(vA);
    lineVertices.push_back(vB);
}

void Renderer::DrawAABB(const AABB& aabb)
{
    
}

void Renderer::FlushLines()
{
    if (lineVertices.empty())
        return;

    //Bind Line Shader
    glUseProgram(assetManager->GetShader("line")->ID);

    Transform cameraTransform = camera.transform;
    glm::vec3 cameraPos = cameraTransform.GetPosition();
    glm::vec3 cameraForward = cameraTransform.Forward();

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAtLH(cameraPos, cameraPos + cameraForward, cameraTransform.Up());

    assetManager->GetShader("line")->setMat4("projection", camera.GetProjection());
    assetManager->GetShader("line")->setMat4("view", view);

    glBindVertexArray(linesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, linesVBO);

    glBufferSubData(GL_ARRAY_BUFFER, 0, lineVertices.size() * sizeof(Vertex), lineVertices.data());

    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(lineVertices.size()));

    lineVertices.clear();
}