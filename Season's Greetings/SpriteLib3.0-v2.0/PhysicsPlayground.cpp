#include "PhysicsPlayground.h"
#include "PhysicsPlaygroundListener.h"
#include "Utilities.h"
#include "Vector.h"
#include "PlayerHealth.h"
#include "Dungeon.h";
#include <vector>
#include <random>
#include <time.h>
#include <fstream>


PhysicsPlayground::PhysicsPlayground()
{
}

PhysicsPlayground::PhysicsPlayground(std::string name)
	: Scene(name)
{
	m_gravity = b2Vec2(0.f, 0.f); //0.f, -98.f
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
}

Dungeon dungeon;

void PhysicsPlayground::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-65.f, 65.f, -65.f, 65.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		ECS::GetComponent<Camera>(entity).SetPosition(0, 40, 50);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//player entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);
		ECS::AttachComponent<PlayerAim>(entity);
		ECS::AttachComponent<IceBlock>(entity);
		ECS::AttachComponent<PlayerHealth>(entity);

		//Sets up the components
		ECS::AttachComponent<Player>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up the components
		std::string fileName = "spritesheets/Frost-Sheet.png";
		std::string animations = "Frostbite.json";
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 19.5, 19.5, &ECS::GetComponent<Sprite>(entity),
			&ECS::GetComponent<AnimationController>(entity), &ECS::GetComponent<Transform>(entity));
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 19.5, 19.5);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 5.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 9.5f;
		float shrinkY = 8.5f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(15.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -4.f), false, PLAYER, ENEMY | TRIGGER | EBULLET | ENVIRONMENT | DOOR, 0.f, 3.f);
		//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		//std::vector<b2Vec2> points = { b2Vec2(-tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(0, tempSpr.GetHeight() / 2.f) };
		//tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.5);
		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetGravityScale(40.f);
	}

	
	makeCamFocus();

	makeImage("StartScreen.png", 195, 130, 1, 0, -495, 1);
	makeImage("winScreen.png", 195, 130, 1, 300, -495, 1);
	makeImage("gameOver.png", 195, 130, 1, 800, -495, 1);
	makeImage("Health10.png", 25, 25, 1, 105, 60, 40);
	//makeGround();

	for (int i = 0; i < 49; i++) {
		makeWall(i);
	}

	std::cout << "beep";
	for (int i = 0; i < 16; i++) {
		makeSpike(i);
	}
	
	for (int i = 0; i < 4; i++) {
		makeShooter(i);
		makeEnemy(i);
		makeClockwiseEnemy(i);
		makeChaser(i);
		makeHostileBullet();
	}
	

	for (int i = 0; i < 4; i++)
	{
		auto entity = ECS::CreateEntity();
		bulletHold.push_back(entity);

		makeBullet(i + 1);

		makeDoor(i);
		makeLockedDoor();
	}
	

	newRoom(4, 4);

	makeImage("blackbox.png", 300, 300, 1.f, 0, 10, 30);

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::CameraFocus()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::CameraFocus()));
}

