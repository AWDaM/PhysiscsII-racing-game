#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "ModuleAudio.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Primitive.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	bestTime = 999;
	laps = 0;

	App->audio->PlayMusic("BGM.ogg");

	LoadLevelFromXML();
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

bool ModuleSceneIntro::LoadLevelFromXML()
{
	bool ret = false;
	pugi::xml_document map_file;
	pugi::xml_node node;
	pugi::xml_parse_result result = map_file.load_file("map.xml");
	if (!result)
	{
		LOG("The config file couldnt be loaded properly because %s", result.description());
	}
	else
	{
		ret = true;
		LOG("File loaded correctly");
		node = map_file.child("map");
	}
	pugi::xml_node tmp = node.child("cube");

	while (tmp)
	{
		LoadCubeFromXML(tmp);
		tmp = tmp.next_sibling("cube");
	}
	
	tmp = node.child("sensor");
	while (tmp)
	{
		LoadSensorFromXML(tmp);
		tmp = tmp.next_sibling("sensor");
	}

	AddBridgeConstrain();

	return ret;
}

PhysBody3D* ModuleSceneIntro::LoadCubeFromXML(pugi::xml_node node)
{
	Cube c;
	c.SetSize(node.child("size").attribute("x").as_float(), node.child("size").attribute("y").as_float(), node.child("size").attribute("z").as_float());
	c.SetPos(node.child("pos").attribute("x").as_float(), node.child("pos").attribute("y").as_float(), node.child("pos").attribute("z").as_float());
	
	float angle = node.child("rotation").attribute("angle").as_float(0);

	if(angle != 0)
	c.SetRotation(angle , { node.child("vector").attribute("x").as_float(),node.child("vector").attribute("y").as_float(),node.child("vector").attribute("z").as_float() });
	
	mapObjects.add(c);
	if (!node.child("firstObstacle").attribute("value").as_bool(false) && !node.child("secondObstacle").attribute("value").as_bool(false) && !node.child("thirdObstacle").attribute("value").as_bool(false) && !node.child("Obstacle4").attribute("value").as_bool(false) && !node.child("Obstacle5").attribute("value").as_bool(false) && !node.child("Obstacle6").attribute("value").as_bool(false) && !node.child("Obstacle7").attribute("value").as_bool(false) && !node.child("Obstacle8").attribute("value").as_bool(false) && !node.child("Obstacle9").attribute("value").as_bool(false))
		return App->physics->AddBody(c, 0.0f);
	else if (node.child("firstObstacle").attribute("value").as_bool(false))
	{
		firstObstacle = App->physics->AddBody(c, 0.0f);
		return firstObstacle;
	}
	else if (node.child("secondObstacle").attribute("value").as_bool(false))
	{
		secondObstacle = App->physics->AddBody(c, 0.0f);
		return secondObstacle;
	}
	else if (node.child("thirdObstacle").attribute("value").as_bool(false))
	{
		thirdObstacle = App->physics->AddBody(c, 0.0f);
		return thirdObstacle;
	}
	else if (node.child("Obstacle4").attribute("value").as_bool(false))
	{
		Obstacle4 = App->physics->AddBody(c, 0.0f);
		return Obstacle4;
	}
	else if (node.child("Obstacle5").attribute("value").as_bool(false))
	{
		Obstacle5 = App->physics->AddBody(c, 0.0f);
		return Obstacle5;
	}
	else if (node.child("Obstacle6").attribute("value").as_bool(false))
	{
		Obstacle6 = App->physics->AddBody(c, 0.0f);
		return Obstacle6;
	}
	else if (node.child("Obstacle7").attribute("value").as_bool(false))
	{
		Obstacle7 = App->physics->AddBody(c, 0.0f);
		return Obstacle7;
	}
	else if (node.child("Obstacle8").attribute("value").as_bool(false))
	{
		Obstacle8 = App->physics->AddBody(c, 0.0f);
		return Obstacle8;
	}
	else if (node.child("Obstacle9").attribute("value").as_bool(false))
	{
		Obstacle9 = App->physics->AddBody(c, 0.0f);
		return Obstacle9;
	}
}

