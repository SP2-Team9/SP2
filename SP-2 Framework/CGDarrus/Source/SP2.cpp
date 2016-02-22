#include "SP2.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Utility.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"



SP2::SP2()
{
}

SP2::~SP2()
{

	for(vector<Vehicles*>::iterator it = allVehicles.begin(); it != allVehicles.end();)
	{
		delete *it;
		it = allVehicles.erase(it);
	}

	for (vector<Asteroid*>::iterator it = Vasteroid.begin(); it != Vasteroid.end();)
	{
		delete *it;
		it = Vasteroid.erase(it);
	}
	 for (vector<Bullet*>::iterator it = playerBullets.begin(); it != playerBullets.end(); it++){

        delete *it;
        it = playerBullets.erase(it);

    }

	delete selection;
	delete testShip;
}

void SP2::Init()
{
	enableLight = true;
	readyToUse = 2.f;
	LightView = Vector3(0, 1, 0);
	state = MainMenu;
	selection = nullptr;

	AmmoCount = 50;
	HealthPoints = 100;
    money = 1000;

	WorldHitboxInit();
	objectsInit();

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Camera Init
	camera.Init(Vector3(0, 100, 0), Vector3(0, 100, 1));

	// Matrix Stack Init
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
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

    meshList[GEO_BULLETS] = MeshBuilder::GenerateSphere("Bullets", Color(1, 0, 0), 18, 36);

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

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//OCRA.tga");

	meshList[GEO_TEXT1] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT1]->textureID = LoadTGA("Image//startfont.tga");

	meshList[GEO_XWING] = MeshBuilder::GenerateOBJ("spaceShip", "OBJ//Flying.obj");
	meshList[GEO_XWING]->textureID = LoadTGA("Image//flyingUV.tga");

	meshList[GEO_CONTROL_PANEL] = MeshBuilder::GenerateOBJ("Control Panel", "OBJ//Control Panel.obj");
	meshList[GEO_CONTROL_PANEL]->textureID = LoadTGA("Image//Control Panel.tga");

	meshList[GEO_SPACE_STATION] = MeshBuilder::GenerateOBJ("Space Station", "OBJ//Space Station.obj");
	meshList[GEO_SPACE_STATION]->textureID = LoadTGA("Image//Space Station.tga");

	meshList[GEO_NPC] = MeshBuilder::GenerateOBJ("NPChead", "OBJ//headnbody.obj");
    meshList[GEO_NPC]->textureID = LoadTGA("Image//headnbody_uv.tga");

    meshList[GEO_LEFTHAND] = MeshBuilder::GenerateOBJ("left hand", "OBJ//lefthand.obj");
    meshList[GEO_LEFTHAND]->textureID = LoadTGA("Image//arms_uv.tga");

    meshList[GEO_RIGHTHAND] = MeshBuilder::GenerateOBJ("right hand", "OBJ//righthand.obj");
    meshList[GEO_RIGHTHAND]->textureID = LoadTGA("Image//arms_uv.tga");

    meshList[GEO_LEFTLEG] = MeshBuilder::GenerateOBJ("left leg", "OBJ//leftleg.obj");
    meshList[GEO_LEFTLEG]->textureID = LoadTGA("Image//legs_uv.tga");

    meshList[GEO_RIGHTLEG] = MeshBuilder::GenerateOBJ("right leg", "OBJ//rightleg.obj");
    meshList[GEO_RIGHTLEG]->textureID = LoadTGA("Image//legs_uv.tga");

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("menu", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//menu.tga");

	meshList[GEO_ASTEROID] = MeshBuilder::GenerateSphere("Asteroid", Color(1, 1, 1), 10, 10);

    move = 0.f;
    rotate = 0.f;
    moveleg = 0.f;

}