PhysicsPlaygroundListener timer;
void PhysicsPlayground::Update()
{
	if (needToAdd) {
		attackFrames++;
		if (attackFrames == 48) {
			needToAdd = false;
			attackFrames = 0;
		}
	}
	//animations
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& pain = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	pain.Update();

	for (int i = 0; i < hostileBullets.size(); i++) {
		if (ECS::GetComponent<IsInactive>(hostileBullets[i]).hit) {
			ECS::GetComponent<PhysicsBody>(hostileBullets[i]).SetVelocity(vec3(0.f, 0.f, 0.f));
			ECS::GetComponent<PhysicsBody>(hostileBullets[i]).SetPosition(b2Vec2(500, 520));
			ECS::GetComponent<IsInactive>(hostileBullets[i]).hit = false;
			ECS::GetComponent<IsInactive>(hostileBullets[i]).m_notInUse = true;
			std::cout << "beep";
		}
	}

	if (activeEnemies.size()) {
			for (int i = 0; i < activeEnemies.size(); i++) {
				if (ECS::GetComponent<Enemy>(activeEnemies[i]).ded && ECS::GetComponent<PhysicsBody>(activeEnemies[i]).GetPosition().x != 500) {
					std::cout << "oh no he ded \n";
					ECS::GetComponent<PhysicsBody>(activeEnemies[i]).SetPosition(b2Vec2(500, 500));
					ECS::GetComponent<Enemy>(activeEnemies[i]).ded = false;
					activeEnemies.erase(activeEnemies.begin() + i);

					//remove this abomination as soon as possible
					
					//dungeon.enemiesInRooms[dungeon.currentRoom].erase(dungeon.enemiesInRooms[dungeon.currentRoom].begin() + (i+(1+(dungeon.enemiesInRooms[dungeon.currentRoom][0]))));
					//dungeon.enemiesInRooms[dungeon.currentRoom].erase(dungeon.enemiesInRooms[dungeon.currentRoom].begin() + (i + 1));
					//this line is ok though 
					dungeon.enemiesInRooms[dungeon.currentRoom][0]--;
					if (!dungeon.enemiesInRooms[dungeon.currentRoom][0]) {
						for (int i = 0; i < 4; i++) {
							ECS::GetComponent<PhysicsBody>(blockedDoors[i]).SetPosition(b2Vec2(500, 500));
						}

						dungeon.mapCleared = true;
						for (int i = 0; i < dungeon.nOfRooms; i++) {
							if (dungeon.enemiesInRooms[i][0]) {
								dungeon.mapCleared = false;
							}
						}

						if (dungeon.mapCleared) {
							ECS::GetComponent<PhysicsBody>(MainEntities::CameraFocus()).SetPosition(b2Vec2(300, -500));
							hasEnded = true;
						}
					}
				}
			}
		}

	if (activeEnemies.size()) {
		for (int i = 0; i < activeEnemies.size(); i++) {
			if (ECS::GetComponent<Enemy>(activeEnemies[i]).moves) {
				if (ECS::GetComponent<Enemy>(activeEnemies[i]).collided) {
					if (ECS::GetComponent<Enemy>(activeEnemies[i]).movesClockwise)
					{
						ECS::GetComponent<Enemy>(activeEnemies[i]).changeClockwiseDirection();
					}
					else
					{
						ECS::GetComponent<Enemy>(activeEnemies[i]).changeDirection();
					}
					ECS::GetComponent<PhysicsBody>(activeEnemies[i]).SetVelocity(vec3(ECS::GetComponent<Enemy>(activeEnemies[i]).eVelo));
					ECS::GetComponent<Enemy>(activeEnemies[i]).collided = false;
				}
			}
			if (ECS::GetComponent<Enemy>(activeEnemies[i]).chases)
			{
				auto& chaser = ECS::GetComponent<PhysicsBody>(activeEnemies[i]);

				vec3 source((chaser.GetBody()->GetPosition().x), (chaser.GetBody()->GetPosition().y), 3);
				vec3 target((player.GetBody()->GetPosition().x), (player.GetBody()->GetPosition().y), 3);

				b2Vec2 shotVec(target.x - source.x, target.y - source.y);
				b2Vec2 unitVec = shotVec;
				double normal = sqrt(pow(shotVec.x, 2) + pow(shotVec.y, 2));
				unitVec.x /= normal;
				unitVec.y /= normal;

				ECS::GetComponent<Enemy>(activeEnemies[i]).setVel(unitVec.x * 25, unitVec.y * 25, 0);


				ECS::GetComponent<PhysicsBody>(activeEnemies[i]).SetVelocity(vec3(ECS::GetComponent<Enemy>(activeEnemies[i]).eVelo));
				ECS::GetComponent<Enemy>(activeEnemies[i]).collided = false;
			}
			
		}
	}
	

	if (activeEnemies.size()) {
		timer.AddTime(Timer::deltaTime);
		if (timer.GetTimer() > 1.5) {
			int hBIndex = 0;
			for (int i = 0; i < activeEnemies.size(); i++) {
				if (ECS::GetComponent<Enemy>(activeEnemies[i]).shoots) {
					fireEnemyBullet(activeEnemies[i], hostileBullets[hBIndex]);
					hBIndex++;
				}

			}
			timer.SetTimer(0);
		}
	}

	

	if (activeBullets.size() > 1) {
		for (int i = 1; i < activeBullets.size(); i++) {
			bool inactive = ECS::GetComponent<IsInactive>(activeBullets[i]).m_notInUse;

			if (inactive) {
				ECS::GetComponent<PhysicsBody>(activeBullets[i]).SetVelocity(vec3(0.f, 0.f, 0.f));
				ECS::GetComponent<PhysicsBody>(activeBullets[i]).SetPosition(b2Vec2(100, 300));
				bulletHold.push_back(activeBullets[i]);
				activeBullets.erase(activeBullets.begin() + i);
			}
		}
	}


	auto& phealth = ECS::GetComponent<PlayerHealth>(MainEntities::MainPlayer());
	if (phealth.hasBeenDamaged)
	{
		phealth.health--;
		phealth.hasBeenDamaged = false;
		phealth.CheckPlayerStatus();
		if (phealth.deceased) {
			ECS::GetComponent<PhysicsBody>(MainEntities::CameraFocus()).SetPosition(b2Vec2(800, -500));
			for (int i = 0; i < enemies.size(); i++) {
				ECS::GetComponent<PhysicsBody>(enemies[i]).SetVelocity(vec3(0, 0, 0));
			}

			for (int i = 0; i < allTiles.size(); i++) {
				ECS::GetComponent<PhysicsBody>(allTiles[i]).SetPosition(b2Vec2(500, 500));
			}

			if (activeEnemies.size()) {
				activeEnemies.clear();
			}
			hasEnded = true;
		}
		if (phealth.health == 9)
		{
			std::string fileN = "Health9.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileN, 25, 25);
		}
		else if (phealth.health == 8)
		{
			std::string fileN = "Health8.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileN, 25, 25);
		}
		else if (phealth.health == 7)
		{
			std::string fileN = "Health7.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileN, 25, 25);
		}
		else if (phealth.health == 6)
		{
			std::string fileN = "Health6.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileN, 25, 25);
		}
		else if (phealth.health == 5)
		{
			std::string fileN = "Health5.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileN, 25, 25);
		}
		else if (phealth.health == 4)
		{
			std::string fileN = "Health4.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileN, 25, 25);
		}
		else if (phealth.health == 3)
		{
			std::string fileN = "Health3.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileN, 25, 25);
		}
		else if (phealth.health == 2)
		{
			std::string fileN = "Health2.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileN, 25, 25);
		}
		else if (phealth.health == 1)
		{
			std::string fileN = "Health1.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileN, 25, 25);
		}
	}

	for (int i = 0; i < 4; i++) {
		if (ECS::GetComponent<Door>(doors[i]).activated) {
			ECS::GetComponent<Door>(doors[i]).activated = false;
			dungeon.position[0] += ECS::GetComponent<Door>(doors[i]).yOffSet;
			dungeon.position[1] += ECS::GetComponent<Door>(doors[i]).xOffSet;
			
			dungeon.currentRoom = dungeon.map[dungeon.position[0]][dungeon.position[1]];
			dungeon.currentRoom--;
			std::cout << dungeon.currentRoom;
			newRoom(dungeon.currentRoom, i);
		}
	}


}


