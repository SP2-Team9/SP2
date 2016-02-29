#ifndef SP2_H
#define SP2_H

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
#include "sharedData.h"
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
using std::cout;
using std::endl;




class SP2 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_RAY,
		GEO_LIGHTBALL,
		GEO_SPHERE,
		GEO_TITLESCREEN,

		GEO_FRONT,
		GEO_BACK,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_TEXT,
		GEO_TEXT1,
		GEO_TEXT2,


		GEO_XWING,
		// DONT PUT ANYTHING INBETWEEN! FROM HERE
		GEO_SMALLSHIP,
		GEO_MIDSHIP,
		GEO_LARGESHIP,
		// TO HERE

		GEO_CONTROL_PANEL,
		GEO_SPACE_STATION,
		GEO_TELEPORTER,
		GEO_HITBOX,

		GEO_SHOPBACKDROP,
		GEO_ATTACKUP,
		GEO_FIRERATEUP,
		GEO_HEALTHUP,
		GEO_BUYBUTTON,

		GEO_NPC,
		GEO_NPC2,
		GEO_NPC3,
		GEO_NPC4,
		GEO_NPC5,
		GEO_NPC6,
		GEO_NPC7,
		GEO_LEFTHAND,
		GEO_RIGHTHAND,
		GEO_LEFTLEG,
		GEO_RIGHTLEG,
		GEO_BULLETS,
		GEO_ASTEROID,
		GEO_EXPLOSION,
		GEO_HEALTHBAR,
		GEO_INNERSTATION,
		GEO_SELECTION,

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
		RTS,
		inPlayerShip,
		inSpaceStation,
		inShop,
		exit,
	};

	enum HITBOXCHECK
	{
		CheckStation = 0,
		CheckShips,
		CheckAsteroids,
	};


	enum QUEST
	{
		noQuest = 0,
		ballQuest,
		asteroidQuest,
		buyshipQuest,
		abductionQuest,
	};

	enum SHOPSTATE
	{
		Main = 0,
		FirstShip,
		SecondShip,
		ThirdShip,
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
	void lightInit();
	void shopInit();
	void WorldHitboxInit();
    void bulletCreation(double dt);
	void generateAsteroid();
    void shipBulletCreation(double dt);
    void playerBulletCreation(double dt);
	
	// Renders
	void renderNPC();
	void renderNPC2();
	void renderNPC3();
	void renderNPC4();
	void renderNPC5();
	void renderShips();
    void renderSkybox();
    void renderStation();
    void renderBullets();
    void renderGeneral();
	void renderShopMenu();
	void renderAsteroid();
    void renderDistances();
	void renderAllHitbox();
    void renderExplosion();
	void renderWayPoints();
	void renderFightingUI();
    void renderExplosions();
	void renderTitleScreen();
    void renderHealthBar(Vector3 asteroidPosition, int asteroidSize, int health, Color color);

	// Updates

	void RTSUpdates(double dt);
	void NPCUpdates(double dt);
	void shopUpdates(double dt);
	void inShopUpdates(double dt);
	void bulletUpdates(double dt);
	void vehicleUpdates(double dt);
	void asteroidUpdate(double dt);
	void generalUpdates(double dt);
	void mainMenuUpdates(double dt);
	void explosionUpdate(double dt);
	void inPlayerShipUpdates(double dt);
	void inSpaceStationUpdates(double dt);


	//Others

    void quests();
    void ballquest();
    void asteroidquest();
	void buyshipquest();
	void abductionquest();
    void checkHitboxes();
	void shipHitboxCheck();
	void stationHitboxCheck();
	void asteroidHitboxCheck();
	void MouseSelection(double dt);

	// Tools

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, float z = 0);
	void RenderOnScreen(Mesh* mesh, float x, float y, float z, float size, float rotX, float rotY, float rotZ);
	void RenderOnScreen(Mesh* mesh, Vector3 pos, float size, float rotX, float rotY, float rotZ);

	bool Timer(float second, double dt);

	int generate_range(int from, int to);


private:

	int lastState;
    int currMoney;

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
	float count;

    bool restart = false;
    bool restart2 = false;
	bool blink = false;
	bool re = false;
	bool talking = 0;
	bool pickup = 0;
	bool complete = 0;
	bool complete2 = 0;
	bool complete3 = 0;
	bool complete4 = 0;
	bool complete5 = 0;
	

	int destroyed;

	bool enableLight, enableAxis, widescreen;
	
	float readyToUse, delay, second;

	// Shop variables
	float screenWidth;
	float screenHeight;
	Vector3 shopSmallPos, shopMidPos, shopLargePos, shopTarget;
	float objSize;
	float shopSmallScale, shopMidScale, shopLargeScale;
	float shopSmallRot, shopMidRot, shopLargeRot;

	bool hold;
	Vector3 initCursor, endCursor;

	SHOPSTATE shopState;

	GAMESTATE state;

	HITBOXCHECK HBcheck;

	MousePicker picker;

    Object NPC1;
	Object NPC2;
	Object NPC3;
	Object NPC4;
	Object NPC5;
	Object station;
	Object LastLocation;
	Object ball;

	string FPSText;
    
    Shop* playerShop;

	map<int, vector<Vehicles*>> allVehicles;
	map<int, stack<Vehicles*>> storedVehicles;

	vector<Vehicles*> smallVehicles;
	vector<Vehicles*> largeVehicles;
	vector<Vehicles*> midVehicles;

	stack<Vehicles*> stackSmallVehicles;
	stack<Vehicles*> stackLargeVehicles;
	stack<Vehicles*> stackMidVehicles;

	PlayerVehicle playerShip;

	Vehicles *midShip;
	Vehicles *smallShip;
	Vehicles *largeShip;

	Vehicles* selection;
	vector<Vehicles*> selectionTest;

	Vehicles* place;
	int placeType;

	vector<AABB> worldHitbox;
	vector<AABB> Interactions;
    vector<Bullet*> allBullets;
	vector<Asteroid*> Vasteroid;
    vector<Explosion*> allExplosions;

	MS modelStack, viewStack, projectionStack;

	QUEST currentQuest = noQuest;
};


#endif