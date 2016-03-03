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
	enableLight = true;
	displayHelp = false;
	readyToUse = 2.f;
	LightView = Vector3(0, 1, 0);
	state = MainMenu;
	widescreen = false;
	kidnap = false;
	pickup = false;
    currMoney = 1000;
	place = nullptr;
	placeType = 0;
	destroyed = 0;
	shipbought = 0;

	shootingsfx->setDefault3DSoundMinDistance(200.f);
	shootingsfx->setDefault3DSoundMaxDistance(1000.f);
	shootingsfx->setSoundVolume(0.5f);

	explosionsfx->setDefault3DSoundMinDistance(1000.f);
	explosionsfx->setDefault3DSoundMaxDistance(5000.f);
	explosionsfx->setSoundVolume(1.f);

	shopInit();
	WorldHitboxInit();
	objectsInit();
	npcInit();

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Camera Init
	camera.Init(Vector3(0, 100, 0), Vector3(0, 100, 1));

	// Matrix Stack Init
	Mtx44 projection;
	if (static_cast<float>(Application::screenWidth) / static_cast<float>(Application::screenHeight) > 1.3334)
	{
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
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");


	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	glUseProgram(m_programID);

	lightInit();

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

	meshList[GEO_NPC5] = MeshBuilder::GenerateOBJ("NPChead", "OBJ//headnbody.obj");
	meshList[GEO_NPC5]->textureID = LoadTGA("Image//headnbody_uv4.tga");


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
	meshList[GEO_SPHERE]->textureID = LoadTGA("Image//ballquest.tga");

	meshList[GEO_INNERSTATION] = MeshBuilder::GenerateOBJ("Test", "OBJ//innerstation.obj");
	meshList[GEO_INNERSTATION]->textureID = LoadTGA("Image//innerstation.tga");

    meshList[GEO_EXPLOSION] = MeshBuilder::GenerateQuad("Explosion", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_EXPLOSION]->textureID = LoadTGA("Image//explosion.tga");

	meshList[GEO_HELPSCREEN] = MeshBuilder::GenerateQuad("menu", Color(1, 1, 1), screenWidth, screenHeight);

	//Shop Related
	meshList[GEO_SHOPBACKDROP] = MeshBuilder::GenerateQuad("Shop Background", Color(1, 1, 1), screenWidth, screenHeight);


	if (widescreen == false)
	{
		meshList[GEO_HELPSCREEN]->textureID = LoadTGA("Image//help.tga");
		meshList[GEO_SHOPBACKDROP]->textureID = LoadTGA("Image//shopbackground.tga");
	}
	else
	{
		meshList[GEO_HELPSCREEN]->textureID = LoadTGA("Image//helpwide.tga");
		meshList[GEO_SHOPBACKDROP]->textureID = LoadTGA("Image//shopbackgroundwide.tga");
	}

	meshList[GEO_ATTACKUP] = MeshBuilder::GenerateQuad("Attack Up Logo", Color(1, 1, 1), screenWidth, screenWidth);
	meshList[GEO_ATTACKUP]->textureID = LoadTGA("Image//attackup.tga");
	meshList[GEO_FIRERATEUP] = MeshBuilder::GenerateQuad("Fire Rate Up", Color(1, 1, 1), screenWidth, screenWidth);
	meshList[GEO_FIRERATEUP]->textureID = LoadTGA("Image//firerateup.tga");
	meshList[GEO_HEALTHUP] = MeshBuilder::GenerateQuad("Health Up", Color(1, 1, 1), screenWidth, screenWidth);
	meshList[GEO_HEALTHUP]->textureID = LoadTGA("Image//healthup.tga");
}

void SP2::Update(double dt){

	if (waveFunctions->stationHealth <= 0){

		cout << "Error" << endl;
		state = gameOver;

	}

	if (Application::IsKeyPressed('P'))
	{
		Exit();
		Init();
	}

	if (state != MainMenu && state != exit && state != help && state != waveTransition && state != gameOver){

		generalUpdates(dt);

	}

	

	if (Application::IsKeyPressed(VK_ESCAPE) && state != inShop && sharedData::GetInstance()->Delay(0.5f))
	{ 
		lastState = state;
		state = exit;
	}

	if (waveFunctions->maxWaveCooldownTime - waveFunctions->currWaveCooldownTime >= 0 && waveFunctions->maxWaveCooldownTime - waveFunctions->currWaveCooldownTime <= 10)
	{
		Mtx44 rotation;
		rotation.SetToRotation(5, 0, 1, 0);
		light[2].spotDirection = rotation * light[2].spotDirection;
		light[2].power = 1.f;
		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);

		light[1].power = 0.f;
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	}
	else
	{
		light[2].spotDirection.Set(0, 0, 1);
		light[2].power = 0.f;
		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);

		light[1].power = 0.5f;
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	}

	camera.Update(dt);
	FPSText = std::to_string(toupper(1 / dt));
	sharedData::GetInstance()->DelayIncrement(dt, 5);

	switch (state){
	case MainMenu:

		mainMenuUpdates(dt);

		break;

	case RTS:

		RTSUpdates(dt);

		break;

	case inSpaceStation:

		inSpaceStationUpdates(dt);
		quests();

		break;

	case inPlayerShip:

		inPlayerShipUpdates(dt);
		quests();

		break;

	case inShop:

		inShopUpdates(dt);

		break;

	case help:

		if (Application::IsKeyPressed('H') && sharedData::GetInstance()->Delay(0.5f))
		{
			state = lastState;
		}

		break;
	
	case waveTransition:

		waveTransitionUpdate(dt);

		break;

	case gameOver:

		gameOverUpdate();
		break;

	case exit:
		camera.EnableCursor();
		if (Application::IsKeyPressed(VK_LBUTTON) && sharedData::GetInstance()->Delay(0.5f))
		{
			double mouseX, mouseY;
			Application::getMouse(mouseX, mouseY);
			float screenWidth = Application::screenWidth / 10;
			float screenHeight = Application::screenHeight / 10;
			mouseX /= 10;
			mouseY /= 10;
			mouseY = screenHeight - mouseY;

			if (mouseX > 0.2f * screenWidth && mouseX < 0.4f * screenWidth && mouseY > 0.3f * screenHeight && mouseY < 0.5f * screenHeight)
			{
				sharedData::GetInstance()->quit = true;
			}
			else if (mouseX > 0.5f * screenWidth && mouseX < 0.7f * screenWidth && mouseY > 0.3f * screenHeight && mouseY < 0.5f * screenHeight)
			{
				state = lastState;
			}
		}

		break;

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

	Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
	glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
	glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);

	lightPosition_cameraspace = viewStack.Top() * light[2].position;
	glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
	spotDirection_cameraspace = viewStack.Top() * light[2].spotDirection;
	glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	
	if (enableAxis == true)
		RenderMesh(meshList[GEO_AXES], false);

	switch (state)
	{
	case MainMenu:

		renderTitleScreen();
		break;

    case RTS:
		
        renderGeneral();
        renderFightingUI();
        quests();
        ballquest();
        buyshipquest();
        abductionquest();
		asteroidquest();

		break;

	case inSpaceStation:

        renderGeneral();
		renderNPC();
		renderNPC5();
		renderFightingUI();
		quests();
		ballquest();
		buyshipquest();
		abductionquest();
		asteroidquest();

		break;

	case inPlayerShip:

        renderGeneral();
		renderFightingUI();
        renderDistances();
		quests();
		ballquest();
		buyshipquest();
		abductionquest();
		asteroidquest();

		break;

	case inShop:

		renderShopMenu();
		renderFightingUI();

		break;

	case help:

		renderHelp();
		break;

	case gameOver:

		renderGameOverScreen();
		break;

	case waveTransition:

		renderWaveTransition();
		break;

	case exit:

		renderExit();
		break;

	}

	meshList[GEO_SELECTION] = MeshBuilder::GenerateSquare("Selection box", Color(0, 1, 0), initCursor, endCursor);
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_SELECTION], false);
	modelStack.PopMatrix();
	delete meshList[GEO_SELECTION];

	renderAllHitbox();
	RenderTextOnScreen(meshList[GEO_TEXT], FPSText, Color(1, 0, 0), 3, 0, 0);
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
            delete *it;
            it = allVehicles[i].erase(it);
        }

        while (!storedVehicles[i].empty())
        {
            delete storedVehicles[i].top();
            storedVehicles[i].pop();
        }
    }

    std::cout << "Clearing Explosions" << std::endl;
    for (vector<Explosion*>::iterator it = allExplosions.begin(); it != allExplosions.end();)
    {
        delete *it;
        it = allExplosions.erase(it);
    }

    std::cout << "Clearing Asteroids" << std::endl;
    for (vector<Asteroid*>::iterator it = Vasteroid.begin(); it != Vasteroid.end();)
    {
        delete *it;
        it = Vasteroid.erase(it);
    }

    std::cout << "Clearing Bullets" << std::endl;
    for (vector<Bullet*>::iterator it = allBullets.begin(); it != allBullets.end();){

        delete *it;
        it = allBullets.erase(it);
    }

	std::cout << "Clearing NPCS" << std::endl;
	for (vector<NPC*>::iterator it = allNPC.begin(); it != allNPC.end();)
	{
		delete *it;
		it = allNPC.erase(it);
	}

    delete playerShop;
	
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}


