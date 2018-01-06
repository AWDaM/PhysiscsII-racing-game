#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{

	LoadCarFromXML();
	btTransform transform = App->player->vehicle->vehicle->getChassisWorldTransform();
	btVector3 position = transform.getOrigin();
	btVector3 direction = transform.getBasis().getColumn(2);
	App->camera->SetInitialSpeedAndPos(position, direction);
	return true;
}

// Unload assets

void ModulePlayer::LoadCarFromXML()
{
	LOG("Loading player");

	pugi::xml_document carconfig;
	pugi::xml_node node;
	pugi::xml_parse_result result = carconfig.load_file("car.xml");
	if (!result)
	{
		LOG("The config file couldnt be loaded properly because %s", result.description());
	}
	else
	{
		LOG("File loaded correctly");
		node = carconfig.child("car");
	}
	car = new VehicleInfo();
	// Car properties ----------------------------------------
	car->chassis_size.Set(node.child("size").attribute("x").as_float(), node.child("size").attribute("y").as_float(), node.child("size").attribute("z").as_float());
	car->chassis_offset.Set(node.child("offset").attribute("x").as_float(), node.child("offset").attribute("y").as_float(), node.child("offset").attribute("z").as_float());
	car->mass = node.child("mass").attribute("value").as_float();
	car->suspensionStiffness = node.child("suspensionStiffness").attribute("value").as_float();
	car->suspensionCompression = node.child("suspensionCompression").attribute("value").as_float();
	car->suspensionDamping = node.child("suspensionDamping").attribute("value").as_float();
	car->maxSuspensionTravelCm = node.child("maxSuspensionTravelCm").attribute("value").as_float();
	car->frictionSlip = node.child("frictionSlip").attribute("value").as_float();
	car->maxSuspensionForce = node.child("maxSuspensionForce").attribute("value").as_float();

	// Wheel properties ---------------------------------------
	float connection_height = node.child("connection_height").attribute("value").as_float();
	float wheel_radius = node.child("wheel_radius").attribute("value").as_float();
	float wheel_width = node.child("wheel_width").attribute("value").as_float();
	float suspensionRestLength = node.child("suspensionRestLength").attribute("value").as_float();

	// Don't change anything below this line ------------------

	float half_width = car->chassis_size.x*0.5f;
	float half_length = car->chassis_size.z*0.5f;

	vec3 direction(node.child("direction").attribute("x").as_float(), node.child("direction").attribute("y").as_float(), node.child("direction").attribute("z").as_float());
	vec3 axis(node.child("axis").attribute("x").as_float(), node.child("axis").attribute("y").as_float(), node.child("axis").attribute("z").as_float());


	car->num_wheels = 4;
	car->wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	pugi::xml_node wheel = node.child("wheel_FrontLeft");
	car->wheels[0].connection.Set(half_width +  wheel.child("connectionOffset").attribute("value").as_float()* wheel_width, connection_height, half_length - wheel_radius);
	car->wheels[0].direction = direction;
	car->wheels[0].axis = axis;
	car->wheels[0].suspensionRestLength = suspensionRestLength;
	car->wheels[0].radius = wheel_radius;
	car->wheels[0].width = wheel_width;
	car->wheels[0].front = wheel.child("front").attribute("value").as_bool();
	car->wheels[0].drive = wheel.child("drive").attribute("value").as_bool();
	car->wheels[0].brake = wheel.child("brake").attribute("value").as_bool();
	car->wheels[0].steering = wheel.child("steering").attribute("value").as_bool();

	// FRONT-RIGHT ------------------------
	wheel = node.child("wheel_FrontRight");
	car->wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car->wheels[1].direction = direction;
	car->wheels[1].axis = axis;
	car->wheels[1].suspensionRestLength = suspensionRestLength;
	car->wheels[1].radius = wheel_radius;
	car->wheels[1].width = wheel_width;
	car->wheels[1].front = wheel.child("front").attribute("value").as_bool();
	car->wheels[1].drive = wheel.child("drive").attribute("value").as_bool();
	car->wheels[1].brake = wheel.child("brake").attribute("value").as_bool();
	car->wheels[1].steering = wheel.child("steering").attribute("value").as_bool();

	// REAR-LEFT ------------------------
	wheel = node.child("wheel_RearLeft");
	car->wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car->wheels[2].direction = direction;
	car->wheels[2].axis = axis;
	car->wheels[2].suspensionRestLength = suspensionRestLength;
	car->wheels[2].radius = wheel_radius;
	car->wheels[2].width = wheel_width;
	car->wheels[2].front = wheel.child("front").attribute("value").as_bool();
	car->wheels[2].drive = wheel.child("drive").attribute("value").as_bool();
	car->wheels[2].brake = wheel.child("brake").attribute("value").as_bool();
	car->wheels[2].steering = wheel.child("steering").attribute("value").as_bool();

	// REAR-RIGHT ------------------------
	wheel = node.child("wheel_RearRight");
	car->wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car->wheels[3].direction = direction;
	car->wheels[3].axis = axis;
	car->wheels[3].suspensionRestLength = suspensionRestLength;
	car->wheels[3].radius = wheel_radius;
	car->wheels[3].width = wheel_width;
	car->wheels[3].front = wheel.child("front").attribute("value").as_bool();
	car->wheels[3].drive = wheel.child("drive").attribute("value").as_bool();
	car->wheels[3].brake = wheel.child("brake").attribute("value").as_bool();
	car->wheels[3].steering = wheel.child("steering").attribute("value").as_bool();


	vehicle = App->physics->AddVehicle(*car);

	vehicle->SetPos(node.child("position").attribute("x").as_float(), node.child("position").attribute("y").as_float(), node.child("position").attribute("z").as_float());

	vehicle->GetTransform(matrix);
}

void ModulePlayer::SetCarToStart()
{
	vehicle->SetTransform(matrix);
	vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0,0,0 });
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && vehicle->GetKmh() < 250)
	{
		acceleration = MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		acceleration = - MAX_ACCELERATION/2;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		restart = true;
		restarting.Start();
		
	}

	if (restart && restarting.ReadSec() < 0.3f)
	{
		SetCarToStart();
		brake = BRAKE_POWER;
		acceleration = -acceleration;
		turn = 0;
	}
	else if (restarting.ReadSec() > 0.3f)
	{
		restart = false;
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);
	vehicle->GetTransform(&App->camera->Camera_view);

	

	vehicle->Render();

	char title[80];
	sprintf_s(title, "%.1f Km/h", vehicle->GetKmh());
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}



