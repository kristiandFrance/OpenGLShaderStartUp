#include "Skybox.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Skybox::Skybox(Camera* _CameraRef, std::string FilePaths[])
{
    CameraRef = _CameraRef;

    Program = ShaderLoader::CreateProgram("Resources/Shaders/Skybox.vert",
        "Resources/Shaders/Skybox.frag");

    // Cube vertices for the skybox (only positions)
    GLfloat Vertices[] = {
        // Positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    DrawCount = sizeof(Vertices) / sizeof(Vertices[0]) / 3;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Load and create the cubemap texture
    glGenTextures(1, &TextureID_Skybox);
    glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID_Skybox);
    stbi_set_flip_vertically_on_load(false);

    int ImageWidth, ImageHeight, ImageComponents;
    for (int i = 0; i < 6; ++i)
    {
        unsigned char* ImageData = stbi_load(FilePaths[i].c_str(),
            &ImageWidth, &ImageHeight, &ImageComponents, 0);

        if (ImageData)
        {
            GLint Format = (ImageComponents == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Format,
                ImageWidth, ImageHeight, 0, Format, GL_UNSIGNED_BYTE, ImageData);
            stbi_image_free(ImageData);
        }
        else
        {
            std::cerr << "Failed to load texture: " << FilePaths[i] << std::endl;
            stbi_image_free(ImageData);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::Render()
{
    glDepthFunc(GL_LEQUAL); // Change depth function for skybox

    glUseProgram(Program);

    // Bind the skybox cubemap texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID_Skybox);
    glUniform1i(glGetUniformLocation(Program, "Texture_Skybox"), 0);

    glm::mat4 CamMatView = glm::mat4(glm::mat3(CameraRef->GetViewMat())); // Remove translation
    glm::mat4 CamMatProj = CameraRef->GetProjMat();
    glUniformMatrix4fv(glGetUniformLocation(Program, "VP"), 1, GL_FALSE, glm::value_ptr(CamMatProj * CamMatView));

    glBindVertexArray(VAO);
	glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, DrawCount);
    glDepthFunc(GL_LESS);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glUseProgram(0);

    glDepthFunc(GL_LESS); // Reset depth function
}