void PhysicsPlayground::KeyboardHold()
{
	if (hasStarted) {
		auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
		auto& playerAiming = ECS::GetComponent<PlayerAim>(MainEntities::MainPlayer());
		auto& iceBlock = ECS::GetComponent<IceBlock>(MainEntities::MainPlayer());

		float speed = 0.6f;
		int velX = 0;
		int velY = 0;
		if (iceBlock.m_isActive == false) {
		
			if (Input::GetKey(Key::Shift)) {
			iceBlock.m_isActive = true;
		}
		else {
		
			if (Input::GetKey(Key::A))
			{
				velX = -70;
			}
			else if (Input::GetKey(Key::D))
			{
				velX = 70;
			}
		
		
			if (Input::GetKey(Key::W))
			{
				velY = 70;
			}
			else if (Input::GetKey(Key::S))
			{
				velY = -70;
			}

			iceBlock.m_isActive = false;
			
			}	
		}
		else if(iceBlock.m_isActive){
			if (Input::GetKey(Key::Shift)) {
				iceBlock.m_isActive = true;
			}
			else {
				iceBlock.m_isActive = false;
			}
		}

		player.SetVelocity(vec3(velX, velY, 0));
	}
	
}

void PhysicsPlayground::KeyboardDown()
{
	if (hasStarted) {
		auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
		auto& attacking = ECS::GetComponent<PlayerAim>(MainEntities::MainPlayer());
		if (needToAdd == false && !ECS::GetComponent<IceBlock>(MainEntities::MainPlayer()).m_isActive) {
			if (Input::GetKeyDown(Key::UpArrow))
			{
				attacking.m_dirFacing = 'W';
				fireBullet();
				needToAdd = true;

			}
			else if (Input::GetKeyDown(Key::LeftArrow)) {

				attacking.m_dirFacing = 'A';
				fireBullet();
				needToAdd = true;

			}
			else if (Input::GetKeyDown(Key::DownArrow)) {

				attacking.m_dirFacing = 'S';
				fireBullet();
				needToAdd = true;

			}
			else if (Input::GetKeyDown(Key::RightArrow)) {

				attacking.m_dirFacing = 'D';
				fireBullet();
				needToAdd = true;

			}

		}
	

		

		if (Input::GetKeyDown(Key::T))
		{
			PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
		}
	
		if (Input::GetKeyDown(Key::F))
		{
			std::cout << player.GetBody()->GetPosition().x << " " << player.GetBody()->GetPosition().y << "\n";
		}
	}
	else if(hasEnded) {
		if (Input::GetKeyDown(Key::Enter))
		{
			exit(1);
		}
	}
	else {
		if (Input::GetKeyDown(Key::Enter))
		{
			ECS::GetComponent<PhysicsBody>(MainEntities::CameraFocus()).SetPosition(b2Vec2(0, 15));
			hasStarted = true;
		}
	}
	

}

