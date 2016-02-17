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
#include <queue>
#include <vector>

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
		GEO_OBJECT,
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

public:
	SP2();
	~SP2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderSkybox();
	void pathCheck();
	void renderTitleScreen();
	void renderFightingUI();
	void renderHealth();
	void MouseSelection(double dt);
	void objectsInit();

private:

	unsigned m_vertexArrayID;
	unsigned m_vertexBuffer[NUM_GEOMETRY];
	unsigned m_colorBuffer[NUM_GEOMETRY];
	unsigned m_indexBuffer[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	bool enableLight, enableAxis;
	float readyToUse, rotateAngle, ExplosionYaw, ExplosionPitch, ExplosionSize;
	Vector3 LightView;
	std::vector<AABB> Interactions;
	std::vector<AABB> worldHitbox;
	Object station;
	Vehicles ship;
	Vehicles* selection;

	Camera camera;
	MousePicker picker;
	Light light[1];

	Mesh *meshList[NUM_GEOMETRY];

	std::string FPSText;
	std::string Ammo;
	int a;
	std::string Health;
	int hp;
	bool blinking;
	bool renderStart;
	bool renderNext;
	bool start;
	bool sstart;

	MS modelStack, viewStack, projectionStack;

	double blinkDuration = 0;
	pathFinding spaceCraft;
	pathFinding xWing;

	double wayPointSetCoolDown = 0;
};


#endif