void SP2::Update(double dt)
{
	camera.Update(dt);
	switch (state)
	{
	case MainMenu:
		camera.EnableCursor();
		if (Application::IsKeyPressed(VK_LBUTTON))
		{
			state = RTS;
			camera.PointAt(station, 100, 200);
		}
		break;
	case RTS:

		camera.EnableCursor();
		camera.YawRotation(dt);
		picker.set(camera, projectionStack.Top());
		picker.update();
		MouseSelection(dt);
        shipBulletCreation(dt);
        bulletUpdates(dt);
		if (Application::IsKeyPressed('E') && delay >= 1.f)
		{
			delay = 0;
			state = FPS;
			selection = nullptr;
			camera.Init(LastLocation.Pos, LastLocation.Pos + LastLocation.View);
			
		}
		vehicleUpdates(dt);
		checkHitboxes();
		break;

	case FPS:

		camera.DisableCursor();
		camera.FPSMovement(dt, worldHitbox);
		if (Application::IsKeyPressed('E') && delay >= 1.f)
		{
			if (Interactions[0].PointToAABB(camera.position))
			{
				delay = 0;
				state = TPS;
				LastLocation.SetPos(camera.position.x, camera.position.y, camera.position.z);
				LastLocation.SetView(camera.view.x, camera.view.y, camera.view.z);
				camera.PointAt(playerShip, 20, 30);
			}
			else
			{
				delay = 0;
				state = RTS;
				LastLocation.SetPos(camera.position.x, camera.position.y, camera.position.z);
				LastLocation.SetView(camera.view.x, camera.view.y, camera.view.z);
				camera.PointAt(station, 100, 200);
			}
		}
		vehicleUpdates(dt);
		checkHitboxes();
        NPCUpdates(dt);
		break;

	case TPS:
		camera.DisableCursor();
		camera.TPSMovement(dt, playerShip, worldHitbox);
		vehicleUpdates(dt);
		checkHitboxes();
		NPCUpdates(dt);
        playerBulletCreation(dt);
        bulletUpdates(dt);
		if (Application::IsKeyPressed('E') && delay >= 1.f)
		{
			if (playerShip.hitbox.AABBtoAABB(Interactions[1], playerShip.View))
			{
				delay = 0;
				state = FPS;
				camera.Init(LastLocation.Pos, LastLocation.Pos + LastLocation.View);
			}
		}
		break;
	}


    for (vector<Asteroid*>::iterator it = Vasteroid.begin(); it != Vasteroid.end(); ++it)
    {
        Asteroid* asteroid = *it;
        asteroid->update(dt);
    }

    if (Timer(1, dt) == true)
    {
        generateAsteroid();
    }



	delay += dt;
	if (Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if (Application::IsKeyPressed('4'))
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

	FPSText = std::to_string(toupper(1 / dt)) + " FPS";

	//ammo use
	Ammo = "Ammo: " + std::to_string(AmmoCount);
	if (Application::IsKeyPressed('Z') && AmmoCount != 0 && readyToUse >= 0.8f)
	{
		readyToUse = 0.f;
		AmmoCount--;
	}


	//health
	Health = "Health: " + std::to_string(HealthPoints);

    //money
    Money = "Money: $" + std::to_string(money);

	//Path finding test
	blinkDuration += dt;

    //buy health
    if (Application::IsKeyPressed(' ') && money != 0 && readyToUse >= 0.8f)
    {
        readyToUse = 0.f;
        money -= 100;
        HealthPoints += 5;
    }


	HBcheck = static_cast<HITBOXCHECK>((HBcheck + 1) % 3);
}

void SP2::Render()
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

	renderSkybox();
	renderAsteroid();

	//SpaceStation

	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_SPACE_STATION], enableLight);
	modelStack.PopMatrix();

	RenderTextOnScreen(meshList[GEO_TEXT], FPSText, Color(1, 0, 0), 3, 0, 0);

	renderExplosion();

	switch (state)
	{
	case MainMenu:
		renderTitleScreen();
		break;

	case RTS:

		if (Application::IsKeyPressed(VK_LBUTTON))
		{
			meshList[GEO_RAY] = MeshBuilder::GenerateLine("Ray", Vector3(0, 200, 0), picker.WorldCoord());
			modelStack.PushMatrix();
			RenderMesh(meshList[GEO_RAY], false);
			modelStack.PopMatrix();
		}

        renderShips();
        renderWayPoints();
		break;

	case FPS:
		renderNPC();
		renderShips();
		break;

	case TPS:
		renderShips();
		renderFightingUI();
		break;
	}

    renderBullets();
	renderAllHitbox();
	quests();
}

