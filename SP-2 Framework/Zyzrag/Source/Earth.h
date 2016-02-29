#ifndef _EARTH_H
#define _EARTH_H


#include "Mesh.h"
#include "Shop.h"
#include "Light.h"
#include "Scene.h"
#include "Object.h"
#include "Bullet.h"
#include "Camera.h"
#include "Asteroid.h"
#include "Vehicles.h"
#include "Explosion.h"
#include "MatrixStack.h"
#include "MousePicker.h"
#include "pathFinding.h"
#include "PlayerVehicle.h"
#include "NPC.h"

#include <map>
#include <queue>
#include <vector>
#include <cstdlib>


using std::map;
using std::queue;
using std::stack;
using std::string;
using std::vector;

class Earth : public Scene
{
    enum GEOMETRY_TYPE {

        GEO_AXES,
        GEO_RAY,
        GEO_LIGHTBALL,
        GEO_SPHERE,
        GEO_TITLESCREEN,

        GEO_TEXT,
        GEO_FRONT,
        GEO_BACK,
        GEO_LEFT,
        GEO_RIGHT,
        GEO_TOP,
        GEO_BOTTOM,


        NUM_GEOMETRY,

    };
    enum UNIFORM_TYPE{

        U_MVP = 0,
        U_MODELVIEW,
        U_MODELVIEW_INVERSE_TRANSPOSE,
        U_MATERIAL_AMBIENT,
        U_MATERIAL_DIFFUSE,
        U_MATERIAL_SPECULAR,
        U_MATERIAL_SHININESS,

        U_LIGHT0_POSITION,
        U_LIGHT0_COLOR,
        U_LIGHT0_POWER,
        U_LIGHT0_KC,
        U_LIGHT0_KL,
        U_LIGHT0_KQ,
        U_LIGHT0_TYPE,
        U_LIGHT0_SPOTDIRECTION,
        U_LIGHT0_COSCUTOFF,
        U_LIGHT0_COSINNER,
        U_LIGHT0_EXPONENT,

        U_LIGHTENABLED,
        U_NUMLIGHTS,
        U_COLOR_TEXTURE_ENABLED,
        U_COLOR_TEXTURE,
        U_TEXT_ENABLED,
        U_TEXT_COLOR,
        U_TOTAL,

    };

    enum HITBOXCHECK
    {
        CheckStation = 0,
        CheckShips,
        CheckAsteroids,
    };



public:

    Earth();
    ~Earth();

    virtual void Exit();
    virtual void Init();
    virtual void Render();
    virtual void Update(double dt);

    // Initializers

    void objectsInit();
    void WorldHitboxInit();
   

    // Renders

    void renderAllTrees();
    void renderPlayerShip();
    

    // Updates


    //Others


    // Tools

    void RenderMesh(Mesh* mesh, bool enableLight);
    void RenderText(Mesh* mesh, std::string text, Color color);
    void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, float z = 0);
    void RenderOnScreen(Mesh* mesh, float x, float y, float z, float size, float rotX, float rotY, float rotZ);
    void RenderOnScreen(Mesh* mesh, Vector3 pos, float size, float rotX, float rotY, float rotZ);

    bool Timer(float second, double dt);

    int generate_range(int from, int to);


private:

    Camera camera;
    Light light[1];
    Vector3 LightView;
    Mesh *meshList[NUM_GEOMETRY];

    unsigned m_programID;
    unsigned m_vertexArrayID;
    unsigned m_parameters[U_TOTAL];
    unsigned m_colorBuffer[NUM_GEOMETRY];
    unsigned m_indexBuffer[NUM_GEOMETRY];
    unsigned m_vertexBuffer[NUM_GEOMETRY];

   

    bool enableLight, enableAxis, widescreen;

    float readyToUse, delay, second;

    // Shop variables
    float screenWidth;
    float screenHeight;
    Vector3 shopSmallPos, shopMidPos, shopLargePos, shopTarget;
    float objSize;
    float shopSmallScale, shopMidScale, shopLargeScale;
    float shopSmallRot, shopMidRot, shopLargeRot;

   
    MS modelStack, viewStack, projectionStack;



    vector<Vector3> allTrees;

};

#endif _EARTH_H