// Initializers

void SP2::objectsInit()
{
	//Object init
	station.SetPos(0, 0, 0);
	station.SetHitbox(AABB(-15, -25, -15, 15, 10, 15));

	LastLocation.SetPos(0, 1, 0);
	LastLocation.SetView(0, 0, 1);
	LastLocation.SetUp(0, 1, 0);
	LastLocation.SetRight(1, 0, 0);

	waveFunctions = new Wave();

	//Player Vehicle
	playerShip.SetPos(0, 10, 0);
	playerShip.SetView(0, 0, 1);
	playerShip.SetRight(-1, 0, 0);
	playerShip.SetUp(playerShip.Right, playerShip.View);
	playerShip.SetHitboxSize(-5, -3, -5, 4, 5, 5);
	playerShip.reset();
	playerShip.updateHitbox();

	//Vehicles Init
	allVehicles.insert(std::pair<int, vector<Vehicles*>>(GEO_SMALLSHIP, smallVehicles));
	allVehicles.insert(std::pair<int, vector<Vehicles*>>(GEO_MIDSHIP, midVehicles));
	allVehicles.insert(std::pair<int, vector<Vehicles*>>(GEO_LARGESHIP, largeVehicles));

	storedVehicles.insert(std::pair<int, stack<Vehicles*>>(GEO_SMALLSHIP, stackSmallVehicles));
	storedVehicles.insert(std::pair<int, stack<Vehicles*>>(GEO_MIDSHIP, stackMidVehicles));
	storedVehicles.insert(std::pair<int, stack<Vehicles*>>(GEO_LARGESHIP, stackLargeVehicles));
}

void SP2::npcInit()
{
	NPC5.SetPos(-6, 0, -7);
	NPC5.SetHitboxSize(2);

	ball.SetPos(4, 0.1, 7);
	ball.SetHitboxSize(1);

	NPC* newNPC = new NPC(Vector3(2, 0, -7), Vector3(0,0,1), 0.5f, 2, 3, GEO_NPC);
	newNPC->questName = "Find my ball";
	newNPC->questDialogue = "Find the ball and give it to pucboi: ";
	newNPC->questCompleteDialogue = "GUD BALL KTHXBYE";
	allNPC.push_back(newNPC);

	newNPC = new NPC(Vector3(2, 0, 3), Vector3(1, 0, 0), 0.5f, 2, 3, GEO_NPC2);
	newNPC->questName = "Too much asteroids, help KILL 5 kthx";
	newNPC->questDialogue = "DE_STROY asteroids: " + destroyed;
	newNPC->questCompleteDialogue = "YU HAV DONE W3LL";
	allNPC.push_back(newNPC);

	newNPC = new NPC(Vector3(-5, 0, 5), Vector3(0, 0, 1), 0.5f, 2, 4, GEO_NPC3);
	newNPC->questName = "buy any ship skillfully 3 times";
	newNPC->questDialogue = "Buy ships: ";
	newNPC->questCompleteDialogue = "GUD JOB YU ARE MLG";
	allNPC.push_back(newNPC);


	newNPC = new NPC(Vector3(-3, 0, -7), Vector3(0, 0, 1), 0.5f, 2, 5, GEO_NPC4);
	newNPC->questName = "help meh kidnapz cuti3 pi3";
	newNPC->questDialogue = "Abduction: ";
	newNPC->questCompleteDialogue = "teim for sum lovey dovey stuff ohhh yeaaaa";
	allNPC.push_back(newNPC);
}

void SP2::lightInit()
{
	light[0].position.Set(-10, 20, 20);
	light[0].color.Set(1, 1, 1);
	light[0].power = 0.5f;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	light[1].position.Set(0, 1, 0);
	light[1].color.Set(0, 0, 1);
	light[1].power = 0.5f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].type = Light::LIGHT_POINT;
	light[1].cosCutoff = cos(Math::DegreeToRadian(20));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	light[2].position.Set(0, 3, 0);
	light[2].color.Set(1, 0, 0);
	light[2].power = 0.f;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].type = Light::LIGHT_SPOT;
	light[2].cosCutoff = cos(Math::DegreeToRadian(45));
	light[2].cosInner = cos(Math::DegreeToRadian(30));
	light[2].exponent = 3.f;
	light[2].spotDirection.Set(0.f, 0.f, 1.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 3);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);
}

void SP2::shopInit()
{
	playerShop = new Shop(playerShip);

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
				shootingsfx->play3D("Sound/vehicleshooting.mp3", irrklang::vec3df(temp->Pos.x, temp->Pos.y, temp->Pos.z));

            }

            ++vitV;
        }
    }

}

void SP2::playerBulletCreation(double dt){

    if (Application::IsKeyPressed(VK_LBUTTON)){

        if (playerShip.fireBullets()){

            Bullet* newBullet = new Bullet(playerShip.View, playerShip.Pos, playerShip.bulletDamage);
            allBullets.push_back(newBullet);
			shootingsfx->play3D("Sound/shooting.mp3", irrklang::vec3df(playerShip.Pos.x, playerShip.Pos.y, playerShip.Pos.z));

        }

    }

}

void SP2::generateAsteroid(){

	int minOffset = -2000;
	int maxOffset = 2000;
	int viewOffset = 100 / waveFunctions->waveNumber;

    if (waveFunctions->spawnAsteroid() && Vasteroid.size() < waveFunctions->maxNumberOfAsteroids - waveFunctions->numberOfAsteroidsDestroyed){
		
		Asteroid* asteroid = new Asteroid(generate_range(5, 10 * waveFunctions->waveNumber));
	    Vector3 target(generate_range(-viewOffset, viewOffset), 0, generate_range(-viewOffset, viewOffset));

	    switch (generate_range(0, 4))
	    {
	    case 0:

	    	asteroid->SetPos(generate_range(camera.position.x + minOffset, camera.position.x + maxOffset), 0, generate_range(camera.position.z + maxOffset - 1000, camera.position.z + maxOffset));
	    	asteroid->SetView((target - asteroid->Pos).Normalized());
	    	break;

	    case 1:

	    	asteroid->SetPos(generate_range(camera.position.x + minOffset, camera.position.x + maxOffset), 0, generate_range(camera.position.z + minOffset - 1000, camera.position.z + minOffset));
	    	asteroid->SetView((target - asteroid->Pos).Normalized());
	    	break;

	    case 2:

	    	asteroid->SetPos(generate_range(camera.position.x + maxOffset - 1000, camera.position.x + maxOffset), 0, generate_range(camera.position.z + minOffset, camera.position.z + maxOffset));
	    	asteroid->SetView((target - asteroid->Pos).Normalized());
	    	break;

	    case 3:

	    	asteroid->SetPos(generate_range(camera.position.x + minOffset - 1000, camera.position.x + minOffset), 0, generate_range(camera.position.z + minOffset, camera.position.z + maxOffset));
	    	asteroid->SetView((target - asteroid->Pos).Normalized());
	    	break;

	    }

	    asteroid->updateHitbox();
  		Vasteroid.push_back(asteroid);
	}

}



// Renders
void SP2::renderExit()
{
	RenderOnScreen(meshList[GEO_HELPSCREEN], screenWidth / 2, screenHeight / 2, -20, 1, 90, 0, 0);

	RenderTextOnScreen(meshList[GEO_TEXT], "Are you sure you want to quit?", Color(0, 1, 0), 10 * objSize, 0.1f * screenWidth, 0.5 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "Yes", Color(0, 1, 0), 10 * objSize, 0.3f * screenWidth, 0.4 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "No", Color(0, 1, 0), 10 * objSize, 0.6f * screenWidth, 0.4 * screenHeight);
}