void ModuleSceneIntro::AddBridgeConstrain()
{
	Cube c(49, 5, 49);
	c.SetPos(125, 55, -500);

	firstObstaclePhys = App->physics->AddBody(c, 1000);
	firstObstacleRender = c;

	btVector3 pivotStart(0, 0, 0);
	btVector3 pivotA(-22.5, 0, 0);

	
	App->physics->AddConstraintHinge(*firstObstacle, *firstObstaclePhys, App->camera->GetVec3From_btVec3(pivotStart), App->camera->GetVec3From_btVec3(pivotA), { 0,0,1 }, { 0,0,1 }, true);

	c.SetPos(125, 55, -600);

	secondObstacleRender = c;
	secondObstaclePhys = App->physics->AddBody(c, 1000);
	App->physics->AddConstraintHinge(*secondObstacle, *secondObstaclePhys, App->camera->GetVec3From_btVec3(pivotStart), App->camera->GetVec3From_btVec3(pivotA), { 0,0,1 }, { 0,0,1 }, true);
	
	c.SetPos(125, 55, -700);

	thirdObstacleRender = c;
	thirdObstaclePhys = App->physics->AddBody(c, 1000);
	App->physics->AddConstraintHinge(*thirdObstacle, *thirdObstaclePhys, App->camera->GetVec3From_btVec3(pivotStart), App->camera->GetVec3From_btVec3(pivotA), { 0,0,1 }, { 0,0,1 }, true);

	c.SetPos(0, 55, -750);

	Obstacle4Render = c;
	Obstacle4Phys = App->physics->AddBody(c, 1000);
	App->physics->AddConstraintHinge(*Obstacle4, *Obstacle4Phys, App->camera->GetVec3From_btVec3(pivotStart), App->camera->GetVec3From_btVec3(pivotA), { 0,0,1 }, { 0,0,1 }, true);

	c.SetPos(0, 55, -700);

	Obstacle5Render = c;
	Obstacle5Phys = App->physics->AddBody(c, 1000);
	App->physics->AddConstraintHinge(*Obstacle5, *Obstacle5Phys, App->camera->GetVec3From_btVec3(pivotStart), App->camera->GetVec3From_btVec3(pivotA), { 0,0,1 }, { 0,0,1 }, true);

	c.SetPos(0, 55, -500);

	Obstacle6Render = c;
	Obstacle6Phys = App->physics->AddBody(c, 1000);
	App->physics->AddConstraintHinge(*Obstacle6, *Obstacle6Phys, App->camera->GetVec3From_btVec3(pivotStart), App->camera->GetVec3From_btVec3(pivotA), { 0,0,1 }, { 0,0,1 }, true);

	c.SetPos(0, 55, -450);

	Obstacle7Render = c;
	Obstacle7Phys = App->physics->AddBody(c, 1000);
	App->physics->AddConstraintHinge(*Obstacle7, *Obstacle7Phys, App->camera->GetVec3From_btVec3(pivotStart), App->camera->GetVec3From_btVec3(pivotA), { 0,0,1 }, { 0,0,1 }, true);

	c.SetPos(0, 55, -300);

	Obstacle8Render = c;
	Obstacle8Phys = App->physics->AddBody(c, 1000);
	App->physics->AddConstraintHinge(*Obstacle8, *Obstacle8Phys, App->camera->GetVec3From_btVec3(pivotStart), App->camera->GetVec3From_btVec3(pivotA), { 0,0,1 }, { 0,0,1 }, true);

	c.SetPos(0, 55, -200);

	Obstacle9Render = c;
	Obstacle9Phys = App->physics->AddBody(c, 1000);
	App->physics->AddConstraintHinge(*Obstacle9, *Obstacle9Phys, App->camera->GetVec3From_btVec3(pivotStart), App->camera->GetVec3From_btVec3(pivotA), { 0,0,1 }, { 0,0,1 }, true);

}

PhysBody3D* ModuleSceneIntro::LoadSensorFromXML(pugi::xml_node node)
{
	Cube c;
	PhysBody3D* tmp;
	c.SetSize(node.child("size").attribute("x").as_float(), node.child("size").attribute("y").as_float(), node.child("size").attribute("z").as_float());
	c.SetPos(node.child("pos").attribute("x").as_float(), node.child("pos").attribute("y").as_float(), node.child("pos").attribute("z").as_float());

	float angle = node.child("rotation").attribute("angle").as_float(0);

	if (angle != 0)
		c.SetRotation(angle, { node.child("vector").attribute("x").as_float(),node.child("vector").attribute("y").as_float(),node.child("vector").attribute("z").as_float() });

	tmp = App->physics->AddBody(c, 0.0f);
	tmp->SetAsSensor(true);
	tmp->collision_listeners.add(this);
	tmp->s_type = GetTypeFromInt(node.child("type").attribute("value").as_int());

	return tmp;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{	
	for(p2List_item<Cube>* item = mapObjects.getFirst(); item; item = item->next)
		item->data.Render();

	firstObstaclePhys->GetTransform(&firstObstacleRender.transform);
	secondObstaclePhys->GetTransform(&secondObstacleRender.transform);
	thirdObstaclePhys->GetTransform(&thirdObstacleRender.transform); 
	Obstacle4Phys->GetTransform(&Obstacle4Render.transform);
	Obstacle5Phys->GetTransform(&Obstacle5Render.transform);
	Obstacle6Phys->GetTransform(&Obstacle6Render.transform);
	Obstacle7Phys->GetTransform(&Obstacle7Render.transform);
	Obstacle8Phys->GetTransform(&Obstacle8Render.transform);
	Obstacle9Phys->GetTransform(&Obstacle9Render.transform);

	firstObstacleRender.Render();
	secondObstacleRender.Render();
	thirdObstacleRender.Render(); 
	Obstacle4Render.Render();
	Obstacle5Render.Render();
	Obstacle6Render.Render();
	Obstacle7Render.Render();
	Obstacle8Render.Render();
	Obstacle9Render.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1->s_type == DEATH && body2->IsVehicle() && !App->player->restart)
	{
		App->player->dead = true;
		half_circuit = false;
	}
	if (body1->s_type == MID_CIRCUIT && body2->IsVehicle())
	{
		half_circuit = true;
	}
	if (body1->s_type == START && body2->IsVehicle() && half_circuit)
	{
		half_circuit = false;
		if (laps <= 3)
		{
			laps++;
			if (bestTime > laptimer.ReadSec())
			{
				bestTime = laptimer.ReadSec();
			}
		}
		else
		{
			finished;
			
		}
	}
	if (body1->s_type == START && body2->IsVehicle())
	{
		laptimer.Start();
	}
}

SensorType ModuleSceneIntro::GetTypeFromInt(int type)
{
	SensorType ret;
	switch (type)
	{
	case(1):
		ret = DEATH;
		break;
	case(2):
		ret = START;
		break;
	case(3):
		ret = MID_CIRCUIT;
		break;
	default:
		ret = NO_SENSOR;
		break;
	}
	return ret;
}