void SP2::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

// Initializers

void SP2::objectsInit()
{
	//Object Init
	station.SetPos(0, 0, 0);
	station.SetHitbox(AABB(-15, -25, -15, 15, 10, 15));

	LastLocation.SetPos(0, 1, 0);
	LastLocation.SetView(0, 0, 1);
	LastLocation.SetUp(0, 1, 0);
	LastLocation.SetRight(-1, 0, 0);

	NPC.SetPos(0, 0.1, 0);

	//Player Vehicle
	playerShip.SetPos(0, 50, 0);
	playerShip.SetView(0, 0, 1);
	playerShip.SetRight(-1, 0, 0);
	playerShip.SetHitboxSize(5);

	//Vehicles Init
    ship = new Vehicles(Vector3(100, 0, -100), Vector3(0, 0, 1), 20, 50);
	ship->SetHitboxSize(5);
    ship->SetInteractionSize(10, 10, 10, 10, 10, 10);
    ship->initialMoveDirection();

    boat = new Vehicles(Vector3(100, 0, -100), Vector3(1, 0, 0), 30, 50);
    boat->SetHitboxSize(5);
    boat->SetInteractionSize(10, 10, 10, 10, 10, 10);
    boat->initialMoveDirection();

	allVehicles.push_back(ship);
	allVehicles.push_back(boat);
}

void SP2::WorldHitboxInit()
{
	worldHitbox.push_back(AABB(Vector3(-10, -10, -10), Vector3(10, 0, 10)));
	worldHitbox.push_back(AABB(Vector3(-10, 8, -10), Vector3(10, 10, 10)));
	worldHitbox.push_back(AABB(Vector3(-10, 0, 10), Vector3(10, 10, 15)));
	worldHitbox.push_back(AABB(Vector3(-10, 0, -15), Vector3(10, 10, -10)));
	worldHitbox.push_back(AABB(Vector3(8, 0, -10), Vector3(15, 10, 10)));
	worldHitbox.push_back(AABB(Vector3(-15, 0, -10), Vector3(-8, 10, 10)));

	Interactions.push_back(AABB(-1, 0, -1, 1, 2, 1));
	Interactions.push_back(AABB(-20, 0, -20, 20, 20, 20));
}

void SP2::shipBulletCreation(double dt){


    if (Application::IsKeyPressed(VK_SPACE) && bulletCooldown > 0.3 && selection != nullptr){

        Mtx44 rotation;
        rotation.SetToRotation(selection->getRotationAngle(), 0, 1, 0);

        Vector3 view = rotation * selection->View;

        Bullet* newBullet = new Bullet(view, selection->Pos);

        playerBullets.push_back(newBullet);
        bulletCooldown = 0;

    }

    bulletCooldown += dt;

}

void SP2::playerBulletCreation(double dt){

    if (Application::IsKeyPressed(VK_LBUTTON) && bulletCooldown > 0.3){

        Bullet* newBullet = new Bullet(playerShip.View, playerShip.Pos);

        playerBullets.push_back(newBullet);
        bulletCooldown = 0;

    }

    bulletCooldown += dt;

}

void SP2::generateAsteroid()
{
	if (generate_range(0, 100) < 20)
	{
		Asteroid* asteroid = new Asteroid(generate_range(5, 30));
		asteroid->SetPos(generate_range(-400, 400), 0, generate_range(-400, 400));
		asteroid->SetView(Vector3(0, 0, 0) - asteroid->Pos);

		Vasteroid.push_back(asteroid);
	}
}


// Renders