void SP2::renderGameOverScreen(){

	RenderOnScreen(meshList[GEO_HELPSCREEN], screenWidth / 2, screenHeight / 2, -20, 1, 90, 0, 0);

	RenderTextOnScreen(meshList[GEO_TEXT], "GAME OVER", Color(0, 1, 0), 30 * objSize, 0.15f * screenWidth, 0.7 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press R to restart", Color(0, 1, 0), 10 * objSize, 0.25f * screenWidth, 0.45 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press Esc to quit", Color(0, 1, 0), 10 * objSize, 0.26f * screenWidth, 0.35 * screenHeight);

}

void SP2::renderWaveTransition(){

    RenderOnScreen(meshList[GEO_HELPSCREEN], screenWidth / 2, screenHeight / 2, -20, 1, 90, 0, 0);

    RenderTextOnScreen(meshList[GEO_TEXT], "Wave " + std::to_string(waveFunctions->waveNumber), Color(0, 1, 0), 20 * objSize, 0.33f * screenWidth, 0.6 * screenHeight);
    RenderTextOnScreen(meshList[GEO_TEXT], "Complete!", Color(0, 1, 0), 20 * objSize, 0.26f * screenWidth, 0.4 * screenHeight);

}

void SP2::renderHelp()
{
	RenderOnScreen(meshList[GEO_HELPSCREEN], screenWidth / 2, screenHeight / 2, -20, 1, 90, 0, 0);

	RenderTextOnScreen(meshList[GEO_TEXT], "General Controls:", Color(0, 1, 0), 4 * objSize, 0.05f * screenWidth, 0.9 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press TAB to go into RTS mode", Color(0, 1, 0), 3 * objSize, 0.05f * screenWidth, 0.85 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "Walk to control panel and Press E to buy/upgrade ships", Color(0, 1, 0), 3 * objSize, 0.05f * screenWidth, 0.8 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press E when at teleporter to beam into your ship", Color(0, 1, 0), 3 * objSize, 0.05f * screenWidth, 0.75 * screenHeight);

	RenderTextOnScreen(meshList[GEO_TEXT], "Ship Controls:", Color(0, 1, 0), 4 * objSize, 0.05f * screenWidth, 0.65 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "W,S,A,D to control ship's yaw and pitch", Color(0, 1, 0), 3 * objSize, 0.05f * screenWidth, 0.6 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "Shift to increase Thrust", Color(0, 1, 0), 3 * objSize, 0.05f * screenWidth, 0.55 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "Ctrl to decrease Thrust", Color(0, 1, 0), 3 * objSize, 0.05f * screenWidth, 0.5 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "Left click to shoot", Color(0, 1, 0), 3 * objSize, 0.05f * screenWidth, 0.45 * screenHeight);

	RenderTextOnScreen(meshList[GEO_TEXT], "RTS controls:", Color(0, 1, 0), 4 * objSize, 0.05f * screenWidth, 0.35 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "1 to select Speeder", Color(0, 1, 0), 3 * objSize, 0.05f * screenWidth, 0.3 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "2 to select Fighter", Color(0, 1, 0), 3 * objSize, 0.05f * screenWidth, 0.25 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "3 to select Devastator", Color(0, 1, 0), 3 * objSize, 0.05f * screenWidth, 0.2 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "Left click to select ship, Drag to select multiple ship", Color(0, 1, 0), 3 * objSize, 0.05f * screenWidth, 0.15 * screenHeight);
	RenderTextOnScreen(meshList[GEO_TEXT], "Right click to give command to selected ships", Color(0, 1, 0), 3 * objSize, 0.05f * screenWidth, 0.1 * screenHeight);
}

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

	if (playerShip.isDead == false)
	{
		modelStack.PushMatrix();
		modelStack.Translate(playerShip.Pos.x, playerShip.Pos.y, playerShip.Pos.z);
		
		modelStack.Rotate(playerShip.pitch, playerShip.Right.x, 0, playerShip.Right.z);
		modelStack.Rotate(playerShip.yaw, 0, 1, 0);
		RenderMesh(meshList[GEO_XWING], enableLight);
		modelStack.PopMatrix();
	}


    for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
    {
        vector<Vehicles*>::iterator it = allVehicles[i].begin();
        while (it != allVehicles[i].end())
        {
            modelStack.PushMatrix();
            modelStack.Translate((*it)->Pos.x, (*it)->Pos.y, (*it)->Pos.z);
            modelStack.Rotate((*it)->Yaw, 0, 1, 0);
            RenderMesh(meshList[i], enableLight);
            modelStack.PopMatrix();

            if ((*it)->health < (*it)->maxHealth){

                renderHealthBar((*it)->Pos, 10, (*it)->health, Color(0, 0, 1));

            }

            it++;
        }

    } 

	if (!selection.empty()){

		for (vector<Vehicles*>::iterator it = selection.begin(); it != selection.end(); ++it)
		{
			meshList[GEO_HITBOX] = MeshBuilder::GenerateSquare("Hitbox", Color(0, 1, 0), (*it)->interaction.GetMin(), (*it)->interaction.GetMax());
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			modelStack.PushMatrix();
			RenderMesh(meshList[GEO_HITBOX], false);
			modelStack.PopMatrix();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            delete meshList[GEO_HITBOX];
		}

	}

	if (place)
	{
		modelStack.PushMatrix();
		modelStack.Translate(place->Pos.x, place->Pos.y, place->Pos.z);
		modelStack.Rotate(place->initialYaw, 0, 1, 0);
		RenderMesh(meshList[placeType], enableLight);
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

		RenderTextOnScreen(meshList[GEO_TEXT], "Damage: " + std::to_string(playerShip.bulletDamage), Color(0, 1, 0), objSize * 5, 0.13f * screenWidth, 0.15f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "$" + std::to_string(playerShop->damageUpgradePrice), Color(0, 1, 0), objSize * 5, 0.17f * screenWidth, 0.10f * screenHeight, 9);

		RenderTextOnScreen(meshList[GEO_TEXT], "Fire Rate: " + std::to_string(playerShip.bulletFireRate / 100) + "RPS", Color(0, 1, 0), objSize * 5, 0.4f * screenWidth, 0.15f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "$" + std::to_string(playerShop->fireRateUpgradePrice), Color(0, 1, 0), objSize * 5, 0.48f * screenWidth, 0.10f * screenHeight, 9);

		RenderTextOnScreen(meshList[GEO_TEXT], "Increase Max Health", Color(0, 1, 0), objSize * 5, 0.68f * screenWidth, 0.15f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "$" + std::to_string(playerShop->healthUpgradePrice), Color(0, 1, 0), objSize * 5, 0.775f * screenWidth, 0.10f * screenHeight, 9);

		RenderOnScreen(meshList[GEO_ATTACKUP], 0.2f * screenWidth, 0.3f * screenHeight, 2, 0.1f * objSize, 90, 0, 0);
		RenderOnScreen(meshList[GEO_FIRERATEUP], 0.5f * screenWidth, 0.3f * screenHeight, 2, 0.1f *  objSize, 90, 0, 0);
		RenderOnScreen(meshList[GEO_HEALTHUP], 0.8f * screenWidth, 0.3f * screenHeight, 2, 0.1f *  objSize, 90, 0, 0);
		break;
	case FirstShip:
		RenderTextOnScreen(meshList[GEO_TEXT], "Speeder", Color(0, 1, 0), objSize * 12, 0.39f * screenWidth, 0.9f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "Buy", Color(0, 1, 0), objSize * 10, 0.46f * screenWidth, 0.2f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "$500", Color(0, 1, 0), objSize * 8, 0.453f * screenWidth, 0.135f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "Current Amount: " + std::to_string(storedVehicles[GEO_SMALLSHIP].size() + allVehicles[GEO_SMALLSHIP].size()), Color(0, 1, 0), objSize * 5, 0.39f * screenWidth, 0.1f * screenHeight, 9);
		RenderOnScreen(meshList[GEO_SMALLSHIP], shopSmallPos, shopSmallScale, 20, shopSmallRot, 0);
		break;
	case SecondShip:
		RenderTextOnScreen(meshList[GEO_TEXT], "Fighter", Color(0, 1, 0), objSize * 12, 0.39f * screenWidth, 0.9f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "Buy", Color(0, 1, 0), objSize * 10, 0.46f * screenWidth, 0.2f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "$1000", Color(0, 1, 0), objSize * 8, 0.44f * screenWidth, 0.133f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "Current Amount: " + std::to_string(storedVehicles[GEO_MIDSHIP].size() + allVehicles[GEO_MIDSHIP].size()), Color(0, 1, 0), objSize * 5, 0.39f * screenWidth, 0.1f * screenHeight, 9);
		RenderOnScreen(meshList[GEO_MIDSHIP], shopMidPos, shopMidScale, 20, shopMidRot, 0);
		break;
	case ThirdShip:
		RenderTextOnScreen(meshList[GEO_TEXT], "Devastator", Color(0, 1, 0), objSize * 12, 0.32f * screenWidth, 0.9f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "Buy", Color(0, 1, 0), objSize * 10, 0.46f * screenWidth, 0.2f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "$1500", Color(0, 1, 0), objSize * 8, 0.44f * screenWidth, 0.135f * screenHeight, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "Current Amount: " + std::to_string(storedVehicles[GEO_LARGESHIP].size() + allVehicles[GEO_LARGESHIP].size()), Color(0, 1, 0), objSize * 5, 0.39f * screenWidth, 0.1f * screenHeight, 9);
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

	if (displayHelp == false)
	{
		if (playerShip.health > 0){
			RenderTextOnScreen(meshList[GEO_TEXT], "HP:" + std::to_string(playerShip.health), Color(0, 1, 0), objSize * 8, 0.02f * screenWidth, screenHeight * 0.9f, 50);
		}
		else{
			RenderTextOnScreen(meshList[GEO_TEXT], "Ship is Destroyed!!!", Color(0, 1, 0), objSize * 8, 0.02f * screenWidth, screenHeight * 0.9f, 50);
		}
		if (state == RTS)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Speeder: " + std::to_string(storedVehicles[GEO_SMALLSHIP].size()), Color(0, 1, 0), objSize * 8, 0.02f * screenWidth, screenHeight * 0.79f, 50);
			RenderTextOnScreen(meshList[GEO_TEXT], "Fighter: " + std::to_string(storedVehicles[GEO_MIDSHIP].size()), Color(0, 1, 0), objSize * 8, 0.02f * screenWidth, screenHeight * 0.73f, 50);
			RenderTextOnScreen(meshList[GEO_TEXT], "Devastator: " + std::to_string(storedVehicles[GEO_LARGESHIP].size()), Color(0, 1, 0), objSize * 8, 0.02f * screenWidth, screenHeight * 0.67f, 50);
		}
		RenderTextOnScreen(meshList[GEO_TEXT], "Thrust: " + std::to_string((int)(playerShip.thrust)), Color(0, 1, 0), objSize * 8, 0.02f * screenWidth, screenHeight * 0.84f, 50);
		RenderTextOnScreen(meshList[GEO_TEXT], "Cash: $" + std::to_string(currMoney), Color(0, 1, 0), objSize * 8, 0.75f * screenWidth, screenHeight * 0.9f, 50);

		if (state != inShop)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Station HP: " + std::to_string((int)(waveFunctions->stationHealth)), Color(0, 1, 0), objSize * 5, 0.39f * screenWidth, screenHeight * 0.9f, 50);
			if (waveFunctions->currWaveCooldownTime >= waveFunctions->maxWaveCooldownTime)
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "Asteroids: " + std::to_string((int)(waveFunctions->maxNumberOfAsteroids - waveFunctions->numberOfAsteroidsDestroyed)), Color(0, 1, 0), objSize * 5, 0.41f * screenWidth, screenHeight * 0.85f, 50);
			}
			else
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "Next Wave: " + std::to_string((int)(waveFunctions->maxWaveCooldownTime - waveFunctions->currWaveCooldownTime)), Color(0, 1, 0), objSize * 5, 0.41f * screenWidth, screenHeight * 0.85f, 50);
			}
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
            allHitbox.push_back((*it)->hitbox);
			it++;
		}
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void SP2::renderBullets(){

    for (vector<Bullet*>::iterator vitB = allBullets.begin(); vitB != allBullets.end(); vitB++){

        modelStack.PushMatrix();

        modelStack.Translate((*vitB)->Pos.x, (*vitB)->Pos.y, (*vitB)->Pos.z);
        modelStack.Scale(1.5f, 1.f, 1.5f);
        RenderMesh(meshList[GEO_BULLETS], false);

        modelStack.PopMatrix();

      
    }

}