void PhysicsPlayground::KeyboardUp()
{


}




void PhysicsPlayground::makeImage(std::string filename, int width, int height, float opacity, float x, float y, float z)
{
	/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
	auto entity = ECS::CreateEntity();
	if (filename == "Health10.png") {
		healthBar = entity;
	}
	
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);

	//Set up the components
	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, width, height);
	ECS::GetComponent<Sprite>(entity).SetTransparency(opacity);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(x, y, z));
}

void PhysicsPlayground::makeCamFocus()
{
	std::string filename = "nothingness.png";
	//stores wall entity in vector
	auto entity = ECS::CreateEntity();
	ECS::SetIsCameraFocus(entity, true);

	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 20, 20);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30, -30, 1));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(0), float32(-500));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
		float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, GROUND, BOUNDARY);
	tempPhsBody.SetColor(vec4(0, 1, 0, 0.3));

}

void PhysicsPlayground::makeWall(int index)
{
	std::string filename = "wall.png";
	//stores wall entity in vector
	auto entity = ECS::CreateEntity();
	walls.push_back(entity);
	allTiles.push_back(entity);

	//attaches components to wall entity
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 20, 20);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30, -30, 2));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(500), float32(500));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
		float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, ENVIRONMENT, PLAYER | ENEMY | PWEAPON | BULLET | EBULLET);
	tempPhsBody.SetColor(vec4(0, 1, 0, 0.3)); 

}

void PhysicsPlayground::makeLockedDoor()
{
	std::string filename = "pensivetosat.png";
	//stores wall entity in vector
	auto entity = ECS::CreateEntity();
	blockedDoors.push_back(entity);
	allTiles.push_back(entity);

	//attaches components to wall entity
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 20, 20);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30, -30, 2));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(500), float32(500));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
		float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, ENVIRONMENT, PLAYER | ENEMY | PWEAPON | BULLET | EBULLET);
	tempPhsBody.SetColor(vec4(0, 1, 0, 0.3));

}

void PhysicsPlayground::makeSpike(int index)
{
	
	std::string filename = "spikes1.png";
	//stores wall entity in vector
	auto entity = ECS::CreateEntity();
	spikes.push_back(entity);
	allTiles.push_back(entity);

	//attaches components to wall entity
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Damage>(entity);
	ECS::AttachComponent<Enemy>(entity);

	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 20, 20);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30, -30, 2));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(500), float32(500));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
		float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, ENEMY, PLAYER | PWEAPON | BULLET);
	tempPhsBody.SetColor(vec4(0, 1, 0, 0.3));
}

void PhysicsPlayground::makeShooter(int index)
{

	std::string filename = "Bean_shooter.png";
	//stores wall entity in vector
	auto entity = ECS::CreateEntity();
	allTiles.push_back(entity);
	enemies.push_back(entity);

	//attaches components to wall entity
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Damage>(entity);
	ECS::AttachComponent<Enemy>(entity);

	ECS::GetComponent<Enemy>(entity).shoots = true;

	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 20, 20);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30, -30, 2));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(500), float32(500));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
		float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, ENEMY, PLAYER | PWEAPON | BULLET);
	tempPhsBody.SetColor(vec4(0, 1, 0, 0.3));
}

