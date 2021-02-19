#include "EnemyShotTrigger.h"
#include "ECS.h"

void EnemyShotTrigger::OnEnter()
{
	Trigger::OnEnter();
	

	for (int i = 0; i < m_targetEntities.size(); i++)
	{

		//make bullet visible
		auto& ebSpr = ECS::GetComponent<Sprite>(m_targetEntities[i]);

		ebSpr.SetTransparency(1.f);

		auto& enBul = ECS::GetComponent<PhysicsBody>(m_targetEntities[i]);


		enBul.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		//shoot bullet

		auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

		vec3 source((enBul.GetBody()->GetPosition().x), (enBul.GetBody()->GetPosition().y), 3);
		vec3 target((player.GetBody()->GetPosition().x + 50), (player.GetBody()->GetPosition().y), 3);

		b2Vec2 shotVec(target.x - source.x, target.y - source.y);
		b2Vec2 unitVec = shotVec;
		double normal = sqrt(pow(shotVec.x, 2) + pow(shotVec.y, 2));
		unitVec.x /= normal;
		unitVec.y /= normal;

		vec3 velocity(unitVec.x * 75, unitVec.y * 75, 0);

		enBul.SetVelocity(velocity);


	}


}

void EnemyShotTrigger::OnExit()
{
	Trigger::OnExit();
}
