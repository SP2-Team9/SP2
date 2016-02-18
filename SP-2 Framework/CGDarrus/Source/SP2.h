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
#include "Object.h"

#include <queue>
#include <vector>

using std::string;
using std::vector;
using std::queue;


class SP2 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
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
	};

public:

	SP2();
	~SP2();

	virtual void Exit();
	virtual void Init();
	virtual void Render();
	virtual void Update(double dt);
	
	void objectsInit();
	void renderShips();
	void RenderSkybox();
	void renderFightingUI();
	void renderTitleScreen();
	void MouseSelection(double dt);
	void vehicleUpdates(double dt);
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

private:

	
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


	float readyToUse, rotateAngle, ExplosionYaw, ExplosionPitch, ExplosionSize;
	bool enableLight, enableAxis;
	
	GAMESTATE state;
	
	MousePicker picker;
	Object station;
	Vehicles ship;
	Vehicles boat;
	Vehicles* selection;

	string Ammo;
	string Health;
	string FPSText;

	vector<AABB> worldHitbox;
	vector<AABB> Interactions;
	vector<Vehicles*> allVehicles;
	
	MS modelStack, viewStack, projectionStack;

	
	

};


#endif