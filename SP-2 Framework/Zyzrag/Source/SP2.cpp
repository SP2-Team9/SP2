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
}

void SP2::Init()
{
	playerShop = new Shop(10, 10, 100, 0, 0, 0);
	
	enableLight = true;
	readyToUse = 2.f;
	LightView = Vector3(0, 1, 0);
	state = MainMenu;
	selection = nullptr;
	widescreen = false;
    currMoney = 10000;
	place = nullptr;
	placeType = 0;
	lastState = 0;

	shopInit();
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

    meshList[GEO_ASTEROID_HEALTH] = MeshBuilder::GenerateQuad("Asteroid Health Bar", Color(1, 0, 0), 3, 1);

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

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("Text", 16, 16);
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

	meshList[GEO_MIDSHIP] = MeshBuilder::GenerateOBJ("Midium Ship", "OBJ//midship.obj");
	meshList[GEO_MIDSHIP]->textureID = LoadTGA("Image//midship.tga");

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

	meshList[GEO_TITLESCREEN] = MeshBuilder::GenerateQuad("menu", Color(1, 1, 1), screenWidth, screenHeight);
	meshList[GEO_TITLESCREEN]->textureID = LoadTGA("Image//menu.tga");

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

	//Shop Related
	meshList[GEO_SHOPBACKDROP] = MeshBuilder::GenerateQuad("Shop Background", Color(1, 1, 1), screenWidth, screenHeight);

	if (widescreen == false)
		meshList[GEO_SHOPBACKDROP]->textureID = LoadTGA("Image//shopbackground.tga");
	else
		meshList[GEO_SHOPBACKDROP]->textureID = LoadTGA("Image//shopbackgroundwide.tga");

	meshList[GEO_ATTACKUP] = MeshBuilder::GenerateQuad("Attack Up Logo", Color(1, 1, 1), screenWidth, screenWidth);
	meshList[GEO_ATTACKUP]->textureID = LoadTGA("Image//attackup.tga");
	meshList[GEO_FIRERATEUP] = MeshBuilder::GenerateQuad("Fire Rate Up", Color(1, 1, 1), screenWidth, screenWidth);
	meshList[GEO_FIRERATEUP]->textureID = LoadTGA("Image//firerateup.tga");
	meshList[GEO_HEALTHUP] = MeshBuilder::GenerateQuad("Health Up", Color(1, 1, 1), screenWidth, screenWidth);
	meshList[GEO_HEALTHUP]->textureID = LoadTGA("Image//healthup.tga");
}

void SP2::Update(double dt)
{
	camera.Update(dt);
	FPSText = std::to_string(toupper(1 / dt));

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

	case inShop:

		inShopUpdates(dt);

		break;

	}

	if (state != MainMenu && state != exit){

		generalUpdates(dt);

	}


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
		renderBullets();
		renderExplosion();
        renderShips();
        renderWayPoints();
		renderFightingUI();
		quests();
		ballquest();
		asteroidquest();
		buyshipquest();
		abductionquest();
		break;

	case inSpaceStation:
		renderBullets();
		renderExplosion();
		renderNPC();
		renderNPC2();
		renderNPC3();
		renderNPC4();
		renderShips();
		renderFightingUI();
		quests();
		ballquest();
		asteroidquest();
		buyshipquest();
		abductionquest();
		break;

	case inPlayerShip:
		renderBullets();
		renderExplosion();
		renderWayPoints();
		renderShips();
		renderFightingUI();
        renderDistances();
		quests();
		ballquest();
		asteroidquest();
		buyshipquest();
		abductionquest();
		break;

	case inShop:
		renderShopMenu();
		renderFightingUI();
		break;

	}

	renderAllHitbox();
}

void SP2::Exit()
{
	std::cout << "Exiting" << std::endl;
	std::cout << "Clearing Ships" << std::endl;
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

	std::cout << "Clearing Explosions" << std::endl;
	for (vector<Explosion*>::iterator it = allExplosions.begin(); it != allExplosions.end(); ++it)
	{
		Explosion* temp = *it;
		delete temp;
		it = allExplosions.erase(it);
	}
	

	std::cout << "Clearing Asteroids" << std::endl;
	for (vector<Asteroid*>::iterator it = Vasteroid.begin(); it != Vasteroid.end();)
	{
		Asteroid* temp = *it;
		delete temp;
		it = Vasteroid.erase(it);
	}

	std::cout << "Clearing Bullets" << std::endl;
	for (vector<Bullet*>::iterator it = allBullets.begin(); it != allBullets.end(); it++){

		Bullet* temp = *it;
		delete temp;
		it = allBullets.erase(it);
	}

	delete playerShop;

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
	LastLocation.SetView(0, 0, -1);
	LastLocation.SetUp(0, 1, 0);
	LastLocation.SetRight(1, 0, 0);

	NPC1.SetPos(3, 0, 1);
	NPC1.SetHitboxSize(2);

	NPC2.SetPos(5, 0, 7);
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
	playerShip.SetPos(0, 10, 0);
	playerShip.SetView(0, 0, 1);
	playerShip.SetRight(-1, 0, 0);
	playerShip.SetHitboxSize(-5, -3, -5, 4, 5, 5);
	playerShip.updateHitbox();

	//Vehicles Init
	allVehicles.insert(std::pair<int, vector<Vehicles*>>(GEO_SMALLSHIP, smallVehicles));
	allVehicles.insert(std::pair<int, vector<Vehicles*>>(GEO_MIDSHIP, midVehicles));
	allVehicles.insert(std::pair<int, vector<Vehicles*>>(GEO_LARGESHIP, largeVehicles));

	storedVehicles.insert(std::pair<int, stack<Vehicles*>>(GEO_SMALLSHIP, stackSmallVehicles));
	storedVehicles.insert(std::pair<int, stack<Vehicles*>>(GEO_MIDSHIP, stackMidVehicles));
	storedVehicles.insert(std::pair<int, stack<Vehicles*>>(GEO_LARGESHIP, stackLargeVehicles));
}

