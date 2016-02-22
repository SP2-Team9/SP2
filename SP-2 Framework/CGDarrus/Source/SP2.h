#ifndef SP2_H
#define SP2_H

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "MousePicker.h"
#include "Vehicles.h"
#include "pathFinding.h"
#include "PlayerVehicle.h"
#include "Object.h"
#include "Bullet.h"
#include "Asteroid.h"

#include <queue>
#include <vector>
#include <cstdlib>

using std::string;
using std::vector;
using std::queue;


class SP2 : public Scene
{
	enum GEOMETRY_TYPE
	{
        GEO_AXES,
        GEO_RAY,
        GEO_QUAD,
        GEO_LIGHTBALL,
        GEO_FRONT,
        GEO_BACK,
        GEO_LEFT,
        GEO_RIGHT,
        GEO_TOP,
        GEO_BOTTOM,
        GEO_TEXT,
        GEO_TEXT1,
        GEO_XWING,
        GEO_CONTROL_PANEL,
        GEO_SPACE_STATION,
        GEO_HITBOX,
        GEO_NPC,
        GEO_LEFTHAND,
        GEO_RIGHTHAND,
        GEO_LEFTLEG,
        GEO_RIGHTLEG,
        GEO_BULLETS,
        GEO_ASTEROID,
		NUM_GEOMETRY,

	};
	enum UNIFORM_TYPE
	{
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

	enum GAMESTATE
	{
		MainMenu = 0,
		FPS,
		RTS,
		TPS,
	};

	enum HITBOXCHECK
	{
		CheckStation = 0,
		CheckShips,
		CheckAsteroids,
	};

public:

	SP2();
	~SP2();

	virtual void Exit();
	virtual void Init();
	virtual void Render();
	virtual void Update(double dt);
	
	// Initializers
	void objectsInit();
	void WorldHitboxInit();
    void bulletCreation(double dt);
	void generateAsteroid();
    void shipBulletCreation(double dt);
    void playerBulletCreation(double dt);
	
	// Renders
    void renderNPC();
	void renderShips();
    void renderSkybox();
	void renderAsteroid();
    void renderBullets();
	void renderAllHitbox();
    void renderExplosion();
	void renderWayPoints();
	void renderFightingUI();
	void renderTitleScreen();

	// Others
	void quests();
    void checkHitboxes();
    void NPCUpdates(double dt);
    void bulletUpdates(double dt);
	void vehicleUpdates(double dt);
	void MouseSelection(double dt);
	


	// Tools
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	bool Timer(float second, double dt);
	int generate_range(int from, int to);


private:

    int money;
	int AmmoCount;
	int HealthPoints;

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

	double blinkDuration = 0;
    double bulletCooldown = 0;
	double wayPointSetCoolDown = 0;

    float move;
    float rotate;
    float moveleg;

    bool restart = false;
    bool restart2 = false;
	bool blink = false;
	bool re = false;
	float readyToUse, rotateAngle, ExplosionYaw, ExplosionPitch, ExplosionSize, delay, second;


 
	bool enableLight, enableAxis;

	GAMESTATE state;
	HITBOXCHECK HBcheck;

	MousePicker picker;

	Object station;
	Object LastLocation;
	Object NPC;


	Vehicles *ship;
	Vehicles *boat;
    Vehicles* testShip;
	Vehicles* selection;

	PlayerVehicle playerShip;

	string Ammo;
	string Health;
	string FPSText;
    string Money;

	vector<AABB> worldHitbox;
	vector<AABB> Interactions;
	vector<Vehicles*> allVehicles;
	vector<Vector3> explosionPos;
    vector<Bullet*> playerBullets;
	vector<Asteroid*> Vasteroid;


	MS modelStack, viewStack, projectionStack;
};


#endif