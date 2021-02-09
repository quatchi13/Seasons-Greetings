#include "PhysicsPlayground.h"
#include "PhysicsPlaygroundListener.h"
#include "Utilities.h"
#include "Vector.h"
#include "PlayerHealth.h"
#include <vector>
#include <random>


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
		//ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		ECS::GetComponent<Camera>(entity).SetPosition(0, 40, 50);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		//ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
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
		//ECS::AttachComponent<Player>(entity);
		//ECS::AttachComponent<AnimationController>(entity);

		//Sets up the components
		std::string fileName = "boxSprite.jpg";
		/*std::string animations = "Playerpain.json";*/
		/*ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 25, 25, &ECS::GetComponent<Sprite>(entity),
			&ECS::GetComponent<AnimationController>(entity), &ECS::GetComponent<Transform>(entity));*/
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(15.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(-2.f, 0.f), false, PLAYER, ENEMY | TRIGGER | EBULLET | ENVIRONMENT, 0.f, 3.f);
		//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		//std::vector<b2Vec2> points = { b2Vec2(-tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(0, tempSpr.GetHeight() / 2.f) };
		//tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.5);
		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(40.f);
	}



	makeImage("Background.png", 25000, 150, 1, 12000, 40, -4);

	for (int i = 0; i < 49; i++) {
		makeWall(i);
	}

	std::cout << "beep";

	for (int i = 0; i < 38; i++) {
		makeGround(i);
	}
	std::cout << "boop";

	{
		int a = 0;
		int wCount = 0;
		int gCount = 0;
		b2Vec2 pos = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetPosition();
		
		for (int i = 3; i > -4; i--) {
			for (int j = 4; j > -5; j--) {
				
				int enemy;
				if (a == 14)
				{
					enemy = makeEnemy();
					ECS::GetComponent<PhysicsBody>(enemy).SetPosition(b2Vec2(pos.x + (j * 10), pos.y + (i * 10)));
				}
				if (floor[a] == 0) {
					block = walls[wCount];
					wCount++;
				}
				else {
					block = groundTiles[gCount];
					gCount++;
				}

				ECS::GetComponent<PhysicsBody>(block).SetPosition(b2Vec2(pos.x + (j * 10), pos.y + (i * 10)));

				a++;
				std::cout << a << '\n';
				std::cout << (ECS::GetComponent<PhysicsBody>(block).GetBody()->GetPosition().x) << ' '
					<< (ECS::GetComponent<PhysicsBody>(block).GetBody()->GetPosition().x) << '\n';
			}
		}
	}
	

	

	for (int i = 0; i < 5; i++)
	{
		auto entity = ECS::CreateEntity();
		bulletHold.push_back(entity);

		makeBullet(i + 1);
	}

	makeSword();

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	//ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void PhysicsPlayground::Update()
{
	//animations
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	/*auto& pain = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	pain.Update();*/



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

	//melee attack
	auto& swordBody = ECS::GetComponent<PhysicsBody>(melee);
	auto& swordStuff = ECS::GetComponent<Melee>(melee);
	bool swordHasHit = ECS::GetComponent<Melee>(melee).m_hasHit;
	if (swordStuff.m_isActive) {
		if (swordHasHit) {
			swordStuff.m_isActive = false;
			swordStuff.m_curDeg = 0;
			swordBody.SetVelocity(vec3(0, 0, 0));
			swordBody.SetPosition(b2Vec2(100, 300));
		}
		else {
			swordBody.SetRotationAngleDeg(swordBody.GetRotationAngleDeg() - 9);
			swordStuff.m_curDeg += 9;

			if (swordStuff.m_curDeg == 90) {
				swordStuff.m_isActive = false;
				swordStuff.m_curDeg = 0;
				swordBody.SetVelocity(vec3(0, 0, 0));
				swordBody.SetPosition(b2Vec2(100, 300));
			}
		}

	}

	auto& phealth = ECS::GetComponent<PlayerHealth>(MainEntities::MainPlayer());
	if (phealth.hasBeenDamaged)
	{
		phealth.health--;
		phealth.hasBeenDamaged = false;
		phealth.CheckPlayerStatus();
	}

}

PhysicsPlaygroundListener timer;
void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& playerAiming = ECS::GetComponent<PlayerAim>(MainEntities::MainPlayer());
	auto& iceBlock = ECS::GetComponent<IceBlock>(MainEntities::MainPlayer());

	float speed = 0.6f;
	int velX = 0;
	int velY = 0;
	if (ECS::GetComponent<Melee>(melee).m_isActive == false && iceBlock.m_isActive == false) {
		
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

void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& attacking = ECS::GetComponent<PlayerAim>(MainEntities::MainPlayer());

	if (attacking.meleeAttackOn) {
		if (Input::GetKeyDown(Key::UpArrow))
		{
			attacking.m_dirFacing = 'W';
			slash();

		}else if (Input::GetKeyDown(Key::LeftArrow)){

			attacking.m_dirFacing = 'A';
			slash();

		}else if (Input::GetKeyDown(Key::DownArrow)){
			
			attacking.m_dirFacing = 'S';
			slash();

		}else if (Input::GetKeyDown(Key::RightArrow)){
			
			attacking.m_dirFacing = 'D';
			slash();

		}else if (Input::GetKeyDown(Key::Space)) {
			attacking.meleeAttackOn = false;
		}
	}
	else {
		if (Input::GetKeyDown(Key::UpArrow))
		{
			attacking.m_dirFacing = 'W';
			fireBullet();

		}
		else if (Input::GetKeyDown(Key::LeftArrow)) {

			attacking.m_dirFacing = 'A';
			fireBullet();

		}
		else if (Input::GetKeyDown(Key::DownArrow)) {

			attacking.m_dirFacing = 'S';
			fireBullet();

		}
		else if (Input::GetKeyDown(Key::RightArrow)) {

			attacking.m_dirFacing = 'D';
			fireBullet();

		}
		else if (Input::GetKeyDown(Key::Space)) {
			attacking.meleeAttackOn = true;
		}
	}


	if (Input::GetKeyDown(Key::J))
	{
		slash();
	}

	if (Input::GetKeyDown(Key::K))
	{
		fireBullet();
	}

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
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
	if (filename == "End.png") {
		endCard = entity;
	}
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);

	//Set up the components
	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, width, height);
	ECS::GetComponent<Sprite>(entity).SetTransparency(opacity);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(x, y, z));
}