void SP2::renderSkybox()
{
	modelStack.PushMatrix();
	float skyboxSize = 1005;


	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -skyboxSize / 2);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(skyboxSize + 5, skyboxSize + 5, skyboxSize + 5);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, skyboxSize / 2);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(skyboxSize + 5, skyboxSize + 5, skyboxSize + 5);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-skyboxSize / 2, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(skyboxSize + 5, skyboxSize + 5, skyboxSize + 5);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(skyboxSize / 2, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(skyboxSize + 5, skyboxSize + 5, skyboxSize + 5);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, skyboxSize / 2, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(skyboxSize + 5, skyboxSize + 5, skyboxSize + 5);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -skyboxSize / 2, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(skyboxSize + 5, skyboxSize + 5, skyboxSize + 5);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SP2::renderTitleScreen(){
	//start menu
	modelStack.PushMatrix();
	modelStack.Translate(0, 100, 420);
	modelStack.Rotate(270, 1, 0, 0);
	modelStack.Scale(500, 500, 500);
	RenderMesh(meshList[GEO_QUAD], false);
	modelStack.PopMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT1], "SPACE CONTROL", Color(0, 1, 0), 10, 0.1, 5);

	RenderTextOnScreen(meshList[GEO_TEXT], "Click to Start", Color(0, 1, 0), 3, 9.5, 7);
}

void SP2::renderFightingUI(){
	//Asteroid fighting
	RenderTextOnScreen(meshList[GEO_TEXT], Health, Color(0, 1, 0), 3, 0, 19);
	RenderTextOnScreen(meshList[GEO_TEXT], Ammo, Color(0, 1, 0), 3, 0, 18);
    RenderTextOnScreen(meshList[GEO_TEXT], Money, Color(0, 1, 0), 3, 0, 17);
}

