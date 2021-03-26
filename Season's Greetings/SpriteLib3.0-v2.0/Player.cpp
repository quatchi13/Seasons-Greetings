#include "Player.h"
#include "Damage.h"

Player::Player()
{
}

Player::Player(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
					AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body);
}

void Player::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
							AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	//Store references to the components
	m_sprite = sprite;
	m_animController = controller;
	m_transform = transform;
	m_hasPhysics = hasPhys;
	if (hasPhys)
	{
		m_physBody = body;
	}

	//Initialize UVs
	m_animController->InitUVs(fileName);

	//Loads the texture and sets width and height
	m_sprite->LoadSprite(fileName, width, height, true, m_animController);
	m_animController->SetVAO(m_sprite->GetVAO());
	m_animController->SetTextureSize(m_sprite->GetTextureSize());

	//Loads in the animations json file
	nlohmann::json animations = File::LoadJSON(animationJSON);

	//IDLE ANIMATIONS\\
	
	//Idle Left
	m_animController->AddAnimation(animations["LeftIdle"].get<Animation>());
	//Idle Right
	m_animController->AddAnimation(animations["RightIdle"].get<Animation>());
#ifdef TOPDOWN
	//Idle Up
	m_animController->AddAnimation(animations["BackIdle"].get<Animation>());
	//Idle Down
	m_animController->AddAnimation(animations["FrontIdle"].get<Animation>());
#endif

	//Walk Animations\\

	//WalkLeft
	m_animController->AddAnimation(animations["LeftIdle"].get<Animation>());
	//WalkRight
	m_animController->AddAnimation(animations["RightIdle"].get<Animation>());
#ifdef TOPDOWN
	//WalkUP
	m_animController->AddAnimation(animations["BackIdle"].get<Animation>());
	//WalkDown
	m_animController->AddAnimation(animations["FrontIdle"].get<Animation>());
#endif

	//Attack Animations\\

//	//AttackLeft
//	m_animController->AddAnimation(animations["LeftMeleeAttack"].get<Animation>());
//	//AttackRight
//	m_animController->AddAnimation(animations["RightMeleeAttack"].get<Animation>());
//#ifdef TOPDOWN
//	//AttackUp
//	m_animController->AddAnimation(animations["BackMeleeAttack"].get<Animation>());
//	//AttackDown
//	m_animController->AddAnimation(animations["FrontMeleeAttack"].get<Animation>());
//#endif

	//Shooting anjsdnfksd
	m_animController->AddAnimation(animations["LeftShooter"].get<Animation>());
	//AttackRight
	m_animController->AddAnimation(animations["RightShooter"].get<Animation>());
#ifdef TOPDOWN
	//AttackUp
	m_animController->AddAnimation(animations["BackShooter"].get<Animation>());
	//AttackDown
	m_animController->AddAnimation(animations["FrontShooter"].get<Animation>());
#endif

	//BlockLeft
	m_animController->AddAnimation(animations["LeftBlock"].get<Animation>());
	//BlockRight
	m_animController->AddAnimation(animations["RightBlock"].get<Animation>());
#ifdef TOPDOWN
	//BlockUp
	m_animController->AddAnimation(animations["BackBlock"].get<Animation>());
	//BlockDown
	m_animController->AddAnimation(animations["FrontBlock"].get<Animation>());
#endif

	//DamageLeft
	m_animController->AddAnimation(animations["LeftDamage"].get<Animation>());
	//AttackRight
	m_animController->AddAnimation(animations["RightDamage"].get<Animation>());
#ifdef TOPDOWN
	//AttackUp
	m_animController->AddAnimation(animations["BackDamage"].get<Animation>());
	//AttackDown
	m_animController->AddAnimation(animations["FrontDamage"].get<Animation>());
#endif


	//Set Default Animation
	m_animController->SetActiveAnim(DOWN);


}

void Player::Update()
{
	if (needToAdd1) {
		attackFrame++;
		if (attackFrame == 48) {
			needToAdd1 = false;
			attackFrame = 0;
		}
	}

	if (!m_locked)
	{
		MovementUpdate();
	}

	AnimationUpdate();

}

