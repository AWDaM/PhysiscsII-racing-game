#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "PhysVehicle3D.h"
#include "Timer.h"
#include "p2Log.h"




ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;
	state = THIRD_PERSON;
	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	
	/*float speed = 3.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;*/

	/*Position += Camera_view.translation();*/
	


	
	//Reference += newPos;
	
	// Mouse motion ----------------

	/*if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if(Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}*/

	// Recalculate matrix -------------
	//Last_position.y += 3;
	//Look(Last_position, Camera_view.translation(), true);
	//CalculateViewMatrix();
	//App->camera->Last_position = Camera_view.translation();
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT ||App->player->vehicle->GetKmh() < -60)
	{
		state = THIRD_PERSON_BACK;
	}
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		state = FIRST_PERSON;
	}
	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
	{
		state = FROM_WHEEL;
	}
	else
	{
		state = THIRD_PERSON;
	}
	FollowCar(state);
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

void ModuleCamera3D::FollowCar(CameraState state)
{
	btTransform transform = App->player->vehicle->vehicle->getChassisWorldTransform();
	btVector3 direction = transform.getBasis().getColumn(2);
	vec3 vec3direction = GetVec3From_btVec3(direction);

	while (pastDirections.Count() < 2)
	{
		pastDirections.Push(vec3direction);
	}

	switch (state)
	{
	case(1):
		ThirdPersonCamera();
		break;
	case(2):
		ThirdPersonCameraFromBack();
		break;
	case(3):
		FirstPersonCamera();
		break;
	case(4):
		WheelCamera();
	default:
		break;
	}
}

vec3 ModuleCamera3D::GetVec3From_btVec3(btVector3 vector)
{
	vec3 ret;

	ret.x = vector.x();
	ret.y = vector.y();
	ret.z = vector.z();

	return ret;
}

void ModuleCamera3D::ThirdPersonCamera()
{
	btTransform transform = App->player->vehicle->vehicle->getChassisWorldTransform();
	btVector3 position = transform.getOrigin();
	btVector3 direction = transform.getBasis().getColumn(2);
	vec3 vec3position = GetVec3From_btVec3(position);
	vec3 vec3direction = GetVec3From_btVec3(direction);


	vec3 pop;
	pastDirections.Pop(pop);
	vec3 CameraPosition = vec3position - 10 * pop;

	CameraPosition.y += 7;

	vec3 CameraDirection = 10 * vec3direction + vec3position;
	// CameraDirection.y = 10 * vec3direction.y + vec3position.y;
	// CameraDirection.z = 10 * vec3direction.z + vec3position.z;
	//CameraDirection.x = vec3position.x;
	// if (directionTimer.Read() >= 100)
	// {
	//	 directionTimer.Start();
	//	 CameraDirection.x += 10 * vec3direction.x;
	// }
	//

	Look(CameraPosition, CameraDirection, true);
}

void ModuleCamera3D::ThirdPersonCameraFromBack()
{
	btTransform transform = App->player->vehicle->vehicle->getChassisWorldTransform();
	btVector3 position = transform.getOrigin();
	btVector3 direction = transform.getBasis().getColumn(2);
	vec3 vec3position = GetVec3From_btVec3(position);
	vec3 vec3direction = GetVec3From_btVec3(direction);


	vec3 pop;
	pastDirections.Pop(pop);
	vec3 CameraDirection = vec3position - 10 * pop;



	vec3 CameraPosition = 10 * vec3direction + vec3position;
	CameraPosition.y += 7;

	Look(CameraPosition, CameraDirection, true);
}

void ModuleCamera3D::FirstPersonCamera()
{
	btTransform transform = App->player->vehicle->vehicle->getChassisWorldTransform();
	btVector3 position = transform.getOrigin();
	btVector3 direction = transform.getBasis().getColumn(2);
	vec3 vec3position = GetVec3From_btVec3(position);
	vec3 vec3direction = GetVec3From_btVec3(direction);
	vec3 tmp = vec3direction;
	vec3 CameraPosition = vec3position + 5*normalize(tmp);
	CameraPosition.y += 1;
	vec3 CameraDirection = vec3position + 10*vec3direction;
	Look(CameraPosition, CameraDirection, true);
	//LOG("x:%f y:%f z:%f", vec3direction.x, vec3direction.y, vec3direction.z);
}

void ModuleCamera3D::WheelCamera()
{
	//btTransform transform = App->player->vehicle->vehicle->getChassisWorldTransform();
	//btVector3 position = transform.getOrigin();
	//btVector3 direction = transform.getBasis().getColumn(2);
	//btVector3 tmp = direction.rotate(tmp, 90);
	//vec3 vec3position = GetVec3From_btVec3(position);
	//vec3 vec3direction = GetVec3From_btVec3(direction);
	//vec3 tmp2 = vec3direction;
	//vec3 CameraPosition = vec3position + normalize(tmp2) + normalize(GetVec3From_btVec3(tmp));
	//vec3 CameraDirection = vec3direction;

	//Look(CameraPosition, CameraDirection, true);
}

vec3 ModuleCamera3D::VehicleToWorld(vec3 localpos)
{
	vec3 ret;
	btTransform transform = App->player->vehicle->vehicle->getChassisWorldTransform();
	btVector3 position = transform.getOrigin();
	vec3 vec3position = GetVec3From_btVec3(position);
	ret = localpos + vec3position;

	return ret;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}