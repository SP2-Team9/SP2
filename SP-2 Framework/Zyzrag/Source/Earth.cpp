#include "Earth.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Utility.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"


Earth::Earth(){



}

Earth::~Earth(){
}

void Earth::Init()
{
    enableLight = true;
    readyToUse = 2.f;
    LightView = Vector3(0, 1, 0);
   

    // Set background color to dark blue
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Camera Init
    camera.Init(Vector3(0, 100, 0), Vector3(0, 100, 1));

    // Matrix Stack Init
    Mtx44 projection;

    if (static_cast<float>(Application::screenWidth) / static_cast<float>(Application::screenHeight) > 1.3334)
    {
        std::cout << Application::screenWidth << std::endl;
        std::cout << Application::screenHeight << std::endl;
        widescreen = true;
        projection.SetToPerspective(45.f, 16.f / 9.f, 0.1f, 10000.f);
    }
    else
    {
        projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
    }

    projectionStack.LoadMatrix(projection);

    //Enable depth buffer and depth testing
    glEnable(GL_DEPTH_TEST);

    //Enable back face culling
    glEnable(GL_CULL_FACE);

    //Default to fill mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Generate a default VAO for now
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
    m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
    m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
    m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
    m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
    m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
    m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
    m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

    m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
    m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
    m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
    m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
    m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
    m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
    m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
    m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
    m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
    m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
    m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
    m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
    m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

    m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
    m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
    m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
    m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
    glUseProgram(m_programID);

    light[0].position.Set(-10, 20, 20);
    light[0].color.Set(1, 1, 1);
    light[0].power = 1.f;
    light[0].kC = 1.f;
    light[0].kL = 0.01f;
    light[0].kQ = 0.001f;
    light[0].type = Light::LIGHT_DIRECTIONAL;
    light[0].cosCutoff = cos(Math::DegreeToRadian(45));
    light[0].cosInner = cos(Math::DegreeToRadian(30));
    light[0].exponent = 3.f;
    light[0].spotDirection.Set(0.f, 1.f, 0.f);

    glUniform1i(m_parameters[U_NUMLIGHTS], 2);
    glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
    glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
    glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
    glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
    glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
    glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
    glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
    glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
    glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

    meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

    meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("Lightball", Color(0, 1, 0), 18, 36);

    meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Front", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");

    meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

    meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");

    meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");

    meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");

    meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");

    meshList[GEO_TEXT] = MeshBuilder::GenerateText("Text", 16, 16);
    meshList[GEO_TEXT]->textureID = LoadTGA("Image//OCRA.tga");

    meshList[GEO_TITLESCREEN] = MeshBuilder::GenerateQuad("menu", Color(1, 1, 1), screenWidth, screenHeight);
    meshList[GEO_TITLESCREEN]->textureID = LoadTGA("Image//menu.tga");


}

void Earth::Update(double dt)
{

    if (Application::IsKeyPressed(VK_NUMPAD1)) //enable back face culling
        glEnable(GL_CULL_FACE);
    if (Application::IsKeyPressed(VK_NUMPAD2)) //disable back face culling
        glDisable(GL_CULL_FACE);
    if (Application::IsKeyPressed(VK_NUMPAD3))
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
    if (Application::IsKeyPressed(VK_NUMPAD4))
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode


    //Enable Light
    if (Application::IsKeyPressed('P') && readyToUse >= 3.f)
    {
        readyToUse = 0.f;
        if (enableLight)
            enableLight = false;
        else
            enableLight = true;
    }
    else if (readyToUse < 3.f)
        readyToUse += (float)(1 * dt);

    //Enable Axis
    if (Application::IsKeyPressed('X') && readyToUse >= 3.f)
    {
        readyToUse = 0.f;
        if (enableAxis)
            enableAxis = false;
        else
            enableAxis = true;
    }
    else if (readyToUse < 3.f)
        readyToUse += (float)(1 * dt);


}

void Earth::Render()
{
    // Render VBO here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    viewStack.LoadIdentity();
    viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
    modelStack.LoadIdentity();

    Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
    Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
    glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);

    if (enableAxis == true)
        RenderMesh(meshList[GEO_AXES], false);

}

void Earth::Exit()
{

    // Cleanup VBO here
    glDeleteVertexArrays(1, &m_vertexArrayID);
    glDeleteProgram(m_programID);
}


// Initializers



// Renders