void SP2::renderAsteroid()
{
    
	for (vector<Asteroid*>::iterator it = Vasteroid.begin(); it != Vasteroid.end(); ++it)
    {
		modelStack.PushMatrix();
        modelStack.Translate((*it)->Pos.x, (*it)->Pos.y, (*it)->Pos.z);
        modelStack.Scale((*it)->size, (*it)->size, (*it)->size);
		RenderMesh(meshList[GEO_ASTEROID], enableLight);
		modelStack.PopMatrix();

        if ((*it)->health < (*it)->maxHealth){

            renderHealthBar((*it)->Pos, (*it)->size, (*it)->health, Color(1, 0, 0));

        }

	}
   
}

void SP2::renderExplosion()
{
    for (vector<Explosion*>::iterator vitE = allExplosions.begin(); vitE != allExplosions.end(); ++vitE){
        
        modelStack.PushMatrix();
        modelStack.Translate((*vitE)->Pos.x, (*vitE)->Pos.y, (*vitE)->Pos.z);
        modelStack.Rotate((*vitE)->getYaw(), 0, 1, 0);
        modelStack.Rotate((*vitE)->getPitch(), 0, 0, 1);
        modelStack.Scale((*vitE)->getExplosionSize(), (*vitE)->getExplosionSize(), (*vitE)->getExplosionSize());
        RenderMesh(meshList[GEO_EXPLOSION], false);
        modelStack.PopMatrix();

     
    }

}

void SP2::renderHealthBar(Vector3 asteroidPosition, int asteroidSize, int health, Color color){

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

        //Health bar
        meshList[GEO_HEALTHBAR] = MeshBuilder::GenerateQuad("Asteroid Health Bar", color, 3, 1);

        modelStack.PushMatrix();
        modelStack.Translate(asteroidPosition.x, asteroidPosition.y + asteroidSize * 1.5, asteroidPosition.z);
        modelStack.Rotate(asteroidHealthYaw, 0, 1, 0);
        modelStack.Rotate(asteroidHealthPitch, 0, 0, 1);
        modelStack.Scale(1, 1, health / 2);
        RenderMesh(meshList[GEO_HEALTHBAR], false);
        modelStack.PopMatrix();

        delete meshList[GEO_HEALTHBAR];

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

void SP2::renderWayPoints(){
    for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
    {
        vector<Vehicles*>::iterator it = allVehicles[i].begin();
        
        while (it != allVehicles[i].end()){

            queue<Vector3> currQueue = (*it)->newVehicle.getwayPoints();
            while (!currQueue.empty()){

                modelStack.PushMatrix();
                modelStack.Translate(currQueue.front().x, currQueue.front().y, currQueue.front().z);
                RenderMesh(meshList[GEO_LIGHTBALL], false);
                modelStack.PopMatrix();
                currQueue.pop();

            }

            it++;

        }
    }

}

void SP2::renderGeneral(){

	renderSkybox();
	renderShips();
	renderAsteroid();
    renderBullets();
    renderWayPoints();
    renderStation();
    renderAsteroid();
	renderExplosion();
}

void SP2::renderNPC()
{
	for (vector<NPC*>::iterator it = allNPC.begin(); it != allNPC.end(); ++it)
	{
		//NPC
		modelStack.PushMatrix();
		modelStack.Translate((*it)->Pos.x, (*it)->Pos.y, (*it)->Pos.z);

		modelStack.PushMatrix();
		modelStack.Rotate((*it)->yaw, 0, 1, 0);
		modelStack.Scale((*it)->NPCsize, (*it)->NPCsize, (*it)->NPCsize);
		RenderMesh(meshList[(*it)->mesh], enableLight);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 2.2, 0);
		modelStack.Rotate((*it)->yaw, 0, 1, 0);
		modelStack.Rotate((*it)->rotateHand, 1, 0, 0);
		modelStack.Translate(0, -2.2, 0);
		modelStack.Scale((*it)->NPCsize, (*it)->NPCsize, (*it)->NPCsize);
		RenderMesh(meshList[GEO_LEFTHAND], enableLight);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 2.2, 0);
		modelStack.Rotate((*it)->yaw, 0, 1, 0);
		modelStack.Rotate(-(*it)->rotateHand, 1, 0, 0);
		modelStack.Translate(0, -2.2, 0);
		modelStack.Scale((*it)->NPCsize, (*it)->NPCsize, (*it)->NPCsize);
		RenderMesh(meshList[GEO_RIGHTHAND], enableLight);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0.5, 0);
		modelStack.Rotate((*it)->yaw, 0, 1, 0);
		modelStack.Rotate((*it)->rotateLeg, 1, 0, 0);
		modelStack.Translate(0, -0.5, 0);
		modelStack.Scale((*it)->NPCsize, (*it)->NPCsize, (*it)->NPCsize);
		RenderMesh(meshList[GEO_RIGHTLEG], enableLight);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0.5, 0);
		modelStack.Rotate((*it)->yaw, 0, 1, 0);
		modelStack.Rotate(-(*it)->rotateLeg, 1, 0, 0);
		modelStack.Translate(0, -0.5, 0);
		modelStack.Scale((*it)->NPCsize, (*it)->NPCsize, (*it)->NPCsize);
		RenderMesh(meshList[GEO_LEFTLEG], enableLight);
		modelStack.PopMatrix();

		modelStack.PopMatrix();
	}
}

