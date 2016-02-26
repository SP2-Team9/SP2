#include "SP2.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Utility.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"



SP2::SP2(){



}

SP2::~SP2(){

    for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
    {
        vector<Vehicles*>::iterator it = allVehicles[i].begin();
        while (it != allVehicles[i].end())
        {
            Vehicles* Vtemp = *it;
            delete Vtemp;
            it = allVehicles[i].erase(it);
        }
    }

	for (vector<Asteroid*>::iterator it = Vasteroid.begin(); it != Vasteroid.end();)
	{
		delete *it;
		it = Vasteroid.erase(it);
	}
    for (vector<Bullet*>::iterator it = allBullets.begin(); it != allBullets.end(); it++){

        delete *it;
        it = allBullets.erase(it);

    }

	delete selection;

}

void SP2::Init()
{
	playerShop = new Shop(10, 10, 100, 0, 0, 0);

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
	
	if (static_cast<float>(Application::screenWidth) / static_cast<float>(Application::screenHeight) > 1.3334)
	{
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

    meshList[GEO_BULLETS] = MeshBuilder::GenerateSphere("Bullets", Color(1, 0.5, 0), 18, 36);

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

	meshList[GEO_TEXT2] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT2]->textureID = LoadTGA("Image//OCRA.tga");

	// Ships
	meshList[GEO_XWING] = MeshBuilder::GenerateOBJ("spaceShip", "OBJ//Flying.obj");
	meshList[GEO_XWING]->textureID = LoadTGA("Image//flyingUV.tga");

	meshList[GEO_SMALLSHIP] = MeshBuilder::GenerateOBJ("Small Ship", "OBJ//smallship.obj");
	meshList[GEO_SMALLSHIP]->textureID = LoadTGA("Image//smallship.tga");

	meshList[GEO_LARGESHIP] = MeshBuilder::GenerateOBJ("Large Ship", "OBJ//largeship.obj");
	meshList[GEO_LARGESHIP]->textureID = LoadTGA("Image//largeship.tga");


	// Station
	meshList[GEO_CONTROL_PANEL] = MeshBuilder::GenerateOBJ("Control Panel", "OBJ//Control Panel.obj");
	meshList[GEO_CONTROL_PANEL]->textureID = LoadTGA("Image//Control Panel.tga");

	meshList[GEO_SPACE_STATION] = MeshBuilder::GenerateOBJ("Space Station", "OBJ//Space Station.obj");
	meshList[GEO_SPACE_STATION]->textureID = LoadTGA("Image//Space Station.tga");

	meshList[GEO_TELEPORTER] = MeshBuilder::GenerateOBJ("Asteroid", "OBJ//teleporter.obj");
	meshList[GEO_TELEPORTER]->textureID = LoadTGA("Image//teleporter.tga");

	// NPC
	meshList[GEO_NPC] = MeshBuilder::GenerateOBJ("NPChead", "OBJ//headnbody.obj");
	meshList[GEO_NPC]->textureID = LoadTGA("Image//headnbody_uv.tga");

	meshList[GEO_NPC2] = MeshBuilder::GenerateOBJ("NPChead", "OBJ//headnbody.obj");
	meshList[GEO_NPC2]->textureID = LoadTGA("Image//headnbody_uv2.tga");

	meshList[GEO_NPC3] = MeshBuilder::GenerateOBJ("NPChead", "OBJ//headnbody.obj");
	meshList[GEO_NPC3]->textureID = LoadTGA("Image//headnbody_uv3.tga");

	meshList[GEO_NPC4] = MeshBuilder::GenerateOBJ("NPChead", "OBJ//headnbody.obj");
	meshList[GEO_NPC4]->textureID = LoadTGA("Image//headnbody_uv4.tga");

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

	meshList[GEO_ASTEROID] = MeshBuilder::GenerateOBJ("Asteroid", "OBJ//asteroid.obj");
	meshList[GEO_ASTEROID]->textureID = LoadTGA("Image//AM3.tga");

	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("ball", Color(1, 1, 1), 10, 20);


	meshList[GEO_INNERSTATION] = MeshBuilder::GenerateOBJ("Test", "OBJ//innerstation.obj");
	meshList[GEO_INNERSTATION]->textureID = LoadTGA("Image//innerstation.tga");

    move = 0.f;
    rotate = 0.f;
    moveleg = 0.f;
	count = 0.f;
	destroyed = 0;

    meshList[GEO_EXPLOSION] = MeshBuilder::GenerateQuad("Explosion", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_EXPLOSION]->textureID = LoadTGA("Image//explosion.tga");
}