void SP2::shopInit()
{
	playerShop = new Shop(100, 10, 100, 0, 0, 0);

	objSize = static_cast<float>(Application::screenWidth) / 1920.f;
	shopState = Main;
	screenWidth = (Application::screenWidth / 10);
	screenHeight = (Application::screenHeight / 10);

	shopSmallRot = shopMidRot = shopLargeRot = 0;

	shopSmallPos.Set(0.2f * screenWidth, (0.7 * screenHeight) - 2.f, 50);
	shopSmallScale = objSize;

	shopMidPos.Set(0.5f * screenWidth, 0.7 * screenHeight, 50);
	shopMidScale = objSize;

	shopLargePos.Set(0.8f * screenWidth, 0.7 * screenHeight, 50);
	shopLargeScale = objSize;
}

void SP2::WorldHitboxInit()
{
	worldHitbox.push_back(AABB(Vector3(-10, -1, -10), Vector3(10, 0, 10)));
	worldHitbox.push_back(AABB(Vector3(-10, 3, -10), Vector3(10, 5, 10)));
	worldHitbox.push_back(AABB(Vector3(-10, 0, 10), Vector3(10, 5, 15)));
	worldHitbox.push_back(AABB(Vector3(-10, 0, -15), Vector3(10, 5, -10)));
	worldHitbox.push_back(AABB(Vector3(8, 0, -10), Vector3(15, 5, 10)));
	worldHitbox.push_back(AABB(Vector3(-15, 0, -10), Vector3(-8, 5, 10)));
	worldHitbox.push_back(AABB(Vector3(-5, -40, -5), Vector3(5, 0, 5)));
	worldHitbox.push_back(AABB(Vector3(-25, -19, -25), Vector3(25, -7, 25)));
	worldHitbox.push_back(AABB(-0.5, 0, 6.6f, 0.5, 1.2f, 7.3f));

	Interactions.push_back(AABB(-1, 0, -1, 1, 2, 1));
	Interactions.push_back(AABB(-0.3, 0, 5, 0.5, 1.2f, 7));
	Interactions.push_back(AABB(-10, 5, -10, 10, 20, 10));
	
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

    if (Application::IsKeyPressed(VK_LBUTTON)){

        if (playerShip.fireBullets(playerShop->playerShipFireRate)){

            Bullet* newBullet = new Bullet(playerShip.View, playerShip.Pos, playerShop->playerShipDamage);

            allBullets.push_back(newBullet);

        }

    }

}

void SP2::generateAsteroid()
{
	if (Vasteroid.size() < 40)
	{
		if (generate_range(0, 100) < 100)
		{
			Asteroid* asteroid = new Asteroid(generate_range(5, 30));

			switch (generate_range(0, 4))
			{
			case 0:

				asteroid->SetPos(generate_range(-2400 + camera.position.x, 2400 + camera.position.x), 0, generate_range(2400 + camera.position.z, 2500 + camera.position.z));
				asteroid->SetView(Vector3(generate_range(-2400, 2400), 0, -2500));

				break;

			case 1:

				asteroid->SetPos(generate_range(-2400 + camera.position.x, 2400 + camera.position.x), 0, generate_range(-2400 + camera.position.z, -2500 + camera.position.z));
				asteroid->SetView(Vector3(generate_range(-2400, 2400), 0, 2500));

				break;

			case 2:

				asteroid->SetPos(generate_range(2400 + camera.position.x, 2500 + camera.position.x), 0, generate_range(-2400 + camera.position.z, 2400 + camera.position.z));
				asteroid->SetView(Vector3(-2500, 0, generate_range(-2400, 2400)));

				break;

			case 3:

				asteroid->SetPos(generate_range(-2400 + camera.position.x, -2500 + camera.position.x), 0, generate_range(-2400 + camera.position.z, 2400 + camera.position.z));
				asteroid->SetView(Vector3(2500, 0, generate_range(-2400, 2400)));

				break;

			}


			Vasteroid.push_back(asteroid);
		}
	}
}