void SP2::renderNPC5()
{
	//npc
	if (kidnap == false)
	{

		modelStack.PushMatrix();
		modelStack.Translate(NPC5.Pos.x, NPC5.Pos.y, NPC5.Pos.z);
		modelStack.Rotate(90, 0, 1, 0);

		modelStack.PushMatrix();
		modelStack.Scale(0.25, 0.25, 0.25);
		RenderMesh(meshList[GEO_NPC5], enableLight);



		modelStack.PushMatrix();
		modelStack.Translate(0, 1, 0);
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMesh(meshList[GEO_RIGHTLEG], enableLight);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 1.2, 0);
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMesh(meshList[GEO_LEFTLEG], enableLight);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}
}

void SP2::renderchildtext(){

	float Yaw = 0;
	float Pitch = 0;
	Vector3 initView(0, 0, 1);
	Vector3 view = (camera.position - NPC5.Pos).Normalized();
	Vector3 XZview(view.x, 0, view.z);
	XZview.Normalize();

	Vector3 normal = initView.Cross(view);
	Yaw = Math::RadianToDegree(acos(initView.Dot(XZview)));

	if (normal.y < 0){

		Yaw *= -1;

	}

	modelStack.PushMatrix();




	modelStack.Translate(-6, 0.5, -7);

	modelStack.Rotate(Yaw, 0, 1, 0);
	modelStack.Scale(0.05, 0.05, 0.05);
	modelStack.PushMatrix();

	//modelStack.Translate(-9, 0, 0);

	RenderText(meshList[GEO_TEXT2], "PLZ KIDNAPZ MEH I NID ROV <3", Color(0, 1, 0));


	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void SP2::renderTextonball(){

	float Yaw = 0;
	float Pitch = 0;
	Vector3 initView(0, 0, 1);
	Vector3 view = (camera.position - ball.Pos).Normalized();
	Vector3 XZview(view.x, 0, view.z);
	XZview.Normalize();

	Vector3 normal = initView.Cross(view);
	Yaw = Math::RadianToDegree(acos(initView.Dot(XZview)));

	if (normal.y < 0){

		Yaw *= -1;

	}

	modelStack.PushMatrix();




	modelStack.Translate(4, 0.3, 7);

	modelStack.Rotate(Yaw, 0, 1, 0);
	modelStack.Scale(0.05, 0.05, 0.05);
	modelStack.PushMatrix();

	modelStack.Translate(-9, 0, 0);


	RenderText(meshList[GEO_TEXT], "Press 'E' to pick up", Color(0, 1, 0));

	modelStack.PopMatrix();
	modelStack.PopMatrix();


}



//Updates

void SP2::gameOverUpdate(){

	if (Application::IsKeyPressed('R')){

		Exit();
		Init();

	}
	else if (Application::IsKeyPressed(VK_ESCAPE)){

		state = gameOver;

	}


}

void SP2::quests()
{
	// Ball Quest
	if (Application::IsKeyPressed('E') && allNPC[0]->hitbox.PointToAABB(camera.position))
	{
		currentQuest = ballQuest;
		if (allNPC[0]->complete == false)
		{
			RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[0]->questName, Color(0, 1, 0), 3, 0, 10);
		}
		if (pickup == true)
		{
			pickup = false;
			allNPC[0]->complete = true;
		}
	}

	if (Application::IsKeyPressed('E') && ball.hitbox.PointToAABB(camera.position))
	{
		pickup = true;
	}

	// asteroids
	if (Application::IsKeyPressed('E') && allNPC[1]->hitbox.PointToAABB(camera.position) && allNPC[1]->complete == false)
	{
		//talking = true;
		currentQuest = asteroidQuest;
		if (allNPC[1]->complete == false)
		{
			RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[1]->questName, Color(0, 1, 0), 3, 0, 10);
		}
		if (destroyed >= 1)
		{
			allNPC[1]->complete = true;
		}
	}

	//buyship
	if (Application::IsKeyPressed('E') && allNPC[2]->hitbox.PointToAABB(camera.position) && allNPC[2]->complete == false)
	{
		//talking = true;
		currentQuest = buyshipQuest;
		if (allNPC[2]->complete == false)
		{
			RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[2]->questName, Color(0, 1, 0), 3, 0, 10);
		}
		if (shipbought >= 3)
		{
			allNPC[2]->complete = true;
		}
	}

	//abduction
	if (Application::IsKeyPressed('E') && allNPC[3]->hitbox.PointToAABB(camera.position) && allNPC[3]->complete == false)
	{
		//talking = true;
		currentQuest = abductionQuest;
		if (allNPC[3]->complete == false)
		{
			RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[3]->questName, Color(0, 1, 0), 3, 0, 10);
		}
		if (kidnap == true)
		{
			allNPC[3]->complete = true;
		}
	}


}