void PhysicsPlayground::makeEnemy(int index)
{

	std::string filename = "LinkStandby.png";
	//stores wall entity in vector
	auto entity = ECS::CreateEntity();
	enemies.push_back(entity);
	allTiles.push_back(entity);

	//attaches components to wall entity
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Damage>(entity);
	ECS::AttachComponent<Enemy>(entity);

	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 16, 16);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30, -30, 2));

	ECS::GetComponent<Enemy>(entity).moves = true;
	ECS::GetComponent<Enemy>(entity).setVel(-50, 1, 0);

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(500), float32(500));
	tempDef.fixedRotation = true;
	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
		float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, ENEMY, PLAYER | PWEAPON | BULLET | ENVIRONMENT, 0, 10);
	tempPhsBody.SetColor(vec4(0, 1, 0, 0.3));
}

void PhysicsPlayground::makeClockwiseEnemy(int index)
{

	std::string filename = "BeachBall.png";
	//stores wall entity in vector
	auto entity = ECS::CreateEntity();
	enemies.push_back(entity);
	allTiles.push_back(entity);

	//attaches components to wall entity
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Damage>(entity);
	ECS::AttachComponent<Enemy>(entity);

	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 16, 16);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30, -30, 2));

	ECS::GetComponent<Enemy>(entity).moves = true;
	ECS::GetComponent<Enemy>(entity).movesClockwise = true;
	ECS::GetComponent<Enemy>(entity).setVel(-50, 0, 0);

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(500), float32(500));
	tempDef.fixedRotation = true;
	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, 8.f, vec2(0.f, 0.f), false, ENEMY, PLAYER | PWEAPON | BULLET | ENVIRONMENT, 0, 10);
	tempPhsBody.SetColor(vec4(0, 1, 0, 0.3));
}

void PhysicsPlayground::makeChaser(int index)
{

	std::string filename = "Spectre1.png";
	//stores wall entity in vector
	auto entity = ECS::CreateEntity();
	enemies.push_back(entity);
	allTiles.push_back(entity);

	//attaches components to wall entity
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Damage>(entity);
	ECS::AttachComponent<Enemy>(entity);

	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 19, 19);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30, -30, 2));

	ECS::GetComponent<Enemy>(entity).chases = true;
	ECS::GetComponent<Enemy>(entity).setVel(0, 0, 0);

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(500), float32(500));
	tempDef.fixedRotation = true;
	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
		float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, ENEMY, PLAYER | PWEAPON | BULLET | ENVIRONMENT, 0, 10);
	tempPhsBody.SetColor(vec4(0, 1, 0, 0.3));
}


void PhysicsPlayground::makeHostileBullet() {
	std::string fileName = "bean.png";

	auto entity = ECS::CreateEntity();
	hostileBullets.push_back(entity);


	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<IsInactive>(entity);

	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 5);
	ECS::GetComponent<Transform>(entity).SetPosition(100, 50, 10);

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);

	tempSpr.SetTransparency(1.f);

	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(500, 520);
	tempDef.fixedRotation = true;

	tempBody = m_physicsWorld->CreateBody(&tempDef);
	tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetWidth()) / 2.f), vec2(0.f, 0.f), false, EBULLET, PLAYER | ENVIRONMENT);
	tempPhsBody.SetColor(vec4(1, 0, 0, 0.3));
	tempPhsBody.SetRotationAngleDeg(0);
	tempPhsBody.SetGravityScale(0.f);

}

void PhysicsPlayground::makeBullet(int index) {

	std::string fileName = "Snow4.png";
	vec3 source(100, 300, 3);
	auto entity = bulletHold[index];

	tempBullet = entity;

	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<IsInactive>(entity);

	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 5);
	ECS::GetComponent<Transform>(entity).SetPosition(source);

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(source.x, source.y);

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight()) / 2.f), vec2(0.f, 0.f), false, BULLET, ENEMY | ENVIRONMENT);
	tempPhsBody.SetColor(vec4(1, 0, 0, 0.3));
	tempPhsBody.SetRotationAngleDeg(0);
	tempPhsBody.SetFixedRotation(true);
	tempPhsBody.SetGravityScale(0.f);

}


