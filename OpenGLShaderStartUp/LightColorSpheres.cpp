#include "LightColorSpheres.h"

// Library Defines
#include <tiny_obj_loader.h>

// Library Includes
#include <iostream>
#include <random>

LightColorSpheres::LightColorSpheres(std::string ObjectFilePath, Camera* _CameraRef, int _InstanceCount,
    float _scale, glm::vec3* _Position, glm::vec3* _Colors)
{
    //VPMatrix = CameraRef->GetProjMat() * CameraRef->GetViewMat();

    Scale = glm::vec3(_scale);
    InstanceCount = _InstanceCount;
    CameraRef = _CameraRef;
    InstancedLightColorSpheress = new glm::mat4[InstanceCount];
    this->Colors = new glm::vec3[InstanceCount]; // Allocate memory for colors

    std::vector<VertexStandardSolidColor> Vertices;
    tinyobj::ObjReaderConfig ReaderConfig;
    tinyobj::ObjReader Reader;

    if (!Reader.ParseFromFile(ObjectFilePath, ReaderConfig))
    {
        if (!Reader.Error().empty())
        {
            std::cerr << "TinyObjReader Error: " << Reader.Error() << std::endl;
        }
        exit(1);
    }

    if (!Reader.Warning().empty())
    {
        std::cout << "TinyObjReader Warning: " << Reader.Warning() << std::endl;
    }

    auto& Attrib = Reader.GetAttrib();
    auto& Shapes = Reader.GetShapes();

    for (int i = 0; i < InstanceCount; i++)
    {
        Position = _Position[i];
        this->Colors[i] = _Colors[i];

        glm::vec3 RotationApplyVector = glm::vec3(1.0f, 0.0f, 0.0f);

        TranslationMat = glm::translate(glm::mat4(1.0f), Position);
        RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), RotationApplyVector);
        ScaleMat = glm::scale(glm::mat4(1.0f), Scale);
        LightColorSpheresMat = TranslationMat * RotationMat * ScaleMat;

        InstancedLightColorSpheress[i] = LightColorSpheresMat;
    }

    for (size_t ShapeIndex = 0; ShapeIndex < Shapes.size(); ShapeIndex++)
    {
        size_t ReadIndexOffset = 0;
        for (size_t FaceIndex = 0; FaceIndex < Shapes[ShapeIndex].mesh.num_face_vertices.size(); FaceIndex++)
        {
            size_t FaceVertexCount = size_t(Shapes[ShapeIndex].mesh.num_face_vertices[FaceIndex]);
            for (size_t VertexIndex = 0; VertexIndex < FaceVertexCount; VertexIndex++)
            {
                VertexStandardSolidColor Vertex{};
                tinyobj::index_t TinyObjVertex = Shapes[ShapeIndex].mesh.indices[ReadIndexOffset + VertexIndex];
                Vertex.position = {
                    Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 0],
                    Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 1],
                    Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 2],
                };
                if (TinyObjVertex.texcoord_index >= 0)
                {
                    Vertex.texcoord = {
                        Attrib.texcoords[2 * size_t(TinyObjVertex.texcoord_index) + 0],
                        Attrib.texcoords[2 * size_t(TinyObjVertex.texcoord_index) + 1],
                    };
                }
                Vertices.push_back(Vertex);
            }
            ReadIndexOffset += FaceVertexCount;
        }
    }

    DrawType = GL_TRIANGLES;
    DrawCount = (GLuint)Vertices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &InstanceVBO);
    glGenBuffers(1, &ColorVBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexStandardSolidColor) * DrawCount, Vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandardSolidColor), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexStandardSolidColor), (void*)offsetof(VertexStandardSolidColor, texcoord));

    glBindBuffer(GL_ARRAY_BUFFER, InstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, InstanceCount * sizeof(glm::mat4), InstancedLightColorSpheress, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(5, 1);

    glBindBuffer(GL_ARRAY_BUFFER, ColorVBO);
    glBufferData(GL_ARRAY_BUFFER, InstanceCount * sizeof(glm::vec3), Colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
}


LightColorSpheres::~LightColorSpheres()
{
}

void LightColorSpheres::Update(GLFWwindow* _WindowRef)
{
	VPMatrix = CameraRef->GetProjMat() * CameraRef->GetViewMat();
}

void LightColorSpheres::Render(GLuint* _Program)
{
    glUseProgram(*_Program);
    glBindVertexArray(VAO);

    GLint colorLoc = glGetUniformLocation(*_Program, "Colors");
    if (colorLoc != -1) {
        glUniform3fv(colorLoc, InstanceCount, glm::value_ptr(*Colors));
    }

    glUniformMatrix4fv(glGetUniformLocation(*_Program, "VP"), 1, GL_FALSE, glm::value_ptr(VPMatrix));

    glDrawArraysInstanced(DrawType, 0, DrawCount, InstanceCount);

    glBindVertexArray(0);
    glUseProgram(0);
}