void SP2::Update(double dt)
{
	camera.Update(dt);

	switch (state){
	case MainMenu:

		mainMenuUpdates(dt);

		break;

	case RTS:

		RTSUpdates(dt);

		break;

	case inSpaceStation:

		inSpaceStationUpdates(dt);

		break;

	case inPlayerShip:

		inPlayerShipUpdates(dt);

		break;

	}

	if (state != MainMenu){

		generalUpdates(dt);

	}

	

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
	renderStation();
	renderAsteroid();
	
	RenderTextOnScreen(meshList[GEO_TEXT], FPSText, Color(1, 0, 0), 3, 0, 0);

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

	case inSpaceStation:
		renderNPC();
		renderNPC2();
		renderNPC3();
		renderNPC4();
		renderShips();
		break;

	case inPlayerShip:

		renderShips();
		renderFightingUI();

		break;

	}

    quests();
	ballquest();
	asteroidquest();
	buyshipquest();
	abductionquest();
    renderBullets();
	renderAllHitbox();
	renderExplosion();
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

	NPC1.SetPos(3, 0, 1);
	NPC1.SetHitboxSize(2);

	NPC2.SetPos(-3, 0, 7);
	NPC2.SetHitboxSize(4);

	NPC3.SetPos(4, 0, -4);
	NPC3.SetHitboxSize(4);

	NPC4.SetPos(-5, 0, -1);
	NPC4.SetHitboxSize(4);

	NPC5.SetPos(-5, 0, -5);
	NPC5.SetHitboxSize(4);

	ball.SetPos(4, 0.1, 7);
	ball.SetHitboxSize(2);

	//Player Vehicle
	playerShip.SetPos(0, 50, 0);
	playerShip.SetView(0, 0, 1);
	playerShip.SetRight(-1, 0, 0);
	playerShip.SetHitboxSize(5);

	//Vehicles Init
	allVehicles.insert(std::pair<int, vector<Vehicles*>>(GEO_SMALLSHIP, smallVehicles));
	allVehicles.insert(std::pair<int, vector<Vehicles*>>(GEO_XWING, midVehicles));
	allVehicles.insert(std::pair<int, vector<Vehicles*>>(GEO_LARGESHIP, largeVehicles));

    smallShip = new Vehicles(Vector3(0, 0, -50), Vector3(0, 0, -1), 50, 20, 500, 10);
    smallShip->SetHitboxSize(10);
    smallShip->SetInteractionSize(10, 10, 10, 10, 10, 10);

	midShip = new Vehicles(Vector3(0, 0, 50), Vector3(0, 0, 1), 10, 20, 50, 100);
	midShip->SetHitboxSize(20);
	midShip->SetInteractionSize(20, 10, 20, 20, 10, 20);

    allVehicles[GEO_SMALLSHIP].push_back(smallShip);
	allVehicles[GEO_XWING].push_back(midShip);
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

    for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
    {
        vector<Vehicles*>::iterator vitV = allVehicles[i].begin();
        while (vitV != allVehicles[i].end())
        {
            Vehicles* temp = *vitV;

            if (temp->fireBullets(dt) == true){

                Vector3 view = temp->currAttackTarget->Pos - temp->Pos;
                view.Normalize();

                Bullet* newBullet = new Bullet(view, temp->Pos, temp->bulletDamage);

                allBullets.push_back(newBullet);

            }

            ++vitV;
        }
    }

}

void SP2::playerBulletCreation(double dt){

    playerShip.update(dt);
   
    if (Application::IsKeyPressed(VK_LBUTTON)){

        if (playerShip.fireBullets(playerShop->playerShipFireRate)){

            Bullet* newBullet = new Bullet(playerShip.View, playerShip.Pos, playerShop->playerShipDamage);

            allBullets.push_back(newBullet);

        }

    }

}