void PhysicsPlayground::fireBullet() {
	b2Vec2 source((ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetPosition().x),
		(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetPosition().y));

	int bulletToShoot;
	if (bulletHold.size() > 1)
	{
		activeBullets.push_back(bulletHold[1]);
		bulletHold.erase(bulletHold.begin() + 1);
	}
	else
	{
		bulletToShoot = activeBullets[1];
		activeBullets.push_back(bulletToShoot);
		activeBullets.erase(activeBullets.begin() + 1);
	}
	bulletToShoot = activeBullets.size() - 1;


	std::vector<int> targetCoord;
	ECS::GetComponent<IsInactive>(activeBullets[bulletToShoot]).m_notInUse = false;
	auto& pBullet = ECS::GetComponent<PhysicsBody>(activeBullets[bulletToShoot]);
	auto& pBSprite = ECS::GetComponent<Sprite>(activeBullets[bulletToShoot]);
	pBullet.SetPosition(b2Vec2(source));

	auto& aimingRules = ECS::GetComponent<PlayerAim>(MainEntities::MainPlayer());
	char directionOfBullet = aimingRules.m_dirFacing;

	switch (directionOfBullet) {
		case('W') :
			targetCoord = (aimingRules.aimingCoords[1]);
			source.y += 10;
			pBullet.SetRotationAngleDeg(90);
			break;
		case('A'):
			targetCoord = (aimingRules.aimingCoords[2]);
			source.x -= 10;
			pBullet.SetRotationAngleDeg(180);
			break;
		case('S'):
			targetCoord = (aimingRules.aimingCoords[3]);
			source.y -= 10;
			pBullet.SetRotationAngleDeg(270);
			break;
		default:
			targetCoord = (aimingRules.aimingCoords[0]);
			source.x += 10;
			pBullet.SetRotationAngleDeg(0);
	}

	targetCoord[0] += (ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetPosition().x);
	targetCoord[1] += (ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetPosition().y);

	vec2 startCoord(pBullet.GetBody()->GetPosition().x, pBullet.GetBody()->GetPosition().y);
	b2Vec2 shotVect(targetCoord[0] - startCoord.x, targetCoord[1] - startCoord.y);
	b2Vec2 unitVect = shotVect;
	double normal = sqrt(pow(shotVect.x, 2) + pow(shotVect.y, 2));
	unitVect.x /= normal;
	unitVect.y /= normal;

	vec3 velocity(unitVect.x * 300, unitVect.y * 300, 0);

	pBullet.SetVelocity(velocity);

	//std::cout << "pew!";
}