// Renders
void SP2::renderSkybox()
{
	
	float skyboxSize = 5005;

    modelStack.PushMatrix();

    modelStack.Translate(playerShip.Pos.x, playerShip.Pos.y, playerShip.Pos.z);

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

	if (place)
	{
		modelStack.PushMatrix();
		modelStack.Translate(place->Pos.x, place->Pos.y, place->Pos.z);
		modelStack.Rotate(place->initialYaw, 0, 1, 0);
		RenderMesh(meshList[placeType], enableLight);
		modelStack.PopMatrix();
	}
	int test = 0;
	for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
	{
		vector<Vehicles*>::iterator it = allVehicles[i].begin();
		while (it != allVehicles[i].end())
		{
			test += 1;
			Vehicles* Vtemp = *it;
			modelStack.PushMatrix();
			modelStack.Translate(Vtemp->Pos.x, Vtemp->Pos.y, Vtemp->Pos.z);
			modelStack.Rotate(Vtemp->Yaw, 0, 1, 0);
			RenderMesh(meshList[i], enableLight);
			modelStack.PopMatrix();

			it++;
		}
	}
	if (playerShip.isDead == false)
	{
		modelStack.PushMatrix();
		modelStack.Translate(playerShip.Pos.x, playerShip.Pos.y, playerShip.Pos.z);
		modelStack.Rotate(playerShip.pitch, playerShip.Right.x, 0, playerShip.Right.z);
		modelStack.Rotate(playerShip.yaw, 0, 1, 0);
		RenderMesh(meshList[GEO_XWING], enableLight);
		modelStack.PopMatrix();
	}
}

void SP2::renderShopMenu()
{
	RenderOnScreen(meshList[GEO_SHOPBACKDROP], screenWidth / 2, screenHeight / 2, -20, 1, 90, 0, 0);

	switch (shopState)
	{
	case Main:
		RenderTextOnScreen(meshList[GEO_TEXT], "Mercenaries", Color(0, 1, 0), objSize * 12, 0.32f * screenWidth, 0.9f * screenHeight, 9);

		RenderOnScreen(meshList[GEO_SMALLSHIP], shopSmallPos, shopSmallScale, 20, shopSmallRot, 0);
		RenderOnScreen(meshList[GEO_MIDSHIP], shopMidPos, shopMidScale, 20, shopMidRot, 0);
		RenderOnScreen(meshList[GEO_LARGESHIP], shopLargePos, shopLargeScale, 20, shopLargeRot, 0);

		RenderTextOnScreen(meshList[GEO_TEXT], "Player Ship Upgrades", Color(0, 1, 0), objSize * 8, 0.3f * screenWidth, 0.45f * screenHeight, 9);

		RenderTextOnScreen(meshList[GEO_TEXT], "Damage: " + std::to_string(playerShop->playerShipDamage), Color(0, 1, 0), objSize * 5, 0.13f * screenWidth, 0.15f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "$100", Color(0, 1, 0), objSize * 5, 0.17f * screenWidth, 0.10f * screenHeight, 9);

		RenderTextOnScreen(meshList[GEO_TEXT], "Fire Rate: " + std::to_string(playerShop->playerShipFireRate / 100) + "RPS", Color(0, 1, 0), objSize * 5, 0.4f * screenWidth, 0.15f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "$50", Color(0, 1, 0), objSize * 5, 0.48f * screenWidth, 0.10f * screenHeight, 9);

		RenderTextOnScreen(meshList[GEO_TEXT], "Repair Ship", Color(0, 1, 0), objSize * 5, 0.733f * screenWidth, 0.15f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "$200", Color(0, 1, 0), objSize * 5, 0.775f * screenWidth, 0.10f * screenHeight, 9);

		RenderOnScreen(meshList[GEO_ATTACKUP], 0.2f * screenWidth, 0.3f * screenHeight, 2, 0.1f * objSize, 90, 0, 0);
		RenderOnScreen(meshList[GEO_FIRERATEUP], 0.5f * screenWidth, 0.3f * screenHeight, 2, 0.1f *  objSize, 90, 0, 0);
		RenderOnScreen(meshList[GEO_HEALTHUP], 0.8f * screenWidth, 0.3f * screenHeight, 2, 0.1f *  objSize, 90, 0, 0);
		break;
	case FirstShip:
		RenderTextOnScreen(meshList[GEO_TEXT], "Mercenaries", Color(0, 1, 0), objSize * 12, 0.32f * screenWidth, 0.9f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "Buy", Color(0, 1, 0), objSize * 10, 0.46f * screenWidth, 0.2f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "Current Amount: " + std::to_string(storedVehicles[GEO_SMALLSHIP].size() + allVehicles[GEO_SMALLSHIP].size()), Color(0, 1, 0), objSize * 5, 0.39f * screenWidth, 0.15f * screenHeight, 9);
		RenderOnScreen(meshList[GEO_SMALLSHIP], shopSmallPos, shopSmallScale, 20, shopSmallRot, 0);
		break;
	case SecondShip:
		RenderTextOnScreen(meshList[GEO_TEXT], "Mercenaries", Color(0, 1, 0), objSize * 12, 0.32f * screenWidth, 0.9f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "Buy", Color(0, 1, 0), objSize * 10, 0.46f * screenWidth, 0.2f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "Current Amount: " + std::to_string(storedVehicles[GEO_MIDSHIP].size() + allVehicles[GEO_MIDSHIP].size()), Color(0, 1, 0), objSize * 5, 0.39f * screenWidth, 0.15f * screenHeight, 9);
		RenderOnScreen(meshList[GEO_MIDSHIP], shopMidPos, shopMidScale, 20, shopMidRot, 0);
		break;
	case ThirdShip:
		RenderTextOnScreen(meshList[GEO_TEXT], "Mercenaries", Color(0, 1, 0), objSize * 12, 0.32f * screenWidth, 0.9f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "Buy", Color(0, 1, 0), objSize * 10, 0.46f * screenWidth, 0.2f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "Current Amount: " + std::to_string(storedVehicles[GEO_LARGESHIP].size() + allVehicles[GEO_LARGESHIP].size()), Color(0, 1, 0), objSize * 5, 0.39f * screenWidth, 0.15f * screenHeight, 9);
		RenderOnScreen(meshList[GEO_LARGESHIP], shopLargePos, shopLargeScale, 20, shopLargeRot, 0);
		break;
	}
}