void SP2::renderShips(){

	if (selection)
	{
		meshList[GEO_HITBOX] = MeshBuilder::GenerateCube("Hitbox", Color(0, 1, 0), selection->interaction.GetMin(), selection->interaction.GetMax());
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		modelStack.PushMatrix();
		RenderMesh(meshList[GEO_HITBOX], false);
		modelStack.PopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	for (vector<Vehicles*>::iterator vitV = allVehicles.begin(); vitV != allVehicles.end(); vitV++){

		Vehicles* currVehicle = *vitV;

		modelStack.PushMatrix();
		modelStack.Translate(currVehicle->Pos.x, currVehicle->Pos.y, currVehicle->Pos.z);
		modelStack.Rotate(currVehicle->Yaw, 0, 1, 0);
		RenderMesh(meshList[GEO_XWING], enableLight);
		modelStack.PopMatrix();

	}

	modelStack.PushMatrix();
	modelStack.Translate(playerShip.Pos.x, playerShip.Pos.y, playerShip.Pos.z);
	modelStack.Rotate(playerShip.pitch, playerShip.Right.x, 0, playerShip.Right.z);
	modelStack.Rotate(playerShip.yaw, 0, 1, 0);
	RenderMesh(meshList[GEO_XWING], enableLight);
	modelStack.PopMatrix();
}

void SP2::renderWayPoints(){


	for (vector<Vehicles*>::iterator vitV = allVehicles.begin(); vitV != allVehicles.end(); vitV++){

		Vehicles* currVehicle = *vitV;
		queue<Vector3> currVehicleQueue = currVehicle->newVehicle.getwayPoints();


		while (!currVehicleQueue.empty()){


			modelStack.PushMatrix();

			modelStack.Translate(currVehicleQueue.front().x, currVehicleQueue.front().y, currVehicleQueue.front().z);
			RenderMesh(meshList[GEO_LIGHTBALL], false);

			modelStack.PopMatrix();

			currVehicleQueue.pop();

		}

	}

}

void SP2::renderNPC()
{
    //NPC
    modelStack.PushMatrix();
	modelStack.Translate(NPC.Pos.x, NPC.Pos.y, NPC.Pos.z);

    modelStack.PushMatrix();
	modelStack.Scale(0.5, 0.5, 0.5);
    RenderMesh(meshList[GEO_NPC], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, 2.2, 0);
    modelStack.Rotate(rotate, 1, 0, 0);
    modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
    RenderMesh(meshList[GEO_LEFTHAND], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, 2.2, 0);
    modelStack.Rotate(-rotate, 1, 0, 0);
    modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
    RenderMesh(meshList[GEO_RIGHTHAND], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, 0.5, 0);
    modelStack.Rotate(moveleg, 1, 0, 0);
    modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
    RenderMesh(meshList[GEO_RIGHTLEG], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, 0.5, 0);
    modelStack.Rotate(-moveleg, 1, 0, 0);
    modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
    RenderMesh(meshList[GEO_LEFTLEG], false);
    modelStack.PopMatrix();

    modelStack.PopMatrix();
}

void SP2::renderExplosion()
{
	for (vector<Vector3>::iterator it = explosionPos.begin(); it != explosionPos.end(); ++it)
	{
		modelStack.PushMatrix();
		modelStack.Translate(it->x, it->y, it->z);
		modelStack.Rotate(ExplosionYaw, 0, 1, 0);
		modelStack.Rotate(ExplosionPitch, -1, 0, 0);
		modelStack.Scale(ExplosionSize, ExplosionSize, ExplosionSize);
		modelStack.PopMatrix();
	}
}

void SP2::renderAllHitbox()
{
	/*vector<AABB> allHitbox;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (vector<AABB>::iterator it = Interactions.begin(); it != Interactions.end(); ++it)
	{
		allHitbox.push_back(*it);
	}

	for (vector<AABB>::iterator it = worldHitbox.begin(); it != worldHitbox.end(); ++it)
	{
		allHitbox.push_back(*it);
	}

	for (vector<Asteroid*>::iterator it = Vasteroid.begin(); it != Vasteroid.end(); ++it)
	{
		Asteroid* asteroid = *it;
		allHitbox.push_back(asteroid->hitbox);
	}

	allHitbox.push_back(station.hitbox);
	allHitbox.push_back(playerShip.hitbox);

	for (vector<AABB>::iterator it = allHitbox.begin(); it != allHitbox.end(); ++it)
	{
		meshList[GEO_HITBOX] = MeshBuilder::GenerateCube("Hitbox", Color(0, 1, 0), it->GetMin(), it->GetMax());
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		modelStack.PushMatrix();
		RenderMesh(meshList[GEO_HITBOX], false);
		modelStack.PopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
}

void SP2::renderBullets(){

    for (vector<Bullet*>::iterator vitB = playerBullets.begin(); vitB != playerBullets.end(); vitB++){

        Bullet* temp = *vitB;

        modelStack.PushMatrix();

        modelStack.Translate(temp->Pos.x, temp->Pos.y, temp->Pos.z);
        RenderMesh(meshList[GEO_BULLETS], false);

        modelStack.PopMatrix();

    }

}

void SP2::renderAsteroid()
{
	for (vector<Asteroid*>::iterator it = Vasteroid.begin(); it != Vasteroid.end(); ++it)
	{
		Asteroid* asteroid = *it;
		modelStack.PushMatrix();
		modelStack.Translate(asteroid->Pos.x, asteroid->Pos.y, asteroid->Pos.z);
		modelStack.Scale(asteroid->size, asteroid->size, asteroid->size);
		RenderMesh(meshList[GEO_ASTEROID], enableLight);
		modelStack.PopMatrix();
	}
}

// Others

void SP2::vehicleUpdates(double dt){

	for (vector<Vehicles*>::iterator vitV = allVehicles.begin(); vitV != allVehicles.end(); vitV++){
		Vehicles* temp = *vitV;
		temp->update(dt);
	}

}

void SP2::NPCUpdates(double dt){

    //npc move
    move += (float)(8 * dt);
    if (rotate < 5 && restart == false)
    {
        rotate += (float)(20 * dt);


        if (rotate >= 5)
        {
            restart = true;

        }

    }
    if (rotate >= -5 && restart == true)
    {
        rotate -= (float)(20 * dt);
        if (rotate <= -5)
        {
            restart = false;


        }
    }
    if (moveleg < 5 && restart2 == false)
    {
        moveleg += (float)(20 * dt);


        if (moveleg >= 5)
        {
            restart2 = true;

        }

    }
    if (moveleg >= -5 && restart2 == true)
    {
        moveleg -= (float)(20 * dt);
        if (moveleg <= -5)
        {
            restart2 = false;


        }
    }


    HBcheck = static_cast<HITBOXCHECK>((HBcheck + 1) % 3);






}

void SP2::MouseSelection(double dt)
{
	if (Application::IsKeyPressed(VK_LBUTTON) && wayPointSetCoolDown > 0.5f)
	{
		Vector3 pressPosition = picker.WorldCoord();

		for (vector<Vehicles*>::iterator it = allVehicles.begin(); it != allVehicles.end(); ++it){

			Vehicles* temp = *it;
			if (temp->interaction.RayToAABB(camera.position, picker.getCurrentRay()))
			{
				selection = temp;

				break;
			}
			else
			{
				selection = nullptr;
			}
		}
		wayPointSetCoolDown = 0;
	}

	if (Application::IsKeyPressed(VK_RBUTTON) && selection != nullptr)
	{
		selection->setNewWayPoint(picker.WorldCoord().x, picker.WorldCoord().z);
		wayPointSetCoolDown = 0;
	}

	wayPointSetCoolDown += dt;
}

void SP2::checkHitboxes()
{
	switch (HBcheck)
	{
	case CheckStation:
		for (vector<Vehicles*>::iterator it = allVehicles.begin(); it != allVehicles.end();)
		{
			Vehicles* temp = *it;
			if (temp->hitbox.AABBtoAABB(station.hitbox, temp->View) == true)
			{
				it = allVehicles.erase(it);
				if (selection == temp)
				{
					selection = nullptr;
				}
			}
			else
				++it;
		}
		break;
	case CheckShips:

		break;
	case CheckAsteroids:
        for (vector<Asteroid*>::iterator vitA = Vasteroid.begin(); vitA != Vasteroid.end();){

            Asteroid* tempAst = *vitA;

            for (vector<Bullet*>::iterator vitB = playerBullets.begin(); vitB != playerBullets.end();){

                Bullet* tempBull = *vitB;

                if (tempAst->hitbox.PointToAABB(tempBull->Pos)){

                    vitB = playerBullets.erase(vitB);

                    tempAst->health -= tempBull->getBulletDamage();

                    delete tempBull;

                    std::cout << tempAst->health << std::endl;
                }
                else{

                    vitB++;

                }

            }

            if (tempAst->health <= 0){

                vitA = Vasteroid.erase(vitA);
                delete tempAst;

            }
            else{

                vitA++;

            }

        }

		break;
	
    }



}

void SP2::bulletUpdates(double dt){

    for (vector<Bullet*>::iterator vitB = playerBullets.begin(); vitB != playerBullets.end();){

        Bullet* temp = *vitB;

        temp->bulletUpdate(dt);

        if (temp->furtherThanBulletMaxRange()){

            vitB = playerBullets.erase(vitB);
            delete temp;

        }
        else{

            vitB++;

        }

    }

}

// Tools

void SP2::RenderMesh(Mesh* mesh, bool enableLight)
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

void SP2::RenderText(Mesh* mesh, std::string text, Color color)
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

void SP2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

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

void SP2::quests()
{
	if (NPC.Pos.z - camera.position.z  < 5.f)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "HI", Color(0, 1, 0), 10, 0, 5);
	}
}

bool SP2::Timer(float second, double dt)
{
	this->second += dt;
	if (this->second > second)
	{
		this->second = 0;
		return true;
	}
	return false;
}

int SP2::generate_range(int from, int to)
{
	return (rand() % (to - from)) + from;
}