void Player::MovementUpdate()
{
	m_moving = false;

	if (m_hasPhysics)
	{
		float speed = 10.f;
		vec3 vel = vec3(0.f, 0.f, 0.f);

		if (Input::GetKey(Key::Shift))
		{
			speed *= 7.f;
		}

#ifdef TOPDOWN
		if (Input::GetKey(Key::W))
		{
			vel = vel + vec3(0.f, 1.f, 0.f);
			m_facing = UP;
			m_moving = true;
		}
		if (Input::GetKey(Key::S))
		{
			vel = vel + vec3(0.f, -1.f, 0.f);
			m_facing = DOWN;
			m_moving = true;
		}
#endif

		if (Input::GetKey(Key::A))
		{
			vel = vel + vec3(-1.f, 0.f, 0.f);
			m_facing = LEFT;
			m_moving = true;
		}
		if (Input::GetKey(Key::D))
		{
			vel = vel + vec3(1.f, 0.f, 0.f);
			m_facing = RIGHT;
			m_moving = true;
		}

		m_physBody->SetVelocity(vel * speed);
	}
	else
	{
		//Regular Movement
		float speed = 15.f;

#ifdef TOPDOWN
		if (Input::GetKey(Key::W))
		{
			m_transform->SetPositionY(m_transform->GetPositionY() + (speed * Timer::deltaTime));
			m_facing = UP;
			m_moving = true;
		}
		if (Input::GetKey(Key::S))
		{
			m_transform->SetPositionY(m_transform->GetPositionY() - (speed * Timer::deltaTime));
			m_facing = DOWN;
			m_moving = true;
		}
#endif

		if (Input::GetKey(Key::A))
		{
			m_transform->SetPositionX(m_transform->GetPositionX() - (speed * Timer::deltaTime));
			m_facing = LEFT;
			m_moving = true;
		}
		if (Input::GetKey(Key::D))
		{
			m_transform->SetPositionX(m_transform->GetPositionX() + (speed * Timer::deltaTime));
			m_facing = RIGHT;
			m_moving = true;
		}
	}


	if (!needToAdd1 && !ECS::GetComponent<IceBlock>(MainEntities::MainPlayer()).m_isActive) {
		if (Input::GetKeyDown(Key::UpArrow))
		{
			m_moving = false;

			/*if (m_hasPhysics)
			{
				m_physBody->SetVelocity(vec3());
			}*/
			m_facing = UP;
			m_Shooting = true;
			m_locked = true;
		}
		else if (Input::GetKeyDown(Key::DownArrow))
		{
			m_moving = false;
			m_facing = DOWN;
			m_Shooting = true;
			m_locked = true;
		}
		else if (Input::GetKeyDown(Key::LeftArrow))
		{
			m_moving = false;
			m_facing = LEFT;
			m_Shooting = true;
			m_locked = true;
		}
		else if (Input::GetKeyDown(Key::RightArrow))
		{
			m_moving = false;
			m_facing = RIGHT;
			m_Shooting = true;
			m_locked = true;
		}
		
	}
	

	//Blocking animations

	if (Input::GetKey(Key::Shift)) {
		m_moving = false;

		m_Block = true;
		m_locked = true;
	}

	if (Input::GetKeyUp(Key::Shift))
	{
		//Will auto set to idle
		m_locked = false;
		m_Block = false;
		//if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone()) {
		//	//Resets the attack animation
		m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();
		//}
	}
}

void Player::AnimationUpdate()
{
	int activeAnimation = 0;

	if (m_moving)
	{
		//Puts it into the WALK category
		activeAnimation = WALK;
	}
	//else if (m_attacking)
	//{
	//	activeAnimation = ATTACK;

	//	//Check if the attack animation is done
	//	if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
	//	{
	//		//Will auto set to idle
	//		m_locked = false;
	//		m_attacking = false;
	//		//Resets the attack animation
	//		m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();

	//		activeAnimation = IDLE;
	//	}
	//}
	else if (m_Shooting)
	{
		activeAnimation = SHOOT;

		//Check if the attack animation is done
		if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
		{
			//Will auto set to idle
			m_locked = false;
			m_Shooting = false;
			//Resets the attack animation
			m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();

			activeAnimation = IDLE;
		}
	}
	else if (m_Block) {
		activeAnimation = BLOCK;
		if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
		{
			//Will auto set to idle
			m_locked = false;
			m_Block = false;
			//Resets the attack animation
			//m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();

			//activeAnimation = IDLE;
		}
	}
	else
	{
		activeAnimation = IDLE;
	}

	SetActiveAnimation(activeAnimation + (int)m_facing);
}



void Player::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}
