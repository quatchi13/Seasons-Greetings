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
			ECS::GetComponent<IsInactive>((int)fixtureA->GetBody()->GetUserData()).m_ready = true;
			ECS::GetComponent<Melee>((int)fixtureB->GetBody()->GetUserData()).m_hasHit = true;
		}
		else if (filterB.categoryBits == ENEMY)
		{
			ECS::GetComponent<IsInactive>((int)fixtureB->GetBody()->GetUserData()).m_ready = true;
			ECS::GetComponent<Melee>((int)fixtureA->GetBody()->GetUserData()).m_hasHit = true;
		}
	}

	if ((filterA.categoryBits == ENEMY && filterB.categoryBits == BULLET) || (filterB.categoryBits == ENEMY && filterA.categoryBits == BULLET))
	{
		if (filterA.categoryBits == ENEMY)
		{
			//ECS::DestroyEntity((int)fixtureA->GetBody()->GetUserData());
			ECS::GetComponent<IsInactive>((int)fixtureA->GetBody()->GetUserData()).m_ready = true;
			ECS::GetComponent<IsInactive>((int)fixtureB->GetBody()->GetUserData()).m_notInUse = true;
		}
		else if (filterB.categoryBits == ENEMY)
		{
			ECS::GetComponent<IsInactive>((int)fixtureB->GetBody()->GetUserData()).m_ready = true;
			ECS::GetComponent<IsInactive>((int)fixtureA->GetBody()->GetUserData()).m_notInUse = true;
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

	/*if ((filterA.categoryBits == BULLET && filterB.categoryBits == FBOUNDARY) || (filterB.categoryBits == BULLET && filterA.categoryBits == FBOUNDARY))
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

	if ((filterA.categoryBits == EBULLET && filterB.categoryBits == BOUNDARY) || (filterB.categoryBits == EBULLET && filterA.categoryBits == BOUNDARY))
	{
		if (filterA.categoryBits == EBULLET)
		{
			ECS::GetComponent<IsInactive>((int)fixtureA->GetBody()->GetUserData()).m_despawned = true;

		}
		else if (filterB.categoryBits == EBULLET)
		{
			ECS::GetComponent<IsInactive>((int)fixtureB->GetBody()->GetUserData()).m_despawned = true;

		}
	}*/

	if ((filterA.categoryBits == EBULLET && filterB.categoryBits == PLAYER) || (filterB.categoryBits == EBULLET && filterA.categoryBits == PLAYER))
	{
		if (filterA.categoryBits == EBULLET)
		{
			ECS::GetComponent<IsInactive>((int)fixtureA->GetBody()->GetUserData()).m_despawned = true;
			if (ECS::GetComponent<IceBlock>((int)fixtureB->GetBody()->GetUserData()).m_isActive)  {
				std::cout << "PARRIED, YOU FUCKING CASUAL!";
			}
			else {
				std::cout << "oof";
			}
		}
		else if (filterB.categoryBits == EBULLET)
		{
			ECS::GetComponent<IsInactive>((int)fixtureB->GetBody()->GetUserData()).m_despawned = true;
			if (ECS::GetComponent<IceBlock>((int)fixtureA->GetBody()->GetUserData()).m_isActive) {
				std::cout << "PARRIED, YOU FUCKING CASUAL!";
			}
			else {
				std::cout << "oof";
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