void SP2::NPCUpdates(double dt){
	for (vector<NPC*>::iterator it = allNPC.begin(); it != allNPC.end(); ++it)
	{
		(*it)->update(dt);
	}

	ball.updateHitbox();
	NPC5.updateHitbox();
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

	if (Application::IsKeyPressed(VK_TAB) && sharedData::GetInstance()->Delay(0.5f))
	{
		Application::centerMouse();
        selection.clear();
		place = nullptr;
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
	else if (Application::IsKeyPressed('1') && sharedData::GetInstance()->Delay(0.5f))
	{
		if (!storedVehicles[GEO_SMALLSHIP].empty())
		{
			place = storedVehicles[GEO_SMALLSHIP].top();
			placeType = GEO_SMALLSHIP;
		}	
	}
	else if (Application::IsKeyPressed('2') && sharedData::GetInstance()->Delay(0.5f))
	{
		if (!storedVehicles[GEO_MIDSHIP].empty())
		{
			place = storedVehicles[GEO_MIDSHIP].top();
			placeType = GEO_MIDSHIP;
		}
	}
	else if (Application::IsKeyPressed('3') && sharedData::GetInstance()->Delay(0.5f))
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
		else if (Application::IsKeyPressed(VK_LBUTTON) && sharedData::GetInstance()->Delay(0.5f))
		{
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
				if (currMoney - playerShop->damageUpgradePrice >= 0)
				{
					currMoney -= playerShop->damageUpgradePrice;
					playerShop->increaseDamage();
				}
			}
			else if (mouseX > 0.4f * screenWidth && mouseX < 0.6f * screenWidth && mouseY > 0.2f * screenHeight && mouseY < 0.4f * screenHeight)
			{
				if (currMoney - playerShop->fireRateUpgradePrice >= 0)
				{
					currMoney -= playerShop->fireRateUpgradePrice;
					playerShop->increaseFireRate();

				}
			}
			else if (mouseX > 0.7f * screenWidth && mouseX < 0.9f * screenWidth && mouseY > 0.2f * screenHeight && mouseY < 0.4f * screenHeight)
			{
				if (currMoney - playerShop->healthUpgradePrice >= 0)
				{
					currMoney -= playerShop->healthUpgradePrice;
					playerShop->increaseHealth();

				}
			}
		}
		else if (Application::IsKeyPressed(VK_ESCAPE) && sharedData::GetInstance()->Delay(0.5f))
		{
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
		if (Application::IsKeyPressed(VK_LBUTTON) && sharedData::GetInstance()->Delay(0.5f) && mouseX > 0.4f * screenWidth && mouseX < 0.6f * screenWidth && mouseY > 0.2f * screenHeight && mouseY < 0.3f * screenHeight)
		{
			if (currMoney - 500 >= 0)
			{
				currMoney -= 500;
				smallShip = new Vehicles(Vector3(0, 0, 0), Vector3(1, 0, 0), 50, 20, 500, 10);
				smallShip->SetHitboxSize(-10, -10, -10, 10, 10, 10);
				smallShip->SetInteractionSize(10, 10, 10, 10, 10, 10);
				storedVehicles[GEO_SMALLSHIP].push(smallShip);
				shipbought++;
			}
		}

		if (Application::IsKeyPressed(VK_ESCAPE) && sharedData::GetInstance()->Delay(0.5f))
		{
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

		if (Application::IsKeyPressed(VK_LBUTTON) && sharedData::GetInstance()->Delay(0.5f) && mouseX > 0.4f * screenWidth && mouseX < 0.6f * screenWidth && mouseY > 0.2f * screenHeight && mouseY < 0.3f * screenHeight)
		{
			if (currMoney - 1000 >= 0)
			{
				currMoney -= 1000;
				midShip = new Vehicles(Vector3(0, 0, 0), Vector3(1, 0, 0), 35, 50, 200, 30);
				midShip->SetHitboxSize(-15, -10, -15, 15, 10, 15);
				midShip->SetInteractionSize(15, 10, 15, 15, 10, 15);
				storedVehicles[GEO_MIDSHIP].push(midShip);
				shipbought++;
			}
		}

		if (Application::IsKeyPressed(VK_ESCAPE) && sharedData::GetInstance()->Delay(0.5f))
		{
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

		if (Application::IsKeyPressed(VK_LBUTTON) && sharedData::GetInstance()->Delay(0.5f) && mouseX > 0.4f * screenWidth && mouseX < 0.6f * screenWidth && mouseY > 0.2f * screenHeight && mouseY < 0.3f * screenHeight)
		{
			if (currMoney - 1500 >= 0)
			{
				currMoney -= 1500;
				largeShip = new Vehicles(Vector3(0, 0, 0), Vector3(1, 0, 0), 20, 100, 50, 100);
				largeShip->SetHitboxSize(-20, -10, -20, 20, 10, 20);
				largeShip->SetInteractionSize(20, 10, 20, 20, 10, 20);
				storedVehicles[GEO_LARGESHIP].push(largeShip);
				shipbought++;
			}
		}

		if (Application::IsKeyPressed(VK_ESCAPE) && sharedData::GetInstance()->Delay(0.5f))
		{
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
            (*it)->update(dt);
            if ((*it)->checkMaxDistance(camera.position)){

                delete (*it);
                it = allVehicles[i].erase(it);
               
            
            }
            else{
                it++;
            }
		}
	}

}

void SP2::bulletUpdates(double dt){

    for (vector<Bullet*>::iterator vitB = allBullets.begin(); vitB != allBullets.end();){

        Bullet* temp = *vitB;

        temp->bulletUpdate(dt);
        if (temp->furtherThanBulletMaxRange()){

            allExplosions.push_back(new Explosion(15, 100, temp->Pos));
            delete temp;
            vitB = allBullets.erase(vitB);
            
            

        }
        else{

            vitB++;

        }

    }

}

void SP2::asteroidUpdate(double dt){

	for (vector<Asteroid*>::iterator it = Vasteroid.begin(); it != Vasteroid.end();)
	{
		Asteroid* asteroid = *it;
		asteroid->update(dt);

        if (asteroid->curRange > asteroid->maxRange || asteroid->health < 0){

            waveFunctions->numberOfAsteroidsDestroyed++;
            delete asteroid;
            it = Vasteroid.erase(it);

        }
        else{

            it++;

        }   

	}

	if (Timer(0, dt) == true)
	{
		generateAsteroid();
	}

}

void SP2::generalUpdates(double dt){

	playerShip.update(dt, worldHitbox);
	asteroidUpdate(dt);
	bulletUpdates(dt);

	explosionUpdate(dt);
	vehicleUpdates(dt);
	checkHitboxes();
	if (playerShip.respawn(10) == true)
	{
		playerShip.reset();
		playerShip.SetPos(0, 15, 0);
		playerShip.SetView(0, 0, 1);
		playerShip.SetRight(-1, 0, 0);
		playerShip.SetUp(0, 1, 0);
		playerShip.SetHitboxSize(5);
	}

	if (Application::IsKeyPressed('H') && sharedData::GetInstance()->Delay(0.5f))
	{
		lastState = state;
		state = help;
	}

	waveFunctions->waveUpdate(dt);

    if (waveFunctions->waveComplete()){
		lastState = state;
        state = waveTransition;

    }


	shootingsfx->setListenerPosition(irrklang::vec3df(camera.position.x, camera.position.y, camera.position.z), irrklang::vec3df(camera.view.x, camera.view.y, camera.view.z));
	explosionsfx->setListenerPosition(irrklang::vec3df(camera.position.x, camera.position.y, camera.position.z), irrklang::vec3df(camera.view.x, camera.view.y, camera.view.z));

}

void SP2::mainMenuUpdates(double dt){

	camera.EnableCursor();
	if (Application::IsKeyPressed(VK_LBUTTON) && sharedData::GetInstance()->Delay(0.5f))
	{
		Application::centerMouse();
		state = inSpaceStation;
		camera.Init(LastLocation.Pos, LastLocation.Pos + LastLocation.View);
	}


}

void SP2::explosionUpdate(double dt){

    for (vector<Explosion*>::iterator vitE = allExplosions.begin(); vitE != allExplosions.end();){

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
		
        (*vitE)->setPitchandYaw(explosionPitch, explosionYaw);
        (*vitE)->explosionUpdate(dt);

        if ((*vitE)->explosionEnd()){

            delete (*vitE);
            vitE = allExplosions.erase(vitE);


        }
        else{

            vitE++;

		}

    }
}

void SP2::inPlayerShipUpdates(double dt){

	camera.DisableCursor();
	camera.TPSMovement(dt, playerShip);
	playerBulletCreation(dt);
	shipBulletCreation(dt);

	if (Application::IsKeyPressed('E') && sharedData::GetInstance()->Delay(0.5f))
	{
		if (playerShip.hitbox.AABBtoAABB(Interactions[2], playerShip.View))
		{
			playerShip.yaw = 0;
			playerShip.pitch = 0;
			playerShip.thrust = 0;
			playerShip.SetPos(0, 10, 0);
			playerShip.SetView(0, 0, 1);
			playerShip.SetRight(-1, 0, 0);
			playerShip.SetUp(0, 1, 0);
			playerShip.updateHitbox();
			camera.Init(LastLocation.Pos, LastLocation.Pos + LastLocation.View);
			state = inSpaceStation;
			
		}
	}
	else if (Application::IsKeyPressed(VK_TAB) && sharedData::GetInstance()->Delay(0.5f))
	{
		state = RTS;
		lastState = inPlayerShip;
		camera.PointAt(playerShip, 50, -200);

	}

	if (playerShip.isDead == true)
	{
		state = inSpaceStation;
		camera.Init(LastLocation.Pos, LastLocation.Pos + LastLocation.View);
	}

	


}

void SP2::waveTransitionUpdate(double dt){

    waveFunctions->waveUpdate(dt);

    if (waveFunctions->endOfTransition){

        if (lastState == inSpaceStation){

            state = inSpaceStation;
            camera.Init(LastLocation.Pos, LastLocation.Pos + LastLocation.View);

        }
        else if (lastState == inPlayerShip){

            state = inPlayerShip;
            camera.PointAt(playerShip, 10, -30);

        }
        else{

            Application::centerMouse();
            state = RTS;
            camera.PointAt(playerShip, 50, -200);
        }

		currMoney += 1000 * waveFunctions->waveNumber;

    }
    

}

void SP2::inSpaceStationUpdates(double dt){

	camera.DisableCursor();
	camera.FPSMovement(dt, worldHitbox);
	if (Application::IsKeyPressed('E') && sharedData::GetInstance()->Delay(0.5f))
	{
		if (Interactions[0].PointToAABB(camera.position) && playerShip.isDead == false)
		{
			state = inPlayerShip;
			LastLocation.SetPos(camera.position.x, camera.position.y, camera.position.z);
			LastLocation.SetView(camera.view.x, camera.view.y, camera.view.z);
			camera.PointAt(playerShip, 10, -30);
		}
		else if (Interactions[1].PointToAABB(camera.position))
		{
			state = inShop;
		}
	}

	if (Application::IsKeyPressed(VK_TAB) && sharedData::GetInstance()->Delay(0.5f))
	{
		state = RTS;
		lastState = inSpaceStation;
		LastLocation.SetPos(camera.position.x, camera.position.y, camera.position.z);
		LastLocation.SetView(camera.view.x, camera.view.y, camera.view.z);
		camera.PointAt(playerShip, 50, -200);
	}

	NPCUpdates(dt);

}


//Hitbox

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
    for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i){

        bool checkHitbox = false;

        vector<Vehicles*>::iterator vitV1 = allVehicles[i].begin();
        
        while (vitV1 != allVehicles[i].end()){

            for (int j = GEO_SMALLSHIP; j <= GEO_LARGESHIP; ++j){

                vector<Vehicles*>::iterator vitV2 = allVehicles[j].begin();

                while (vitV2 != allVehicles[j].end()){

                    if ((*vitV2)->hitbox.AABBtoAABB((*vitV1)->hitbox) && (*vitV1) != (*vitV2)){

                        checkHitbox = true;
                        (*vitV1)->health -= (*vitV2)->maxHealth;
                        (*vitV2)->health -= (*vitV1)->maxHealth;

                        if ((*vitV2)->health <= 0){

                            removeOneSelection((*vitV2));
                            allExplosions.push_back(new Explosion((*vitV2)->maxHealth / 2, 50, (*vitV2)->Pos));
                            explosionsfx->play3D("Sound/vehicleboom.mp3", irrklang::vec3df((*vitV2)->Pos.x, (*vitV2)->Pos.y, (*vitV2)->Pos.z));
                            delete (*vitV2);
                            allVehicles[j].erase(vitV2);

                        }

                        break;

                    }
                    else{

                        vitV2++;

                    }

                }

                if (checkHitbox == true){

                    break;

                }

            }

            if ((*vitV1)->health <= 0){

                removeOneSelection((*vitV1));
                allExplosions.push_back(new Explosion((*vitV1)->maxHealth / 2, 50, (*vitV1)->Pos));
                explosionsfx->play3D("Sound/vehicleboom.mp3", irrklang::vec3df((*vitV1)->Pos.x, (*vitV1)->Pos.y, (*vitV1)->Pos.z));
                delete (*vitV1);
                allVehicles[i].erase(vitV1);
                break;

            }
            else{

                vitV1++;

            }

            if (checkHitbox == true){

                break;

            }

        }

        if (checkHitbox == true){

            break;

        }


    }

	//Player playerShip to Ship
	for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
	{
		vector<Vehicles*>::iterator it = allVehicles[i].begin();
		while (it != allVehicles[i].end())
		{
            if ((*it)->hitbox.AABBtoAABB(playerShip.hitbox))
			{
				Vehicles* Vtemp = *it;
				storedVehicles[i].push(Vtemp);
				removeOneSelection(Vtemp);
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
                removeOneSelection(Vtemp);
				it = allVehicles[i].erase(it);
			}
            else{
                ++it;
            }
            
		}
	}

	// Asteroid to Station
	for (vector<Asteroid*>::iterator it = Vasteroid.begin(); it != Vasteroid.end();)
	{
        if ((*it)->hitbox.AABBtoAABB(station.hitbox, (*it)->View) == true)
		{
			vehiclesRemoveTarget((*it));
			waveFunctions->numberOfAsteroidsDestroyed += 1;
			waveFunctions->stationHealth -= (*it)->size * 20;
			std::cout << waveFunctions->stationHealth << std::endl;
            allExplosions.push_back(new Explosion((*it)->size * 2, 50, (*it)->Pos));
            explosionsfx->play3D("Sound/asteroidboom.mp3", irrklang::vec3df((*it)->Pos.x, (*it)->Pos.y, (*it)->Pos.z));
            delete (*it);
			it = Vasteroid.erase(it);
		}
        else{
            it++;
        }

	}



}