void PhysicsPlayground::makeWall(int index)
{
	std::string filename = "wall.png";
	//stores wall entity in vector
	auto entity = ECS::CreateEntity();
	walls.push_back(entity);

	//attaches components to wall entity
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 10, 10);
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

int PhysicsPlayground::makeEnemy()
{
	
	std::string filename = "wall.png";
	//stores wall entity in vector
	auto entity = ECS::CreateEntity();
	int reference = entity;

	//attaches components to wall entity
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Damage>(entity);

	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 10, 10);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30, -30, 2));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(0), float32(35));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
		float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, ENEMY, PLAYER | PWEAPON | BULLET);
	tempPhsBody.SetColor(vec4(0, 1, 0, 0.3));
	return reference;
}

void PhysicsPlayground::makeGround(int index)
{
	std::string filename = "ground.png";
	//stores wall entity in vector
	auto entity = ECS::CreateEntity();
	groundTiles.push_back(entity);

	//attaches components to wall entity
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 10, 10);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30, -30, 1));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(500), float32(500));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
		float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, GROUND, BOUNDARY);
	tempPhsBody.SetColor(vec4(0, 1, 0, 0.3));

}

//void PhysicsPlayground::makeShotTrigger(int index)
//{
//	//Creates entity
//	auto entity = ECS::CreateEntity();
//
//	shotTriggers.push_back(entity);
//
//	if (index == 0) {
//		shotTriggers.erase(shotTriggers.begin());
//	}
//
//
//	//Add components
//	ECS::AttachComponent<Sprite>(entity);
//	ECS::AttachComponent<Transform>(entity);
//	ECS::AttachComponent<PhysicsBody>(entity);
//	ECS::AttachComponent<Trigger*>(entity);
//
//	//Sets up components
//	//space
//	std::string fileName = "nothingness.png";
//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1, 200);
//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30, -20, 80));
//	ECS::GetComponent<Trigger*>(entity) = new EnemyShotTrigger();
//	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(MainEntities::MainPlayer());
//	ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(hostileBullets[index]);
//
//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
//
//
//	b2Body* tempBody;
//	b2BodyDef tempDef;
//	tempDef.type = b2_staticBody;
//	tempDef.position.Set(float32(positionArray[index][0] - 120), float32(-30));
//
//	tempBody = m_physicsWorld->CreateBody(&tempDef);
//
//	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()), vec2(0.f, 0.f), true, TRIGGER, PLAYER);
//	tempPhsBody.SetColor(vec4(1, 0, 0, 0.3));
//}



void PhysicsPlayground::makeBullet(int index) {

	std::string fileName = "Bullet.png";
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

	tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight()) / 2.f), vec2(0.f, 0.f), false, BULLET, ENEMY | BOUNDARY | FBOUNDARY);
	tempPhsBody.SetColor(vec4(1, 0, 0, 0.3));
	tempPhsBody.SetRotationAngleDeg(0);
	tempPhsBody.SetFixedRotation(true);
	tempPhsBody.SetGravityScale(0.f);

}

void PhysicsPlayground::makeSword() {
	auto entity = ECS::CreateEntity();
	melee = entity;

	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Melee>(entity);
	ECS::AttachComponent<IsInactive>(entity);

	//Sets up components
	std::string fileName = "BoxSprite.jpg";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 16, 2);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30, -30, 2));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(100), float32(300));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
		float(tempSpr.GetHeight()), vec2(0.f, 0.f), false, BULLET, ENEMY | BOUNDARY);
	tempPhsBody.SetColor(vec4(0, 1, 0, 0.3));
	tempPhsBody.SetGravityScale(0);
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

void PhysicsPlayground::slash() {
	auto& swordBody = ECS::GetComponent<PhysicsBody>(melee);
	auto& swordStuff = ECS::GetComponent<Melee>(melee);
	char attackDir = ECS::GetComponent<PlayerAim>(MainEntities::MainPlayer()).m_dirFacing;
	b2Vec2 location = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetPosition();
	vec3 velocity = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetVelocity();

	if (!swordStuff.m_isActive) {
		switch (attackDir) {
		case('W'):
			location.y += 5;
			swordStuff.startDeg = 135;
			break;
		case('A'):
			location.x -= 5;
			swordStuff.startDeg = 225;
			break;
		case('S'):
			location.y -= 5;
			swordStuff.startDeg = 305;
			break;
		default:
			location.x += 5;
			swordStuff.startDeg = 45;
		}

		/*if (isRight) {
			location.x += 8;
			swordStuff.m_attackRight = true;
		}
		else {
			location.x -= 8;
			swordStuff.m_attackRight = false;
		}*/

		swordBody.SetPosition(location);
		swordBody.SetRotationAngleDeg(swordStuff.startDeg);
		swordStuff.m_isActive = true;
		swordStuff.m_hasHit = false;
	}

}