void SP2::generateAsteroid()
{
	if (generate_range(0, 100) < 20)
	{
		Asteroid* asteroid = new Asteroid(generate_range(5, 30));
		switch (generate_range(0, 4))
		{
		case 0:
			asteroid->SetPos(generate_range(-400, 400), 0, generate_range(400, 500));
			asteroid->SetView(Vector3(generate_range(-400, 400), 0, -500));
			break;
		case 1:
			asteroid->SetPos(generate_range(-400, 400), 0, generate_range(-400, -500));
			asteroid->SetView(Vector3(generate_range(-400, 400), 0, 500));
			break;
		case 2:
			asteroid->SetPos(generate_range(400, 500), 0, generate_range(-400, 400));
			asteroid->SetView(Vector3(-500, 0, generate_range(-400, 400)));
			break;
		case 3:
			asteroid->SetPos(generate_range(-400, -500), 0, generate_range(-400, 400));
			asteroid->SetView(Vector3(500, 0, generate_range(-400, 400)));
			break;
		}


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

void SP2::renderStation()
{
	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_SPACE_STATION], enableLight);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -0.3f, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_INNERSTATION], enableLight);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(1.6f, 1.6f, 1.6f);
	RenderMesh(meshList[GEO_TELEPORTER], enableLight);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Scale(0.7f, 0.7f, 0.7f);
	modelStack.Translate(0, -0.5f, 10);
	RenderMesh(meshList[GEO_CONTROL_PANEL], enableLight);
	modelStack.PopMatrix();
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

	for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
	{
		vector<Vehicles*>::iterator it = allVehicles[i].begin();
		while (it != allVehicles[i].end())
		{
			Vehicles* Vtemp = *it;
			modelStack.PushMatrix();
			modelStack.Translate(Vtemp->Pos.x, Vtemp->Pos.y, Vtemp->Pos.z);
			modelStack.Rotate(Vtemp->Yaw, 0, 1, 0);
			RenderMesh(meshList[i], enableLight);
			modelStack.PopMatrix();

			it++;
		}
	}

	modelStack.PushMatrix();
	modelStack.Translate(playerShip.Pos.x, playerShip.Pos.y, playerShip.Pos.z);
	modelStack.Rotate(playerShip.pitch, playerShip.Right.x, 0, playerShip.Right.z);
	modelStack.Rotate(playerShip.yaw, 0, 1, 0);
	RenderMesh(meshList[GEO_XWING], enableLight);
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

void SP2::renderWayPoints(){
	for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
	{
		vector<Vehicles*>::iterator it = allVehicles[i].begin();
		while (it != allVehicles[i].end())
		{
			Vehicles* Vtemp = *it;
			queue<Vector3> currVehicleQueue = Vtemp->newVehicle.getwayPoints();

			while (!currVehicleQueue.empty()){


				modelStack.PushMatrix();

				modelStack.Translate(currVehicleQueue.front().x, currVehicleQueue.front().y, currVehicleQueue.front().z);
				RenderMesh(meshList[GEO_LIGHTBALL], false);

				modelStack.PopMatrix();

				currVehicleQueue.pop();

			}

			it++;
		}
	}

}