void SP2::asteroidHitboxCheck(){

    //Bullet To Asteroid
    for (vector<Asteroid*>::iterator vitA = Vasteroid.begin(); vitA != Vasteroid.end();){
   
        for (vector<Bullet*>::iterator vitB = allBullets.begin(); vitB != allBullets.end();){

            if ((*vitA)->hitbox.PointToAABB((*vitB)->Pos)){

                allExplosions.push_back(new Explosion(15, 100, (*vitB)->Pos));
                (*vitA)->health -= (*vitB)->getBulletDamage();
                delete (*vitB);
                vitB = allBullets.erase(vitB);
            }
            else{

                vitB++;

            }

        }

        if ((*vitA)->health <= 0){

			waveFunctions->numberOfAsteroidsDestroyed += 1;
            vehiclesRemoveTarget((*vitA));
            currMoney += (*vitA)->size * 10;
            allExplosions.push_back(new Explosion((*vitA)->size * 2, 50, (*vitA)->Pos));
			explosionsfx->play3D("Sound/asteroidboom.mp3", irrklang::vec3df((*vitA)->Pos.x, (*vitA)->Pos.y, (*vitA)->Pos.z));
            delete (*vitA);
            vitA = Vasteroid.erase(vitA);
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
            for (vector<Asteroid*>::iterator Ait = Vasteroid.begin(); Ait != Vasteroid.end();)
            {
                Asteroid* tempAst = *Ait;
                if ((*Vit)->hitbox.AABBtoAABB(tempAst->hitbox))
                {
					waveFunctions->numberOfAsteroidsDestroyed += 1;

                    Vector3 ExploCenter = (*Vit)->Pos + tempAst->Pos;
                    ExploCenter /= 2;
					allExplosions.push_back(new Explosion(tempAst->size * 2, 50, ExploCenter));

                    (*Vit)->health -= tempAst->size * 2;
					explosionsfx->play3D("Sound/asteroidboom.mp3", irrklang::vec3df(tempAst->Pos.x, tempAst->Pos.y, tempAst->Pos.z));
                    delete tempAst;
                    Ait = Vasteroid.erase(Ait);

                }
                else
                {
                    Ait++;
                }

            }

            if ((*Vit)->isDead == true)
            {
				
                removeOneSelection((*Vit));
                explosionsfx->play3D("Sound/vehicleboom.mp3", irrklang::vec3df((*Vit)->Pos.x, (*Vit)->Pos.y, (*Vit)->Pos.z));
                delete (*Vit);
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
        if ((*Ait)->hitbox.AABBtoAABB(playerShip.hitbox)){

			waveFunctions->numberOfAsteroidsDestroyed += 1;
            playerShip.health -= (*Ait)->size * 2;
            Vector3 ExploCenter = playerShip.Pos + (*Ait)->Pos;
            ExploCenter /= 2;
            allExplosions.push_back(new Explosion((*Ait)->size * 2, 50, ExploCenter));
            explosionsfx->play3D("Sound/asteroidboom.mp3", irrklang::vec3df((*Ait)->Pos.x, (*Ait)->Pos.y, (*Ait)->Pos.z));
            vehiclesRemoveTarget((*Ait));
            delete (*Ait);
            Ait = Vasteroid.erase(Ait);

        }
        else
            Ait++;
    }

    //Asteroid to Asteroid
    for (vector<Asteroid*>::iterator A1it = Vasteroid.begin(); A1it != Vasteroid.end();){

        int ast1Health = (*A1it)->health;

        for (vector<Asteroid*>::iterator A2it = Vasteroid.begin(); A2it != Vasteroid.end();){

            if ((*A1it)->hitbox.AABBtoAABB((*A2it)->hitbox) && (*A1it) != (*A2it)){
                     
                (*A1it)->health -= (*A2it)->health;
                (*A2it)->health -= ast1Health;

                if ((*A2it)->health > 0){
                    if ((*A2it)->health / 10 < 5){

                        (*A2it)->size = 5;

                    }
                    else{

                        (*A2it)->size = (*A2it)->health / 10;

                    }
                }
                else if ((*A1it)->health > 0){

                    if ((*A1it)->health / 10 < 5){

                        (*A1it)->size = 5;

                    }
                    else{

                        (*A1it)->size = (*A1it)->health / 10;

                    }
                }

            }


            if ((*A2it)->health <= 0){

				waveFunctions->numberOfAsteroidsDestroyed += 1;
                vehiclesRemoveTarget((*A2it));
                Vector3 ExploCenter = (*A2it)->Pos + (*A1it)->Pos;
                ExploCenter /= 2;
                allExplosions.push_back(new Explosion((*A2it)->size * 2, 50, ExploCenter));
                explosionsfx->play3D("Sound/asteroidboom.mp3", irrklang::vec3df((*A2it)->Pos.x, (*A2it)->Pos.y, (*A2it)->Pos.z));
                delete (*A2it);
                A2it = Vasteroid.erase(A2it);

            }
            else{


                A2it++;

            }

        }

        if ((*A1it)->health <= 0){

			waveFunctions->numberOfAsteroidsDestroyed += 1;
            vehiclesRemoveTarget((*A1it));
            allExplosions.push_back(new Explosion((*A1it)->size * 2, 50, (*A1it)->Pos));
            explosionsfx->play3D("Sound/asteroidboom.mp3", irrklang::vec3df((*A1it)->Pos.x, (*A1it)->Pos.y, (*A1it)->Pos.z));
            delete  (*A1it);
            A1it = Vasteroid.erase(A1it);

        }
        else{

            A1it++;

        }
    
    
    }


}


//Others

void SP2::MouseSelection(double dt)
{
	if (place == nullptr)
	{
		if (Application::IsKeyPressed(VK_LBUTTON))
		{
			if (hold == true)
			{
				endCursor = picker.WorldCoord();
				for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
				{
					vector<Vehicles*>::iterator it = allVehicles[i].begin();
					while (it != allVehicles[i].end())
					{
						Vehicles* Vtemp = *it;
						if (Vtemp->interaction.withinPlane(initCursor, endCursor))
						{
							selection.push_back(Vtemp);
						}
						it++;
					}
				}

			}
			else
			{
                selection.clear();
				initCursor = picker.WorldCoord();
				endCursor = picker.WorldCoord();
				bool Bselected = false;
				for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i)
				{
					vector<Vehicles*>::iterator it = allVehicles[i].begin();
					while (it != allVehicles[i].end())
					{
						Vehicles* Vtemp = *it;
						if (Vtemp->interaction.RayToAABB(camera.position, picker.getCurrentRay()))
						{
							if (!selection.empty())
								selection.clear();
							selection.push_back(Vtemp);
							Bselected = true;
							break;
						}
						else
						{
							selection.clear();
                            it++;
						}
					
					}

					if (Bselected == true)
					{
						break;
					}
				}
				hold = true;
			}
		}
		else
		{
			initCursor.SetZero();
			endCursor.SetZero();
			hold = false;
		}
	}
	else
	{
		if (Application::IsKeyPressed(VK_LBUTTON))
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
			selection.clear();
			selection.push_back(place);
			place = nullptr;
			storedVehicles[placeType].pop();
			hold = false;
		}
		else if (Application::IsKeyPressed(VK_RBUTTON))
		{
			place = nullptr;
		}
	}

	if (Application::IsKeyPressed(VK_RBUTTON) && !selection.empty())
	{

        bool setTarget = false;

		for (vector<Asteroid*>::iterator vitA = Vasteroid.begin(); vitA != Vasteroid.end();){

			Asteroid* temp = *vitA;

			if (temp->hitbox.RayToAABB(camera.position, picker.getCurrentRay())){

                setTarget = true;
                selectionSetTarget(temp);
				break;

			}
			else{

				vitA++;

			}

		}

        if (setTarget == false){

            selectionSetTarget(nullptr);
            selectionSetWaypoints(picker.WorldCoord());

		}


	}
	
}

