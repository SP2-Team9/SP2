#ifndef SP2_H
#define SP2_H


#include "Mesh.h"
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


#include <map>
#include <queue>
#include <vector>
#include <cstdlib>

using std::map;
using std::queue;
using std::string;
using std::vector;




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

        // DONT PUT ANYTHING INBETWEEN! FROM HERE
		GEO_SMALLSHIP,
        GEO_XWING,
		GEO_LARGESHIP,
        // TO HERE

        GEO_CONTROL_PANEL,
        GEO_SPACE_STATION,
		GEO_TELEPORTER,
        GEO_HITBOX,

        GEO_NPC,
        GEO_LEFTHAND,
        GEO_RIGHTHAND,
        GEO_LEFTLEG,
        GEO_RIGHTLEG,
        GEO_BULLETS,
        GEO_ASTEROID,
        GEO_EXPLOSION,
		GEO_INNERSTATION,

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
    void renderStation();
    void renderBullets();
	void renderAsteroid();
	void renderAllHitbox();
    void renderExplosion();
	void renderWayPoints();
	void renderFightingUI();
    void renderExplosions();
	void renderTitleScreen();


	// Others
	void quests();
    void checkHitboxes();
    void NPCUpdates(double dt);
    void bulletUpdates(double dt);
	void vehicleUpdates(double dt);
	void MouseSelection(double dt);
    void explosionUpdate(double dt);


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
	double wayPointSetCoolDown = 0;

    float move;
    float rotate;
    float moveleg;

    bool restart = false;
    bool restart2 = false;
	bool blink = false;
	bool re = false;

    double bulletCooldown = 0;

	float readyToUse, rotateAngle, ExplosionYaw, ExplosionPitch, ExplosionSize, delay, second;

	bool enableLight, enableAxis;

	GAMESTATE state;

	HITBOXCHECK HBcheck;

	MousePicker picker;

    Object NPC;
	Object station;
	Object LastLocation;
	

	string Ammo;
    string Money;
	string Health;
	string FPSText;
    

	map<int, vector<Vehicles*>> allVehicles;
	vector<Vehicles*> smallVehicles;
	vector<Vehicles*> largeVehicles;
	vector<Vehicles*> midVehicles;
	PlayerVehicle playerShip;
	Vehicles *midShip;
	Vehicles *smallShip;
	Vehicles *largeShip;
	Vehicles* selection;

	vector<AABB> worldHitbox;
	vector<AABB> Interactions;
    vector<Bullet*> allBullets;
	vector<Asteroid*> Vasteroid;
    vector<Explosion*> allExplosions;

	MS modelStack, viewStack, projectionStack;
};


#endif