void SP2::renderAllHitbox()
{
	/*vector<AABB> allHitbox;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/

	/*for (vector<AABB>::iterator it = Interactions.begin(); it != Interactions.end(); ++it)
	{
		allHitbox.push_back(*it);
	}

	for (vector<AABB>::iterator it = worldHitbox.begin(); it != worldHitbox.end(); ++it)
	{
		allHitbox.push_back(*it);
	}*/

	/*for (vector<Asteroid*>::iterator it = Vasteroid.begin(); it != Vasteroid.end(); ++it)
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

    for (vector<Bullet*>::iterator vitB = allBullets.begin(); vitB != allBullets.end(); vitB++){

        Bullet* temp = *vitB;

        modelStack.PushMatrix();

        modelStack.Translate(temp->Pos.x, temp->Pos.y, temp->Pos.z);
        modelStack.Scale(1.5f, 1.f, 1.5f);
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

void SP2::renderExplosion()
{
    for (vector<Explosion*>::iterator vitE = allExplosions.begin(); vitE != allExplosions.end(); ++vitE){
        
        Explosion* currExplosion = *vitE;
        modelStack.PushMatrix();
		modelStack.Translate(currExplosion->Pos.x, currExplosion->Pos.y, currExplosion->Pos.z);
        modelStack.Rotate(currExplosion->getYaw(), 0, 1, 0);
        modelStack.Rotate(currExplosion->getPitch(), 0, 0, 1);
        modelStack.Scale(currExplosion->getExplosionSize(), currExplosion->getExplosionSize(), currExplosion->getExplosionSize());
        RenderMesh(meshList[GEO_EXPLOSION], false);
        modelStack.PopMatrix();

     
    }

}


//Updates

void SP2::NPCUpdates(double dt){

	//npc move
	if (talking == false)
	{

		if (move < 1 && re == false)
		{
			move += (float)(1 * dt);


			if (move >= 1)
			{
				re = true;

			}

		}
		if (move >= -1 && re == true)
		{
			move -= (float)(1 * dt);
			if (move <= -1)
			{
				re = false;


			}
		}

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
	}

	HBcheck = static_cast<HITBOXCHECK>((HBcheck + 1) % 3);





}

void SP2::RTSUpdates(double dt){

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
		state = inSpaceStation;
		selection = nullptr;
		camera.Init(LastLocation.Pos, LastLocation.Pos + LastLocation.View);

	}


}

void SP2::vehicleUpdates(double dt){
	for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
	{
		vector<Vehicles*>::iterator it = allVehicles[i].begin();
		while (it != allVehicles[i].end())
		{
			Vehicles* Vtemp = *it;
			Vtemp->update(dt);

			it++;
		}
	}
}

void SP2::bulletUpdates(double dt){

    for (vector<Bullet*>::iterator vitB = allBullets.begin(); vitB != allBullets.end();){

        Bullet* temp = *vitB;

        temp->bulletUpdate(dt);
        if (temp->furtherThanBulletMaxRange()){

            vitB = allBullets.erase(vitB);
            
            delete temp;

        }
        else{

            vitB++;

        }

    }

}

void SP2::asteroidUpdate(double dt){

	for (vector<Asteroid*>::iterator it = Vasteroid.begin(); it != Vasteroid.end(); ++it)
	{
		Asteroid* asteroid = *it;
		asteroid->update(dt);
	}

	if (Timer(1, dt) == true)
	{
		generateAsteroid();
	}

}

void SP2::generalUpdates(double dt){

	asteroidUpdate(dt);
	bulletUpdates(dt);
	explosionUpdate(dt);
	vehicleUpdates(dt);
	checkHitboxes();
	delay += dt;

}

void SP2::mainMenuUpdates(double dt){

	camera.EnableCursor();
	if (Application::IsKeyPressed(VK_LBUTTON))
	{
		state = RTS;
		camera.PointAt(station, 100, 200);
	}


}

void SP2::explosionUpdate(double dt){

    for (vector<Explosion*>::iterator vitE = allExplosions.begin(); vitE != allExplosions.end();){

        Explosion* temp = *vitE;

		float explosionYaw = 0;
		float explosionPitch = 0;
		Vector3 initView(0, 1, 0);
		Vector3 view = (camera.position - camera.target).Normalized();
		Vector3 XZview(view.x, 0, view.z);
		XZview.Normalize();

		explosionPitch = Math::RadianToDegree(acos(initView.Dot(view)));
		Vector3 V3 = initView.Cross(view);
		if (V3.Dot(Vector3(0, 0, 1)) < 0)
		{
			explosionPitch *= -1;
		}

		Mtx44 rotation;
		rotation.SetToRotation(explosionPitch, 0, 0, 1);
		initView = rotation * initView;
		initView.Set(initView.x, 0, initView.z);
		initView.Normalize();

		explosionYaw = Math::RadianToDegree(acos(initView.Dot(XZview)));
		V3 = initView.Cross(XZview);
		if (V3.Dot(Vector3(0, 1, 0)) < 0)
		{
			explosionYaw *= -1;
		}
		
        temp->setPitchandYaw(explosionPitch, explosionYaw);
        temp->explosionUpdate(dt);

        if (temp->explosionEnd()){

            vitE = allExplosions.erase(vitE);
            delete temp;

        }
        else{

            vitE++;

		}

    }
}

void SP2::inPlayerShipUpdates(double dt){

	camera.DisableCursor();
	camera.TPSMovement(dt, playerShip, worldHitbox);
	playerBulletCreation(dt);
	if (Application::IsKeyPressed('E') && delay >= 1.f)
	{
		if (playerShip.hitbox.AABBtoAABB(Interactions[1], playerShip.View))
		{
			delay = 0;
			state = inSpaceStation;
			camera.Init(LastLocation.Pos, LastLocation.Pos + LastLocation.View);
		}
	}

}

void SP2::inSpaceStationUpdates(double dt){

	camera.DisableCursor();
	camera.FPSMovement(dt, worldHitbox);
	if (Application::IsKeyPressed('E') && delay >= 1.f)
	{
		if (Interactions[0].PointToAABB(camera.position))
		{
			delay = 0;
			state = inPlayerShip;
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

	NPCUpdates(dt);

}



//Others

void SP2::checkHitboxes(){
	switch (HBcheck){

	case CheckStation:

		stationHitboxCheck();

		break;

	case CheckShips:
		
		shipHitboxCheck();

		break;

	case CheckAsteroids:

		asteroidHitboxCheck();
	
		break;
	}



}

void SP2::shipHitboxCheck(){
	// Ship to Ship
	for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
	{
		vector<Vehicles*>::iterator V1it = allVehicles[i].begin();

		while (V1it != allVehicles[i].end())
		{
			Vehicles* Veh1 = *V1it;
			for (int j = GEO_SMALLSHIP; j <= GEO_LARGESHIP; ++j)
			{
				vector<Vehicles*>::iterator V2it = allVehicles[j].begin();
				while (V2it != allVehicles[j].end())
				{
					Vehicles* Veh2 = *V2it;
					if (Veh1->hitbox.AABBtoAABB(Veh2->hitbox) == true && Veh1 != Veh2)
					{
						Vector3 ExploCenter = Veh1->Pos + Veh2->Pos;
						ExploCenter /= 2;
						allExplosions.push_back(new Explosion(100, 50, ExploCenter));
						Veh1->isDead = true;
						if (selection == Veh1 || selection == Veh2)
							selection = nullptr;
						delete Veh2;
						V2it = allVehicles[j].erase(V2it);
					}
					else
					{
						V2it++;
					}
				}
			}

			if (Veh1->isDead == true)
			{
				delete Veh1;
				V1it = allVehicles[i].erase(V1it);
			}
			else
			{
				V1it++;
			}
		}
	}

	//Player Ship to Ship
	for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
	{
		vector<Vehicles*>::iterator it = allVehicles[i].begin();
		while (it != allVehicles[i].end())
		{
			Vehicles* Vtemp = *it;
			if (Vtemp->hitbox.AABBtoAABB(playerShip.hitbox))
			{
				if (selection == Vtemp)
				{
					selection = nullptr;
				}
				delete Vtemp;
				it = allVehicles[i].erase(it);
			}
			else
				++it;
		}
	}
}

void SP2::stationHitboxCheck(){

	// Vehicles to Station
	for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
	{
		vector<Vehicles*>::iterator it = allVehicles[i].begin();
		while (it != allVehicles[i].end())
		{
			Vehicles* Vtemp = *it;
			if (Vtemp->hitbox.AABBtoAABB(station.hitbox, Vtemp->View) == true)
			{
				allExplosions.push_back(new Explosion(100, 50, Vtemp->Pos));
				it = allVehicles[i].erase(it);
				if (selection == Vtemp)
				{
					selection = nullptr;
				}
				delete Vtemp;
			}
			else
				++it;
		}
	}

	// Asteroid to Station
	for (vector<Asteroid*>::iterator it = Vasteroid.begin(); it != Vasteroid.end();)
	{
		Asteroid* tempAst = *it;
		if (tempAst->hitbox.AABBtoAABB(station.hitbox, tempAst->View) == true)
		{
			for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
			{
				vector<Vehicles*>::iterator vitV = allVehicles[i].begin();
				while (vitV != allVehicles[i].end())
				{
					Vehicles* temp = *vitV;
					if (temp->currAttackTarget == tempAst){

						temp->currAttackTarget = nullptr;

					}

					vitV++;
				}
			}
			delete tempAst;
			it = Vasteroid.erase(it);
		}
		else
			it++;
	}



}

void SP2::asteroidHitboxCheck(){

	//Bullet To Asteroid
	for (vector<Asteroid*>::iterator vitA = Vasteroid.begin(); vitA != Vasteroid.end();){

		Asteroid* tempAst = *vitA;

		for (vector<Bullet*>::iterator vitB = allBullets.begin(); vitB != allBullets.end();){

			Bullet* tempBull = *vitB;

			if (tempAst->hitbox.PointToAABB(tempBull->Pos)){

				vitB = allBullets.erase(vitB);

				tempAst->health -= tempBull->getBulletDamage();

				delete tempBull;

				std::cout << tempAst->health << std::endl;
			}
			else{

				vitB++;

			}

		}

		if (tempAst->health <= 0){

			for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
			{
				vector<Vehicles*>::iterator it = allVehicles[i].begin();

				while (it != allVehicles[i].end())
				{

					Vehicles* temp = *it;

					if (temp->currAttackTarget == tempAst){

						temp->currAttackTarget = nullptr;

					}

					it++;
				}
			}
			vitA = Vasteroid.erase(vitA);
			delete tempAst;
			destroyed++;
		}
		else{

			vitA++;

		}

	}

	//Vehicles to Asteroid
	for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
	{
		vector<Vehicles*>::iterator Vit = allVehicles[i].begin();
		while (Vit != allVehicles[i].end())
		{
			Vehicles* tempVeh = *Vit;
			for (vector<Asteroid*>::iterator Ait = Vasteroid.begin(); Ait != Vasteroid.end();)
			{
				Asteroid* tempAst = *Ait;
				if (tempVeh->hitbox.AABBtoAABB(tempAst->hitbox))
				{
					tempVeh->isDead = true;
					delete tempAst;
					Ait = Vasteroid.erase(Ait);
				}
				else
				{
					Ait++;
				}
			}

			if (tempVeh->isDead == true)
			{

				allExplosions.push_back(new Explosion(100, 50, tempVeh->Pos));
				if (selection == tempVeh)
				{
					selection = nullptr;
				}
				delete tempVeh;
				Vit = allVehicles[i].erase(Vit);
			}
			else
			{
				Vit++;
			}
		}
	}

	// Player Ship to Asteroid
	for (vector<Asteroid*>::iterator Ait = Vasteroid.begin(); Ait != Vasteroid.end();)
	{
		Asteroid* tempAst = *Ait;
		if (tempAst->hitbox.AABBtoAABB(playerShip.hitbox))
		{
			delete tempAst;
			Ait = Vasteroid.erase(Ait);
		}
		else
			Ait++;
	}

	//Asteroid to Asteroid
	for (vector<Asteroid*>::iterator A1it = Vasteroid.begin(); A1it != Vasteroid.end();)
	{
		Asteroid* temp1Ast = *A1it;
		for (vector<Asteroid*>::iterator A2it = Vasteroid.begin(); A2it != Vasteroid.end();)
		{
			Asteroid* temp2Ast = *A2it;
			if (temp1Ast->hitbox.AABBtoAABB(temp2Ast->hitbox) && temp1Ast != temp2Ast)
			{
				std::cout << "HIT" << std::endl;
				if (temp1Ast->size > 10)
				{
					temp1Ast->size /= 2;
					temp1Ast->speed /= 2;
					temp1Ast->health /= 2;
				}
				else
				{
					temp1Ast->boom = true;
				}

				if (temp2Ast->size > 20)
				{
					temp2Ast->size /= 2;
					temp2Ast->speed *= 2;
					temp2Ast->health /= 2;
				}
				else
				{
					delete temp2Ast;
					A2it = Vasteroid.erase(A2it);
				}
			}
			else
			{
				A2it++;
			}
		}

		if (temp1Ast->boom == true)
		{
			delete temp1Ast;
			A1it = Vasteroid.erase(A1it);
		}
		else
		{
			A1it++;
		}
	}


}

void SP2::MouseSelection(double dt)
{
	if (Application::IsKeyPressed(VK_LBUTTON) && wayPointSetCoolDown > 0.5f)
	{
		Vector3 pressPosition = picker.WorldCoord();
		for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
		{
			bool Bselected = false;
			vector<Vehicles*>::iterator it = allVehicles[i].begin();
			while (it != allVehicles[i].end())
			{
				Vehicles* Vtemp = *it;
				if (Vtemp->interaction.RayToAABB(camera.position, picker.getCurrentRay()))
				{
					selection = Vtemp;
					Bselected = true;
					break;
				}
				else
				{
					selection = nullptr;
				}

				it++;
			}

			if (Bselected == true)
				break;
		}
		wayPointSetCoolDown = 0;
	}

	if (Application::IsKeyPressed(VK_RBUTTON) && selection != nullptr)
	{


		for (vector<Asteroid*>::iterator vitA = Vasteroid.begin(); vitA != Vasteroid.end();){

			Asteroid* temp = *vitA;
			if (temp->hitbox.RayToAABB(camera.position, picker.getCurrentRay())){

				selection->currAttackTarget = temp;
				break;

			}
			else{

				selection->currAttackTarget = nullptr;
				vitA++;

			}

		}

		if (selection->currAttackTarget == nullptr){

			selection->setNewWayPoint(picker.WorldCoord().x, picker.WorldCoord().z);

		}
		else
		{
			selection->setNewWayPoint(selection->currAttackTarget->Pos.x, selection->currAttackTarget->Pos.z);

		}

	}

	wayPointSetCoolDown += dt;
}

void SP2::quests()
{
	if (NPC1.hitbox.PointToAABB(camera.position) && Application::IsKeyPressed(VK_LBUTTON) && complete == false)
	{
		talking = true;
		RenderTextOnScreen(meshList[GEO_TEXT2], "GIMME BALL I NEED BALL", Color(0, 1, 0), 3, 0, 5);
		
		currentQuest = ballQuest;
	
	}
	else
	{
		talking = false;
	}
	if (Application::IsKeyPressed(VK_LBUTTON) && NPC2.hitbox.PointToAABB(camera.position) && complete2 == false)
	{
		talking = true;
		RenderTextOnScreen(meshList[GEO_TEXT2], "Destroy 5 asteroids for me", Color(0, 1, 0), 3, 0, 5);
		
		currentQuest = asteroidQuest;
		
		
	}

	if (Application::IsKeyPressed(VK_LBUTTON) && NPC3.hitbox.PointToAABB(camera.position) && complete3 == false)
	{
		talking = true;
		RenderTextOnScreen(meshList[GEO_TEXT2], "Buy duh 3 powderful shipzz", Color(0, 1, 0), 3, 0, 5);

		currentQuest = buyshipQuest;
	

	}

	if (Application::IsKeyPressed(VK_LBUTTON) && NPC4.hitbox.PointToAABB(camera.position) && complete4 == false)
	{
		talking = true;
		RenderTextOnScreen(meshList[GEO_TEXT2], "gimmeh 1000 bucks, gotta kidnap sum alien kidz", Color(0, 1, 0), 3, 0, 5);

		currentQuest = abductionQuest;


	}


}

void SP2::renderNPC()
{

	//NPC
	modelStack.PushMatrix();
	modelStack.Translate(NPC1.Pos.x, NPC1.Pos.y, NPC1.Pos.z + move);

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

void SP2::ballquest()
{
	if (currentQuest == ballQuest && complete == false)
	{
		RenderTextOnScreen(meshList[GEO_TEXT2], "Find the ball and give it to pucboi: IN PROGRESS", Color(0, 1, 0), 3, 0, 12);
	}
	if (pickup == false)
	{

		modelStack.PushMatrix();
		modelStack.Translate(ball.Pos.x, ball.Pos.y, ball.Pos.z );
		modelStack.Scale(0.1, 0.1, 0.1);
		RenderMesh(meshList[GEO_SPHERE], false);
		modelStack.PopMatrix();
	}

	if (ball.hitbox.PointToAABB(camera.position) && pickup == false)
	{
		RenderTextOnScreen(meshList[GEO_TEXT2], "Press 'T' to pick up the ball", Color(0, 1, 0), 3, 0, 5);
		if (Application::IsKeyPressed('T'))
		{
			pickup = true;

		}
	}
	if (pickup == true)
	{
		RenderOnScreen(meshList[GEO_SPHERE], 5, 10, 10);
	}
	if (pickup == true && NPC1.hitbox.PointToAABB(camera.position) && Application::IsKeyPressed('T'))
	{
		complete = true;
	}


	if (complete == true && count <= 150.f)

	{
		count += (float)(2);
		RenderTextOnScreen(meshList[GEO_TEXT2], "Find the ball and give it to pucboi: COMPLETE", Color(0, 1, 0), 3, 0, 10);
	}

}

void SP2::renderNPC2()
{
	//npc
	modelStack.PushMatrix();
	modelStack.Translate(NPC2.Pos.x, NPC2.Pos.y, NPC2.Pos.z + move);

	modelStack.PushMatrix();
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[GEO_NPC2], false);
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

void SP2::asteroidquest()
{
	if (currentQuest == asteroidQuest && complete2 == false)
	{
		RenderTextOnScreen(meshList[GEO_TEXT2], "DE_STROY asteroids: IN PROGRESS", Color(0, 1, 0), 3, 0, 12);
	}

	if (destroyed >= 5 && NPC2.hitbox.PointToAABB(camera.position) && Application::IsKeyPressed(VK_LBUTTON))
	{
		complete2 = true;
	}


	if (complete2 == true && count <= 150.f)

	{
		count += (float)(2);
		RenderTextOnScreen(meshList[GEO_TEXT2], "DE_STROY asteroids: COMPLETE", Color(0, 1, 0), 3, 0, 10);
	}

}

void SP2::renderNPC3()
{
	//npc
	modelStack.PushMatrix();
	modelStack.Translate(NPC3.Pos.x, NPC3.Pos.y, NPC3.Pos.z + move);

	modelStack.PushMatrix();
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[GEO_NPC3], false);
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

void SP2::buyshipquest()
{
	if (currentQuest == buyshipQuest && complete3 == false)
	{
		RenderTextOnScreen(meshList[GEO_TEXT2], "Buy 3 shipz: IN PROGRESS", Color(0, 1, 0), 3, 0, 12);
	}


	if (complete3 == true && count <= 150.f)

	{
		count += (float)(2);
		RenderTextOnScreen(meshList[GEO_TEXT2], "Buy 3 shipz: COMPLETE", Color(0, 1, 0), 3, 0, 10);
	}
}

void SP2::renderNPC4()
{
	//npc
	modelStack.PushMatrix();
	modelStack.Translate(NPC4.Pos.x + move, NPC4.Pos.y, NPC4.Pos.z);
	modelStack.Rotate(90, 0, 1, 0);

	modelStack.PushMatrix();
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[GEO_NPC4], false);
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

void SP2::abductionquest()
{
	if (currentQuest == abductionQuest && complete4 == false)
	{
		RenderTextOnScreen(meshList[GEO_TEXT2], "Help fund kidnap: IN PROGRESS", Color(0, 1, 0), 3, 0, 12);
	}
}

void SP2::renderNPC5()
{
	//npc
	modelStack.PushMatrix();
	modelStack.Translate(NPC5.Pos.x + move, NPC5.Pos.y, NPC5.Pos.z);
	modelStack.Rotate(90, 0, 1, 0);

	modelStack.PushMatrix();
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[GEO_NPC4], false);
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

void SP2::RenderOnScreen(Mesh* mesh, float size, float x, float y)
{
	/*if (!mesh || mesh->textureID <= 0)
	return;*/
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

	glEnable(GL_DEPTH_TEST);
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
	int k = (rand() % (to - from)) + from;
    return k; 
}