void SP2::ballquest()
{
	if (currentQuest == ballQuest && allNPC[0]->complete != true)
	{

		RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[0]->questDialogue + allNPC[0]->questStatus, Color(0, 1, 0), 3, 0, 12, 99);

	}

	if (Application::IsKeyPressed('E') && allNPC[0]->hitbox.PointToAABB(camera.position) && allNPC[0]->complete == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[0]->questDialogue + allNPC[0]->questStatus, Color(0, 1, 0), 3, 0, 12, 99);
		RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[0]->questCompleteDialogue, Color(0, 1, 0), 3, 0, 9, 99);
		RenderTextOnScreen(meshList[GEO_TEXT2], "+400 maniez", Color(0, 1, 0), 3, 0, 6, 99);
	}

	if (pickup == false && allNPC[0]->complete == false)
	{
		if (ball.hitbox.PointToAABB(camera.position))
		{
			renderTextonball();
		}
		modelStack.PushMatrix();
		modelStack.Translate(ball.Pos.x, ball.Pos.y, ball.Pos.z );
		modelStack.Scale(0.1, 0.1, 0.1);
		RenderMesh(meshList[GEO_SPHERE], enableLight);
		modelStack.PopMatrix();
	}
	else if (allNPC[0]->complete == false)
	{
		RenderOnScreen(meshList[GEO_SPHERE], 70, 10, 0, 5, 0, 0, 0);
	}
	
	if (allNPC[0]->complete == true && allNPC[0]->reward == false)
	{
		currMoney += 400;
		allNPC[0]->reward = true;
	}
}

void SP2::asteroidquest()
{
	if (currentQuest == asteroidQuest && allNPC[1]->complete != true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[1]->questDialogue + allNPC[1]->questStatus, Color(0, 1, 0), 3, 0, 12, 100);

	}

	if (Application::IsKeyPressed('E') && allNPC[1]->hitbox.PointToAABB(camera.position) && allNPC[1]->complete == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[1]->questDialogue + allNPC[1]->questStatus, Color(0, 1, 0), 3, 0, 12, 99);
		RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[1]->questCompleteDialogue, Color(0, 1, 0), 3, 0, 9, 99);
		RenderTextOnScreen(meshList[GEO_TEXT2], "+400 maniez", Color(0, 1, 0), 3, 0, 6, 99);
	}

	if (allNPC[1]->complete == true && allNPC[1]->reward == false)
	{
		currMoney += 400;
		allNPC[1]->reward = true;
	}
	
}

void SP2::buyshipquest()
{
	if (currentQuest == buyshipQuest && allNPC[2]->complete != true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[2]->questDialogue + allNPC[2]->questStatus, Color(0, 1, 0), 3, 0, 12, 99);

	}

	if (Application::IsKeyPressed('E') && allNPC[2]->hitbox.PointToAABB(camera.position) && allNPC[2]->complete == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[2]->questDialogue + allNPC[2]->questStatus, Color(0, 1, 0), 3, 0, 12, 99);
		RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[2]->questCompleteDialogue, Color(0, 1, 0), 3, 0, 9, 99);
		RenderTextOnScreen(meshList[GEO_TEXT2], "+400 maniez", Color(0, 1, 0), 3, 0, 6, 99);
	}

	if (allNPC[2]->complete == true && allNPC[2]->reward == false)
	{
		currMoney += 400;
		allNPC[2]->reward = true;
	}
	
}

void SP2::abductionquest()
{

	if (currentQuest == abductionQuest && allNPC[3]->complete != true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[3]->questDialogue + allNPC[3]->questStatus, Color(0, 1, 0), 3, 0, 12, 99);

	}

	if (NPC5.hitbox.PointToAABB(camera.position) && Application::IsKeyPressed('E') && allNPC[3]->complete == false)
	{	
		kidnap = true;
	}

	if (NPC5.hitbox.PointToAABB(camera.position) && kidnap == false)
	{
		renderchildtext();
	}

	if (Application::IsKeyPressed('E') && allNPC[3]->hitbox.PointToAABB(camera.position) && allNPC[3]->complete == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[3]->questDialogue + allNPC[3]->questStatus, Color(0, 1, 0), 3, 0, 12, 99);
		RenderTextOnScreen(meshList[GEO_TEXT2], allNPC[3]->questCompleteDialogue, Color(0, 1, 0), 3, 0, 9, 99);
		RenderTextOnScreen(meshList[GEO_TEXT2], "+400 maniez", Color(0, 1, 0), 3, 0, 6, 99);
	}

	if (allNPC[3]->complete == true && allNPC[3]->reward == false)
	{
		currMoney += 400;
		allNPC[3]->reward = true;
	}

}

void SP2::selectionSetTarget(Asteroid* newTarget){

    vector<Vehicles*>::iterator vitV = selection.begin();

    while (vitV != selection.end()){

        Vehicles* tempV = *vitV;
        tempV->currAttackTarget = newTarget;
        vitV++;

    }

   
}

void SP2::selectionRemovetarget(){

    for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i){

        vector<Vehicles*>::iterator vitV = allVehicles[i].begin();

        while (vitV != allVehicles[i].end()){

            Vehicles* tempV = *vitV;
            tempV->currAttackTarget = nullptr;
            vitV++;

        }

    }
   
}

void SP2::vehiclesRemoveTarget(Asteroid* oldTarget){

    for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i){

        vector<Vehicles*>::iterator vitV = allVehicles[i].begin();

        while (vitV != allVehicles[i].end()){

            if ((*vitV)->currAttackTarget == oldTarget){

                (*vitV)->currAttackTarget = nullptr;
                
            }

            vitV++;

        }

    }

}

void SP2::selectionSetWaypoints(Vector3 newPosition){

   vector<Vehicles*>::iterator vitV = selection.begin();
  
   while (vitV != selection.end()){

       (*vitV)->setNewWayPoint(newPosition.x, newPosition.z);
       vitV++;

   }


}

void SP2::removeOneSelection(Vehicles* removedVehicle){

    for (int i = GEO_SMALLSHIP; i <= GEO_LARGESHIP; ++i){

        vector<Vehicles*>::iterator vitV = selection.begin();

        while (vitV != selection.end()){

            Vehicles* tempV = *vitV;

            if (tempV == removedVehicle){

                vitV = selection.erase(vitV);

            }
            else{

                vitV++;
            }

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