void PhysicsPlayground::newRoom(int room, int dTel) {
	int a = 0;
	int wCount = 0;
	int sCount = 0;
	int dCount = 0;
	int enemy;
	int eCount;


	for (int i = 0; i < enemies.size(); i++) {
		ECS::GetComponent<PhysicsBody>(enemies[i]).SetVelocity(vec3(0, 0, 0));
	}

	for (int i = 0; i < allTiles.size(); i++) {
		ECS::GetComponent<PhysicsBody>(allTiles[i]).SetPosition(b2Vec2(500, 500));
	}

	std::cout << "printing room " << dungeon.map[dungeon.position[0]][dungeon.position[1]] << '\n';
	
	if (activeEnemies.size()) {
		activeEnemies.clear();
	}

	eCount = dungeon.enemiesInRooms[dungeon.currentRoom][0];
	for (int i = 1; i <= eCount; i++) {
		activeEnemies.push_back(enemies[dungeon.enemiesInRooms[dungeon.currentRoom][i]]);
	}

	for (int i = 3; i > -4; i--) {
		for (int j = -4; j < 5; j++) {

			if (eCount && dungeon.rooms[room][a] == 1) {
				if (a == 4 ) {
					block = blockedDoors[dCount];
					ECS::GetComponent<PhysicsBody>(block).SetPosition(b2Vec2(j * 20, 10 + (i * 20)));
					ECS::GetComponent<PhysicsBody>(block).SetRotationAngleDeg(0);
					dCount++;
				}
				else if (a == 27) {
					block = blockedDoors[dCount];
					ECS::GetComponent<PhysicsBody>(block).SetPosition(b2Vec2(j * 20, 10 + (i * 20)));
					ECS::GetComponent<PhysicsBody>(block).SetRotationAngleDeg(90);
					dCount++;
				}
				else if (a == 58) {
					block = blockedDoors[dCount];
					ECS::GetComponent<PhysicsBody>(block).SetPosition(b2Vec2(j * 20, 10 + (i * 20)));
					ECS::GetComponent<PhysicsBody>(block).SetRotationAngleDeg(180);
					dCount++;
				}
				else if (a == 35) {
					block = blockedDoors[dCount];
					ECS::GetComponent<PhysicsBody>(block).SetPosition(b2Vec2(j * 20, 10 + (i * 20)));
					ECS::GetComponent<PhysicsBody>(block).SetRotationAngleDeg(270);
					dCount++;
				}
			}

			int enemy;
			if (dungeon.rooms[room][a] == 2)
			{
				block = spikes[sCount];
				sCount++;
				ECS::GetComponent<PhysicsBody>(block).SetPosition(b2Vec2(j * 20, 10 + (i * 20)));
			}


			if (dungeon.rooms[room][a] == 0) {
				block = walls[wCount];
				wCount++;
				ECS::GetComponent<PhysicsBody>(block).SetPosition(b2Vec2(j * 20, 10 + (i * 20)));
			}

			
			if (eCount) {
				for (int k = 0; k < eCount; k++) {
					if (a == dungeon.enemiesInRooms[dungeon.currentRoom][k + (1 + eCount)]) {
						enemy = activeEnemies[k];
						ECS::GetComponent<PhysicsBody>(enemy).SetPosition(b2Vec2(j * 20, 10 + (i * 20)));
					}	
				}
			}
					
			a++;
		}
	}

	switch (dTel) {
	case 0:
		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetPosition(b2Vec2(70, 10));
		break;
	case 1:
		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetPosition(b2Vec2(-70, 10));
		break;
	case 2:
		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetPosition(b2Vec2(0, 55));
		break;
	case 3:
		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetPosition(b2Vec2(0, -31));
		break;
	default:
		std::cout << "";
	}


	if (activeEnemies.size()) {
		for (int i = 0; i < activeEnemies.size(); i++) {
			ECS::GetComponent<PhysicsBody>(activeEnemies[i]).SetVelocity(vec3(ECS::GetComponent<Enemy>(activeEnemies[i]).eVelo));
		}
	}
}

void PhysicsPlayground::makeDoor(int index) {
	auto entity = ECS::CreateEntity();
	int x, y, xOff, yOff;
	std::string filename = "nothingness.png";

	switch (index) {
	case 0:
		x = -110;
		y = 10; 
		xOff = -1;
		yOff = 0;
		break;
	case 1:
		x = 110;
		y = 10;
		xOff = 1;
		yOff = 0;
		break;
	case 2:
		x = 0;
		y = -76;
		xOff = 0;
		yOff = 1;
		break;
	default:
		x = 0;
		y = 100;
		xOff = 0;
		yOff = -1;
	}

	doors.push_back(entity);
		
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Door>(entity);
	
	ECS::GetComponent<Door>(entity).yOffSet = yOff; 
	ECS::GetComponent<Door>(entity).xOffSet = xOff;

	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 20, 20);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30, -30, 2));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(x), float32(y));
	
	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
		float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, DOOR, PLAYER | PWEAPON | BULLET);
		tempPhsBody.SetColor(vec4(0, 0, 1, 0.3));
	
}

void PhysicsPlayground::fireEnemyBullet(int shooter, int eBullet) {
	
	ECS::GetComponent<IsInactive>(eBullet).m_notInUse = false;

	auto& enBul = ECS::GetComponent<PhysicsBody>(eBullet);

	enBul.SetPosition(b2Vec2(ECS::GetComponent<PhysicsBody>(shooter).GetBody()->GetPosition().x, 
		ECS::GetComponent<PhysicsBody>(shooter).GetBody()->GetPosition().y));

	enBul.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	//shoot bullet

	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	vec3 source((enBul.GetBody()->GetPosition().x), (enBul.GetBody()->GetPosition().y), 3);
	vec3 target((player.GetBody()->GetPosition().x), (player.GetBody()->GetPosition().y), 3);

	b2Vec2 shotVec(target.x - source.x, target.y - source.y);
	b2Vec2 unitVec = shotVec;
	double normal = sqrt(pow(shotVec.x, 2) + pow(shotVec.y, 2));
	unitVec.x /= normal;
	unitVec.y /= normal;

	vec3 velocity(unitVec.x * 75, unitVec.y * 75, 0);

	enBul.SetVelocity(velocity);
}