//void Earth::renderSkybox()
//{
//
//    float skyboxSize = 5005;
//
//    modelStack.PushMatrix();
//
//    modelStack.Translate(playerShip.Pos.x, playerShip.Pos.y, playerShip.Pos.z);
//
//    modelStack.PushMatrix();
//    modelStack.Translate(0, 0, -skyboxSize / 2);
//    modelStack.Rotate(90, 1, 0, 0);
//    modelStack.Scale(skyboxSize + 5, skyboxSize + 5, skyboxSize + 5);
//    RenderMesh(meshList[GEO_FRONT], false);
//    modelStack.PopMatrix();
//
//    modelStack.PushMatrix();
//    modelStack.Translate(0, 0, skyboxSize / 2);
//    modelStack.Rotate(180, 0, 1, 0);
//    modelStack.Rotate(90, 1, 0, 0);
//    modelStack.Scale(skyboxSize + 5, skyboxSize + 5, skyboxSize + 5);
//    RenderMesh(meshList[GEO_BACK], false);
//    modelStack.PopMatrix();
//
//    modelStack.PushMatrix();
//    modelStack.Translate(-skyboxSize / 2, 0, 0);
//    modelStack.Rotate(90, 0, 1, 0);
//    modelStack.Rotate(90, 1, 0, 0);
//    modelStack.Scale(skyboxSize + 5, skyboxSize + 5, skyboxSize + 5);
//    RenderMesh(meshList[GEO_LEFT], false);
//    modelStack.PopMatrix();
//
//    modelStack.PushMatrix();
//    modelStack.Translate(skyboxSize / 2, 0, 0);
//    modelStack.Rotate(-90, 0, 1, 0);
//    modelStack.Rotate(90, 1, 0, 0);
//    modelStack.Scale(skyboxSize + 5, skyboxSize + 5, skyboxSize + 5);
//    RenderMesh(meshList[GEO_RIGHT], false);
//    modelStack.PopMatrix();
//
//    modelStack.PushMatrix();
//    modelStack.Translate(0, skyboxSize / 2, 0);
//    modelStack.Rotate(-90, 0, 1, 0);
//    modelStack.Rotate(180, 1, 0, 0);
//    modelStack.Rotate(180, 0, 1, 0);
//    modelStack.Scale(skyboxSize + 5, skyboxSize + 5, skyboxSize + 5);
//    RenderMesh(meshList[GEO_TOP], false);
//    modelStack.PopMatrix();
//
//    modelStack.PushMatrix();
//    modelStack.Translate(0, -skyboxSize / 2, 0);
//    modelStack.Rotate(90, 0, 1, 0);
//    modelStack.Scale(skyboxSize + 5, skyboxSize + 5, skyboxSize + 5);
//    RenderMesh(meshList[GEO_BOTTOM], false);
//    modelStack.PopMatrix();
//
//    modelStack.PopMatrix();
//}


//Updates



//Others



// Tools



void Earth::RenderMesh(Mesh* mesh, bool enableLight)
{
    Mtx44 MVP, modelView, modelView_inverse_transpose;

    MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    modelView = viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
    if (enableLight)
    {
        glUniform1i(m_parameters[U_LIGHTENABLED], 1);
        modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
        glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

        //load material
        glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
        glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
        glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
        glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
    }
    else
    {
        glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    }
    if (mesh->textureID > 0)
    {
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh->textureID);
        glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    }
    else
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);

    mesh->Render();

    if (mesh->textureID > 0)
        glBindTexture(GL_TEXTURE_2D, 0);

}

void Earth::RenderText(Mesh* mesh, std::string text, Color color)
{
    if (!mesh || mesh->textureID <= 0)
        return;

    glDisable(GL_DEPTH_TEST);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    for (unsigned i = 0; i < text.length(); ++i)
    {
        Mtx44 characterSpacing;
        characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
        Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

        mesh->Render((unsigned)text[i] * 6, 6);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
    glEnable(GL_DEPTH_TEST);
}

void Earth::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, float z)
{
    if (!mesh || mesh->textureID <= 0)
        return;

    glDisable(GL_DEPTH_TEST);

    Mtx44 ortho;
    ortho.SetToOrtho(0, screenWidth, 0, screenHeight, -100, 100); //size of screen UI
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity(); //No need camera for ortho mode
    modelStack.PushMatrix();
    modelStack.LoadIdentity(); //Reset modelStack
    modelStack.Translate(x, y, z);
    modelStack.Scale(size, size, size);

    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    for (unsigned i = 0; i < text.length(); ++i)
    {
        Mtx44 characterSpacing;
        characterSpacing.SetToTranslation((i * 0.5f) + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
        Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

        mesh->Render((unsigned)text[i] * 6, 6);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();

    glEnable(GL_DEPTH_TEST);
}

void Earth::RenderOnScreen(Mesh* mesh, float x, float y, float z, float size, float rotX, float rotY, float rotZ)
{
    Mtx44 ortho;
    ortho.SetToOrtho(0, screenWidth, 0, screenHeight, -100, 100); //size of screen UI
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity(); //No need camera for ortho mode
    modelStack.PushMatrix();
    modelStack.LoadIdentity(); //Reset modelStack
    modelStack.Translate(x, y, z);
    modelStack.Rotate(rotX, 1, 0, 0);
    modelStack.Rotate(rotY, 0, 1, 0);
    modelStack.Rotate(rotZ, 0, 0, 1);
    modelStack.Scale(size, size, size);

    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

    Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    mesh->Render();

    glBindTexture(GL_TEXTURE_2D, 0);

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
}

void Earth::RenderOnScreen(Mesh* mesh, Vector3 pos, float size, float rotX, float rotY, float rotZ)
{
    Mtx44 ortho;
    ortho.SetToOrtho(0, screenWidth, 0, screenHeight, -150, 100);//size of screen UI
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity(); //No need camera for ortho mode
    modelStack.PushMatrix();
    modelStack.LoadIdentity(); //Reset modelStack
    modelStack.Translate(pos.x, pos.y, pos.z);
    modelStack.Rotate(rotX, 1, 0, 0);
    modelStack.Rotate(rotY, 0, 1, 0);
    modelStack.Rotate(rotZ, 0, 0, 1);
    modelStack.Scale(size, size, size);

    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

    Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    mesh->Render();

    glBindTexture(GL_TEXTURE_2D, 0);

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
}

bool Earth::Timer(float second, double dt)
{
    this->second += dt;
    if (this->second > second)
    {
        this->second = 0;
        return true;
    }
    return false;
}

int Earth::generate_range(int from, int to)
{
    int k = (rand() % (to - from)) + from;
    return k;
}