void SP2::renderTitleScreen(){
	float objSize = static_cast<float>(Application::screenWidth) / 1920.f;

	//start menu
	modelStack.PushMatrix();
	RenderOnScreen(meshList[GEO_TITLESCREEN], screenWidth / 2, screenHeight / 2, -1, 1, 90, 0, 0);
	modelStack.PopMatrix();

	RenderTextOnScreen(meshList[GEO_TEXT1], "SPACE CONTROL", Color(0, 1, 0), objSize * 20, 0.18f * screenWidth, 0.8f * screenHeight, 1);
	RenderTextOnScreen(meshList[GEO_TEXT], "Click to Start", Color(0, 1, 0), objSize * 5, 0.42f * screenWidth, 0.45f * screenHeight, 1);
}

void SP2::renderFightingUI(){

    if (playerShip.health > 0){
        RenderTextOnScreen(meshList[GEO_TEXT], "HP:" + std::to_string(playerShip.health), Color(0, 1, 0), objSize * 8, 0.02f * screenWidth, screenHeight * 0.9f, 50);
    }
    else{
        RenderTextOnScreen(meshList[GEO_TEXT], "Ship is Destroyed!!!", Color(0, 1, 0), objSize * 8, 0.02f * screenWidth, screenHeight * 0.9f, 50);
    }
	RenderTextOnScreen(meshList[GEO_TEXT], "Thrust: " + std::to_string((int)(playerShip.thrust)), Color(0, 1, 0), objSize * 8, 0.02f * screenWidth, screenHeight * 0.9f -4.f, 50);

    RenderTextOnScreen(meshList[GEO_TEXT], "Cash: $" + std::to_string(currMoney), Color(0, 1, 0), objSize * 8, 0.7f * screenWidth, screenHeight * 0.9f, 50);
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
	vector<AABB> allHitbox;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
	{
		vector<Vehicles*>::iterator it = allVehicles[i].begin();
		while (it != allVehicles[i].end())
		{
			Vehicles* Vtemp = *it;
			allHitbox.push_back(Vtemp->hitbox);
			it++;
		}
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

        if (asteroid->health < asteroid->maxHealth){

            renderHealthBar(asteroid->Pos, asteroid->size, asteroid->health);

        }

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

void SP2::renderHealthBar(Vector3 asteroidPosition, int asteroidSize, int health){

    float asteroidHealthYaw = 0;
    float asteroidHealthPitch = 0;
    Vector3 initView(0, 1, 0);
    Vector3 view = (camera.position - camera.target).Normalized();
    Vector3 XZview(view.x, 0, view.z);
    XZview.Normalize();

    asteroidHealthPitch = Math::RadianToDegree(acos(initView.Dot(view)));
    Vector3 V3 = initView.Cross(view);
    if (V3.Dot(Vector3(0, 0, 1)) < 0)
    {
        asteroidHealthPitch *= -1;
    }

    Mtx44 rotation;
    rotation.SetToRotation(asteroidHealthPitch, 0, 0, 1);
    initView = rotation * initView;
    initView.Set(initView.x, 0, initView.z);
    initView.Normalize();

    asteroidHealthYaw = Math::RadianToDegree(acos(initView.Dot(XZview)));
    V3 = initView.Cross(XZview);
    if (V3.Dot(Vector3(0, 1, 0)) < 0)
    {
        asteroidHealthYaw *= -1;
    }


    if (health > 0){

        modelStack.PushMatrix();

        modelStack.Translate(asteroidPosition.x, asteroidPosition.y + asteroidSize * 1.5, asteroidPosition.z);
        modelStack.Rotate(asteroidHealthYaw, 0, 1, 0);
        modelStack.Rotate(asteroidHealthPitch, 0, 0, 1);
        modelStack.Scale(1, 1, health / 2);
        RenderMesh(meshList[GEO_ASTEROID_HEALTH], false);

        modelStack.PopMatrix();


    }

}

void SP2::renderDistances(){
    //NPC

    float Yaw = 0;
    float Pitch = 0;
    Vector3 initView(0, 0, 1);
    Vector3 view = (camera.position - station.Pos).Normalized();
    Vector3 XZview(view.x, 0, view.z);
    XZview.Normalize();

    Vector3 normal = initView.Cross(view);
    Yaw = Math::RadianToDegree(acos(initView.Dot(XZview)));

    if (normal.y < 0){

        Yaw *= -1;

    }

    float scaleSize = 10 + playerShip.Pos.distanceBetween2points(station.Pos) / 70;

    int currDistance = playerShip.Pos.distanceBetween2points(station.Pos);
    int numberOfDigits = 0;
    string distance = std::to_string(currDistance);

    for (float i = currDistance; i / 10 > 1;){

        numberOfDigits += 1;
        i /= 10;

    }
    //Pushing the space station Word
    modelStack.PushMatrix(); 

    
    modelStack.Rotate(Yaw, 0, 1, 0);
    modelStack.Scale(scaleSize, scaleSize, scaleSize);

    modelStack.PushMatrix();


    modelStack.Translate(-5, 3, 0);
    RenderText(meshList[GEO_TEXT], "Space Station", Color(0, 1, 0));

    modelStack.PopMatrix();
   

    modelStack.PopMatrix();

    //Pushing the Distance
    modelStack.PushMatrix();

    modelStack.Rotate(Yaw, 0, 1, 0);
    modelStack.Scale(scaleSize, scaleSize, scaleSize);

    modelStack.PushMatrix();

    
    modelStack.Translate(1 - numberOfDigits, 2, 0);
    RenderText(meshList[GEO_TEXT], distance + "m", Color(0, 1, 0));

    modelStack.PopMatrix();


    modelStack.PopMatrix();

    //Pushing the Arrow
    modelStack.PushMatrix();

    modelStack.Rotate(Yaw, 0, 1, 0);
    modelStack.Scale(scaleSize, scaleSize, scaleSize);

    modelStack.PushMatrix();

    modelStack.Translate(0.02f, 1, 0);
    RenderText(meshList[GEO_TEXT], "V ", Color(0, 1, 0));

    modelStack.PopMatrix();
   

    modelStack.PopMatrix();
 
}


//Updates

void SP2::NPCUpdates(double dt){

	//npc move
	if (talking == false)
	{

		if (move < 2 && re == false)
		{
			move += (float)(2 * dt);


			if (move >= 2)
			{
				re = true;

			}

		}
		if (move >= -2 && re == true)
		{
			move -= (float)(2 * dt);
			if (move <= -2)
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
	camera.YawRotation(playerShip, dt);
	picker.set(camera, projectionStack.Top());
	picker.update();
	MouseSelection(dt);
	shipBulletCreation(dt);

	if (place != nullptr && hold == false)
	{
		place->SetPos(picker.WorldCoord());
	}

	if (Application::IsKeyPressed('E') && delay >= 1.f)
	{
		Application::centerMouse();
		selection = nullptr;
		place = nullptr;
		delay = 0;
		if (lastState == inSpaceStation)
		{
			state = inSpaceStation;
			camera.Init(LastLocation.Pos, LastLocation.Pos + LastLocation.View);
		}
		else
		{
			state = inPlayerShip;
			camera.PointAt(playerShip, 10, -30);
		}
		

	}
	else if (Application::IsKeyPressed('1') && place == nullptr && delay >= 1.f)
	{
		if (!storedVehicles[GEO_SMALLSHIP].empty())
		{
			place = storedVehicles[GEO_SMALLSHIP].top();
			placeType = GEO_SMALLSHIP;
		}	
	}
	else if (Application::IsKeyPressed('2') && place == nullptr && delay >= 1.f)
	{
		if (!storedVehicles[GEO_MIDSHIP].empty())
		{
			place = storedVehicles[GEO_MIDSHIP].top();
			placeType = GEO_MIDSHIP;
		}
	}
	else if (Application::IsKeyPressed('3') && place == nullptr && delay >= 1.f)
	{
		if (!storedVehicles[GEO_LARGESHIP].empty())
		{
			place = storedVehicles[GEO_LARGESHIP].top();
			placeType = GEO_LARGESHIP;
		}
	}

}

void SP2::shopUpdates(double dt)
{
	double mouseX, mouseY;
	Application::getMouse(mouseX, mouseY);
	float screenWidth = Application::screenWidth / 10;
	float screenHeight = Application::screenHeight / 10;
	mouseX /= 10;
	mouseY /= 10;
	mouseY = screenHeight - mouseY;

	switch (shopState)
	{
	case Main:
		if (shopSmallPos.x > screenWidth * 0.2f)
		{
			shopSmallRot = 0;
			if (shopSmallScale > objSize)
				shopSmallScale -= dt * 2.f;
			shopSmallPos += shopTarget;
		}
		else if (shopMidPos.y < (screenHeight * 0.7f) - 2.f)
		{
			shopMidRot = 0;
			if (shopMidScale > objSize)
				shopMidScale -= dt * 8.f;
			shopMidPos += shopTarget;
		}
		else if (shopLargePos.x < screenWidth * 0.8f)
		{
			shopLargeRot = 0;
			if (shopLargeScale > objSize)
				shopLargeScale -= dt * 1.5f;
			shopLargePos += shopTarget;
		}
		else if (Application::IsKeyPressed(VK_LBUTTON) && delay > 0.5f)
		{
			delay = 0;
			if (mouseX > 0.1f * screenWidth && mouseX < 0.3f * screenWidth && mouseY > 0.65f * screenHeight && mouseY < 0.8f * screenHeight)
			{
				shopTarget.Set((screenWidth / 2) - shopSmallPos.x, (screenHeight / 2) - shopSmallPos.y, 0);
				shopTarget.Normalize();
				shopState = FirstShip;
			}
			else if (mouseX > 0.4f * screenWidth && mouseX < 0.5f * screenWidth && mouseY > 0.65f * screenHeight && mouseY < 0.8f * screenHeight)
			{
				shopTarget.Set((screenWidth / 2) - shopMidPos.x, (screenHeight / 2) - shopMidPos.y, 0);
				shopTarget.Normalize();
				shopState = SecondShip;
			}
			else if (mouseX > 0.7f * screenWidth && mouseX < 0.9f * screenWidth && mouseY > 0.65f * screenHeight && mouseY < 0.8f * screenHeight)
			{
				shopTarget.Set((screenWidth / 2) - shopLargePos.x, (screenHeight / 2) - shopLargePos.y, 0);
				shopTarget.Normalize();
				shopState = ThirdShip;
			}
			else if (mouseX > 0.1f * screenWidth && mouseX < 0.3f * screenWidth && mouseY > 0.2f * screenHeight && mouseY < 0.4f * screenHeight)
			{
                if (currMoney - 100 >= 0)
				{
                    currMoney -= 100;
					playerShop->playerShipDamage += 10;
				}
			}
			else if (mouseX > 0.4f * screenWidth && mouseX < 0.6f * screenWidth && mouseY > 0.2f * screenHeight && mouseY < 0.4f * screenHeight)
			{
                if (currMoney - 50 >= 0)
				{
                    currMoney -= 50;
					playerShop->playerShipFireRate += 20;
				}
			}
			else if (mouseX > 0.7f * screenWidth && mouseX < 0.9f * screenWidth && mouseY > 0.2f * screenHeight && mouseY < 0.4f * screenHeight)
			{
                if (currMoney - 200 >= 0)
				{
                    currMoney -= 200;
                    playerShip.maxHealth += 10;
                    playerShip.health = playerShip.maxHealth;

				}
			}
		}
		else if (Application::IsKeyPressed(VK_RBUTTON) || Application::IsKeyPressed('E') && delay > 0.5f)
		{
			delay = 0;
			Application::centerMouse();
			state = inSpaceStation;
		}
		break;

	case FirstShip:
		shopSmallRot += dt * 50;
		if (shopSmallScale < objSize * 3)
			shopSmallScale += dt * 3.5f;
		if (shopSmallPos.x < screenWidth / 2)
			shopSmallPos += shopTarget * 2;
		if (Application::IsKeyPressed(VK_LBUTTON) && delay > 0.5f && mouseX > 0.4f * screenWidth && mouseX < 0.6f * screenWidth && mouseY > 0.2f * screenHeight && mouseY < 0.4f * screenHeight)
		{
			delay = 0;
			if (currMoney - 200 >= 0)
			{
				currMoney -= 200;
				smallShip = new Vehicles(Vector3(0, 0, 0), Vector3(1, 0, 0), 50, 20, 500, 10);
				smallShip->SetHitboxSize(-10, -10, -10, 10, 10, 10);
				smallShip->SetInteractionSize(10, 10, 10, 10, 10, 10);
				storedVehicles[GEO_SMALLSHIP].push(smallShip);
			}
		}
		else if (Application::IsKeyPressed(VK_LBUTTON) && delay > 0.5f)
		{
			delay = 0;
			shopTarget.Set(0.2f * screenWidth - shopSmallPos.x, 0.7f * screenHeight - shopSmallPos.y, 0);
			shopTarget.Normalize();
			shopState = Main;
		}
		break;
	case SecondShip:
		shopMidRot += dt * 50;
		if (shopMidScale < objSize * 3)
			shopMidScale += dt * 5;
		if (shopMidPos.y > screenHeight / 2)
		{
			shopMidPos += shopTarget;
		}

		if (Application::IsKeyPressed(VK_LBUTTON) && delay > 0.5f && mouseX > 0.4f * screenWidth && mouseX < 0.6f * screenWidth && mouseY > 0.2f * screenHeight && mouseY < 0.4f * screenHeight)
		{
			delay = 0;
			if (currMoney - 400 >= 0)
			{
				currMoney -= 400;
				midShip = new Vehicles(Vector3(0, 0, 0), Vector3(1, 0, 0), 25, 50, 200, 30);
				midShip->SetHitboxSize(-15, -10, -15, 15, 10, 15);
				midShip->SetInteractionSize(15, 10, 15, 15, 10, 15);
				storedVehicles[GEO_MIDSHIP].push(midShip);
			}
		}
		else if (Application::IsKeyPressed(VK_LBUTTON) && delay > 0.5f)
		{
			delay = 0;
			shopTarget.Set(0.5f * screenWidth - shopMidPos.x, 0.7f * screenHeight - shopMidPos.y, 0);
			shopTarget.Normalize();
			shopState = Main;
		}
		break;
	case ThirdShip:
		shopLargeRot += dt * 50;
		if (shopLargeScale < objSize * 2)
			shopLargeScale += dt * 2;
		if (shopLargePos.x > screenWidth / 2)
		{
			shopLargePos += shopTarget * 2;
		}

		if (Application::IsKeyPressed(VK_LBUTTON) && delay > 0.5f && mouseX > 0.4f * screenWidth && mouseX < 0.6f * screenWidth && mouseY > 0.2f * screenHeight && mouseY < 0.4f * screenHeight)
		{
			delay = 0;
			if (currMoney - 600 >= 0)
			{
				currMoney -= 600;
				largeShip = new Vehicles(Vector3(0, 0, 0), Vector3(1, 0, 0), 10, 100, 50, 100);
				largeShip->SetHitboxSize(-20, -10, -20, 20, 10, 20);
				largeShip->SetInteractionSize(20, 10, 20, 20, 10, 20);
				storedVehicles[GEO_LARGESHIP].push(largeShip);
			}
		}
		else if (Application::IsKeyPressed(VK_LBUTTON) && delay > 0.5f)
		{
			delay = 0;
			shopTarget.Set(0.8f * screenWidth - shopLargePos.x, 0.7f * screenHeight - shopLargePos.y, 0);
			shopTarget.Normalize();
			shopState = Main;
		}
		break;
	}
}

void SP2::inShopUpdates(double dt)
{
	camera.EnableCursor();
	shopUpdates(dt);
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

            allExplosions.push_back(new Explosion(15, 100, temp->Pos));
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

    for (vector<Asteroid*>::iterator Ait = Vasteroid.begin(); Ait != Vasteroid.end();)
    {
        Asteroid* tempAst = *Ait;

        if (tempAst->boom == true){

			allExplosions.push_back(new Explosion(tempAst->size * 2, 50, tempAst->Pos));
            Ait = Vasteroid.erase(Ait);
            delete tempAst;

        }
        else{

            Ait++;

        }

    }
}

void SP2::generalUpdates(double dt){

	playerShip.update(dt, worldHitbox);

	asteroidUpdate(dt);
    asteroidHitboxCheck();
	bulletUpdates(dt);
    asteroidHitboxCheck();
    bulletUpdates(dt);
    asteroidHitboxCheck();

	explosionUpdate(dt);
	vehicleUpdates(dt);
	checkHitboxes();
	
	delay += dt;

	if (playerShip.respawn(10) == true)
	{
		playerShip.reset();
		playerShip.SetPos(0, 15, 0);
		playerShip.SetView(0, 0, 1);
		playerShip.SetRight(-1, 0, 0);
		playerShip.SetHitboxSize(5);
	}

	if (Application::IsKeyPressed(VK_ESCAPE))
	{
		state = exit;
	}
}

void SP2::mainMenuUpdates(double dt){

	camera.EnableCursor();
	if (Application::IsKeyPressed(VK_LBUTTON))
	{
		state = inSpaceStation;
		camera.Init(LastLocation.Pos, LastLocation.Pos + LastLocation.View);
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

			delete temp;
            vitE = allExplosions.erase(vitE);


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
		if (playerShip.hitbox.AABBtoAABB(Interactions[2], playerShip.View))
		{
			playerShip.yaw = 0;
			playerShip.pitch = 0;
			playerShip.thrust = 0;
			playerShip.SetPos(0, 10, 0);
			playerShip.SetView(0, 0, 1);
			playerShip.SetRight(-1, 0, 0);
			playerShip.updateHitbox();
			delay = 0;
			camera.Init(LastLocation.Pos, LastLocation.Pos + LastLocation.View);
			state = inSpaceStation;
			
		}
	}
	else if (Application::IsKeyPressed('F') && delay >= 1.f)
	{
		delay = 0;
		state = RTS;
		lastState = inPlayerShip;
		camera.PointAt(playerShip, 100, -200);
	}

	if (playerShip.isDead == true)
	{
		delay = 0;
		state = inSpaceStation;
		camera.Init(LastLocation.Pos, LastLocation.Pos + LastLocation.View);
	}

	


}

void SP2::inSpaceStationUpdates(double dt){

	camera.DisableCursor();
	camera.FPSMovement(dt, worldHitbox);
	if (Application::IsKeyPressed('E') && delay >= 1.f)
	{
		if (Interactions[0].PointToAABB(camera.position) && playerShip.isDead == false)
		{
			delay = 0;
			state = inPlayerShip;
			LastLocation.SetPos(camera.position.x, camera.position.y, camera.position.z);
			LastLocation.SetView(camera.view.x, camera.view.y, camera.view.z);
			camera.PointAt(playerShip, 10, -30);
		}
		else if (Interactions[1].PointToAABB(camera.position))
		{
			delay = 0;
			state = inShop;
		}
	}

	if (Application::IsKeyPressed('F') && delay >= 1.f)
	{
		delay = 0;
		state = RTS;
		lastState = inSpaceStation;
		LastLocation.SetPos(camera.position.x, camera.position.y, camera.position.z);
		LastLocation.SetView(camera.view.x, camera.view.y, camera.view.z);
		camera.PointAt(playerShip, 100, -200);
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
						allExplosions.push_back(new Explosion(30, 50, ExploCenter));
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

	//Player playerShip to Ship
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
				storedVehicles[i].push(Vtemp);
				if (selection == Vtemp)
				{
					selection = nullptr;
				}
				it = allVehicles[i].erase(it);
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

			allExplosions.push_back(new Explosion(tempAst->size * 2, 50, tempAst->Pos));
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

                allExplosions.push_back(new Explosion(15, 100, tempBull->Pos));

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

            currMoney += tempAst->size * 10;

			allExplosions.push_back(new Explosion(tempAst->size * 2, 50, tempAst->Pos));
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

                    Vector3 ExploCenter = tempVeh->Pos + tempAst->Pos;
                    ExploCenter /= 2;
					allExplosions.push_back(new Explosion(tempAst->size * 2, 50, ExploCenter));

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
            playerShip.health -= tempAst->size * 2;
            Vector3 ExploCenter = playerShip.Pos + tempAst->Pos;
            ExploCenter /= 2;
			allExplosions.push_back(new Explosion(tempAst->size * 2, 50, ExploCenter));
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
        int ast1Health = temp1Ast->health;

        for (vector<Asteroid*>::iterator A2it = Vasteroid.begin(); A2it != Vasteroid.end();)
        {
            Asteroid* temp2Ast = *A2it;

            if (temp1Ast->hitbox.AABBtoAABB(temp2Ast->hitbox) && temp1Ast != temp2Ast){

                temp1Ast->health -= temp2Ast->health;
                temp2Ast->health -= ast1Health;

                if (temp2Ast->health > 0){
                    if (temp2Ast->health / 10 < 5){

                        temp2Ast->size = 5;

                    }
                    else{

                        temp2Ast->size = temp2Ast->health / 10;

                    }
                }
                else if (temp1Ast->health > 0){
                    if (temp1Ast->health / 10 < 5){

                        temp1Ast->size = 5;

                    }
                    else{

                        temp1Ast->size = temp1Ast->health / 10;

                    }
                }

            }


            if (temp2Ast->health <= 0){

				for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
				{
					vector<Vehicles*>::iterator it = allVehicles[i].begin();

					while (it != allVehicles[i].end())
					{

						Vehicles* temp = *it;

						if (temp->currAttackTarget == temp2Ast){

							temp->currAttackTarget = nullptr;

						}


						it++;
					}
				}

                Vector3 ExploCenter = temp2Ast->Pos + temp1Ast->Pos;
                ExploCenter /= 2;
				allExplosions.push_back(new Explosion(temp2Ast->size * 2, 50, ExploCenter));

                delete temp2Ast;
                A2it = Vasteroid.erase(A2it);



            }
            else{


                A2it++;

            }

        }

        if (temp1Ast->health <= 0){

			for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
			{
				vector<Vehicles*>::iterator it = allVehicles[i].begin();

				while (it != allVehicles[i].end())
				{

					Vehicles* temp = *it;

					if (temp->currAttackTarget == temp1Ast){

						temp->currAttackTarget = nullptr;

					}


					it++;
				}
			}

			allExplosions.push_back(new Explosion(temp1Ast->size * 2, 50, temp1Ast->Pos));
            delete temp1Ast;
            A1it = Vasteroid.erase(A1it);

        }
        else{

            A1it++;

        }
    
    
    }


}

void SP2::MouseSelection(double dt)
{
	if (place == nullptr)
	{
		if (Application::IsKeyPressed(VK_LBUTTON) && wayPointSetCoolDown > 0.5f)
		{
			std::cout << picker.WorldCoord() << std::endl;
			std::cout << playerShip.Pos << std::endl;
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

		
	}
	else
	{
		if (Application::IsKeyPressed(VK_LBUTTON) && wayPointSetCoolDown > 0.5f)
		{
			if (hold == false)
			{
				place->newVehicle.setCurrentLocation(place->Pos);
				hold = true;
			}
			else if (hold == true && picker.WorldCoord() != place->Pos)
			{
				place->View.Set(picker.WorldCoord().x - place->Pos.x, picker.WorldCoord().y - place->Pos.y, picker.WorldCoord().z - place->Pos.z);
				place->View.Normalize();
				place->initialYaw = place->getRotationAngle(place->View);
			}
		}
		else if (hold == true)
		{
			place->currAttackTarget = nullptr;
			place->newVehicle.resetWayPoints();
			place->initialMoveDirection();
			allVehicles[placeType].push_back(place);
			place = nullptr;
			storedVehicles[placeType].pop();
			hold = false;
			wayPointSetCoolDown = 0;
		}
		else if (Application::IsKeyPressed(VK_RBUTTON))
		{
			place = nullptr;
			wayPointSetCoolDown = 0;
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
		RenderOnScreen(meshList[GEO_SPHERE], 10, 10, 0, 5, 0, 0, 0);
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

void SP2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, float z)
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

void SP2::RenderOnScreen(Mesh* mesh, float x, float y, float z, float size, float rotX, float rotY, float rotZ)
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

void SP2::RenderOnScreen(Mesh* mesh, Vector3 pos, float size, float rotX, float rotY, float rotZ)
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