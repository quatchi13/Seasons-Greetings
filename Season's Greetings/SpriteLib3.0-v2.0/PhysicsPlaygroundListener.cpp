#include "PhysicsPlaygroundListener.h"
#include "ECS.h"


PhysicsPlaygroundListener::PhysicsPlaygroundListener()
	: b2ContactListener()
{

}

void PhysicsPlaygroundListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();


	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerEnter(fixtureA);
		}
		else if (sensorB)
		{
			TriggerEnter(fixtureB);
		}
	}

	b2Filter filterA = fixtureA->GetFilterData();
	b2Filter filterB = fixtureB->GetFilterData();


	if ((filterA.categoryBits == ENEMY && filterB.categoryBits == PWEAPON) || (filterB.categoryBits == ENEMY && filterA.categoryBits == PWEAPON))
	{
		if (filterA.categoryBits == ENEMY)
		{
			//ECS::DestroyEntity((int)fixtureA->GetBody()->GetUserData());
			ECS::GetComponent<Enemy>((int)fixtureA->GetBody()->GetUserData()).ded = true;
			ECS::GetComponent<Melee>((int)fixtureB->GetBody()->GetUserData()).m_hasHit = true;
		}
		else if (filterB.categoryBits == ENEMY)
		{
			ECS::GetComponent<Enemy>((int)fixtureB->GetBody()->GetUserData()).ded = true;
			ECS::GetComponent<Melee>((int)fixtureA->GetBody()->GetUserData()).m_hasHit = true;
		}
	}

	if ((filterA.categoryBits == ENEMY && filterB.categoryBits == BULLET) || (filterB.categoryBits == ENEMY && filterA.categoryBits == BULLET))
	{
		if (filterA.categoryBits == ENEMY)
		{
			//ECS::DestroyEntity((int)fixtureA->GetBody()->GetUserData());
			ECS::GetComponent<Enemy>((int)fixtureA->GetBody()->GetUserData()).ded = true;
			ECS::GetComponent<IsInactive>((int)fixtureB->GetBody()->GetUserData()).m_notInUse = true;
		}
		else if (filterB.categoryBits == ENEMY)
		{
			ECS::GetComponent<Enemy>((int)fixtureB->GetBody()->GetUserData()).ded = true;
			ECS::GetComponent<IsInactive>((int)fixtureA->GetBody()->GetUserData()).m_notInUse = true;
		}
	}

	if ((filterA.categoryBits == ENEMY && filterB.categoryBits == PLAYER) || (filterB.categoryBits == ENEMY && filterA.categoryBits == PLAYER))
	{
		if (filterA.categoryBits == ENEMY)
		{

			ECS::GetComponent<Enemy>((int)fixtureA->GetBody()->GetUserData()).collided = true;

			if (ECS::GetComponent<IceBlock>((int)fixtureB->GetBody()->GetUserData()).m_isActive) {
				std::cout << "PARRIED, YOU FUCKING CASUAL!";
			}
			else
			{
				ECS::GetComponent<PlayerHealth>((int)fixtureB->GetBody()->GetUserData()).hasBeenDamaged = true;
			}
		}
		else if (filterB.categoryBits == ENEMY)
		{

			ECS::GetComponent<Enemy>((int)fixtureB->GetBody()->GetUserData()).collided = true;

			if (ECS::GetComponent<IceBlock>((int)fixtureA->GetBody()->GetUserData()).m_isActive) {
				std::cout << "PARRIED, YOU FUCKING CASUAL!";
			}
			else
			{
				ECS::GetComponent<PlayerHealth>((int)fixtureA->GetBody()->GetUserData()).hasBeenDamaged = true;
			}
			
		}
	}

	if ((filterA.categoryBits == ENVIRONMENT && filterB.categoryBits == PLAYER) || (filterB.categoryBits == ENVIRONMENT && filterA.categoryBits == PLAYER))
	{
		if (filterA.categoryBits == ENVIRONMENT)
		{
			std::string fileName = "vines.png";
			if (fileName == (ECS::GetComponent<Sprite>((int)fixtureA).GetFileName())) {
				if (ECS::GetComponent<IceBlock>((int)fixtureB->GetBody()->GetUserData()).m_isActive) {
					std::cout << "PARRIED, YOU FUCKING CASUAL!";
				}
				else
				{
					ECS::GetComponent<PlayerHealth>((int)fixtureB->GetBody()->GetUserData()).hasBeenDamaged = true;
				}
			}
			
		}
		else if (filterB.categoryBits == ENVIRONMENT)
		{
			if(ECS::GetComponent<Sprite>((int)fixtureB->GetBody()->GetUserData()).GetFileName() == "vines.png") {

				if (ECS::GetComponent<IceBlock>((int)fixtureA->GetBody()->GetUserData()).m_isActive) {
					std::cout << "PARRIED, YOU FUCKING CASUAL!";
				}
				else
				{
					ECS::GetComponent<PlayerHealth>((int)fixtureA->GetBody()->GetUserData()).hasBeenDamaged = true;
				}
			}
		}
	}


	if ((filterA.categoryBits == ENEMY && filterB.categoryBits == ENVIRONMENT) || (filterB.categoryBits == ENEMY && filterA.categoryBits == ENVIRONMENT))
	{
		if (filterA.categoryBits == ENEMY)
		{
			ECS::GetComponent<Enemy>((int)fixtureA->GetBody()->GetUserData()).collided = true;
		}
		else if (filterB.categoryBits == ENEMY)
		{
			ECS::GetComponent<Enemy>((int)fixtureB->GetBody()->GetUserData()).collided = true;
		}
	}

	if ((filterA.categoryBits == ENEMY && filterB.categoryBits == GROUND) || (filterB.categoryBits == ENEMY && filterA.categoryBits == GROUND))
	{
		if (filterA.categoryBits == ENEMY)
		{
			ECS::GetComponent<Enemy>((int)fixtureA->GetBody()->GetUserData()).collided = true;
		}
		else if (filterB.categoryBits == ENEMY)
		{
			ECS::GetComponent<Enemy>((int)fixtureB->GetBody()->GetUserData()).collided = true;
		}
	}

	if ((filterA.categoryBits == DOOR && filterB.categoryBits == PLAYER) || (filterB.categoryBits == DOOR && filterA.categoryBits == PLAYER))
	{
		if (filterA.categoryBits == DOOR)
		{
			ECS::GetComponent<Door>((int)fixtureA->GetBody()->GetUserData()).activated = true;
		}
		else if (filterB.categoryBits == DOOR)
		{
			ECS::GetComponent<Door>((int)fixtureB->GetBody()->GetUserData()).activated = true;
		
		}
	}

	/*if ((filterA.categoryBits == BULLET && filterB.categoryBits == BOUNDARY) || (filterB.categoryBits == BULLET && filterA.categoryBits == BOUNDARY))
	{
		if (filterA.categoryBits == BULLET)
		{
			ECS::GetComponent<IsInactive>((int)fixtureA->GetBody()->GetUserData()).m_notInUse = true;

		}
		else if (filterB.categoryBits == BULLET)
		{
			ECS::GetComponent<IsInactive>((int)fixtureB->GetBody()->GetUserData()).m_notInUse = true;
		}
	}*/

	if ((filterA.categoryBits == BULLET && filterB.categoryBits == GROUND) || (filterB.categoryBits == BULLET && filterA.categoryBits == GROUND))
	{
		if (filterA.categoryBits == BULLET)
		{
			ECS::GetComponent<IsInactive>((int)fixtureA->GetBody()->GetUserData()).m_notInUse = true;

		}
		else if (filterB.categoryBits == BULLET)
		{
			ECS::GetComponent<IsInactive>((int)fixtureB->GetBody()->GetUserData()).m_notInUse = true;
		}
	}

	if ((filterA.categoryBits == EBULLET && filterB.categoryBits == GROUND) || (filterB.categoryBits == EBULLET && filterA.categoryBits == GROUND))
	{
		if (filterA.categoryBits == EBULLET)
		{
			ECS::GetComponent<IsInactive>((int)fixtureA->GetBody()->GetUserData()).hit = true;

		}
		else if (filterB.categoryBits == EBULLET)
		{
			ECS::GetComponent<IsInactive>((int)fixtureB->GetBody()->GetUserData()).hit = true;

		}
	}

	if ((filterA.categoryBits == EBULLET && filterB.categoryBits == PLAYER) || (filterB.categoryBits == EBULLET && filterA.categoryBits == PLAYER))
	{
		if (filterA.categoryBits == EBULLET)
		{
			ECS::GetComponent<IsInactive>((int)fixtureA->GetBody()->GetUserData()).hit = true;
			if (ECS::GetComponent<IceBlock>((int)fixtureB->GetBody()->GetUserData()).m_isActive)  {
				std::cout << "PARRIED, YOU FUCKING CASUAL!";
			}
			else {
				ECS::GetComponent<PlayerHealth>((int)fixtureA->GetBody()->GetUserData()).hasBeenDamaged = true;
			}
		}
		else if (filterB.categoryBits == EBULLET)
		{
			ECS::GetComponent<IsInactive>((int)fixtureB->GetBody()->GetUserData()).hit = true;
			if (ECS::GetComponent<IceBlock>((int)fixtureA->GetBody()->GetUserData()).m_isActive) {
				std::cout << "PARRIED, YOU FUCKING CASUAL!";
			}
			else {
				ECS::GetComponent<PlayerHealth>((int)fixtureA->GetBody()->GetUserData()).hasBeenDamaged = true;
			}
		}
	}

}

void PhysicsPlaygroundListener::EndContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();

	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerExit(fixtureA);
		}
		else if (sensorB)
		{
			TriggerExit(fixtureB);
		}
	}

	b2Filter filterA = fixtureA->GetFilterData();
	b2Filter filterB = fixtureB->GetFilterData();

	
}

void PhysicsPlaygroundListener::SetTimer(int start)
{
	timer = start;
}

void PhysicsPlaygroundListener::AddTime(float add)
{
	timer += add;
}

float PhysicsPlaygroundListener::GetTimer()
{
	return timer;
}

void PhysicsPlaygroundListener::TriggerEnter(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnEnter();
}

void PhysicsPlaygroundListener::TriggerExit(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnExit();
}
