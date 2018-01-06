#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "Primitive.h"
#include "PhysBody3D.h"

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
	if (!node.child("firstObstacle").attribute("value").as_bool(false) && !node.child("secondObstacle").attribute("value").as_bool(false) && !node.child("thirdObstacle").attribute("value").as_bool(false))
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

}

// Update
update_status ModuleSceneIntro::Update(float dt)
{	
	for(p2List_item<Cube>* item = mapObjects.getFirst(); item; item = item->next)
		item->data.Render();

	firstObstaclePhys->GetTransform(&firstObstacleRender.transform);
	secondObstaclePhys->GetTransform(&secondObstacleRender.transform);
	thirdObstaclePhys->GetTransform(&thirdObstacleRender.transform);

	firstObstacleRender.Render();
	secondObstacleRender.Render();
	thirdObstacleRender.Render();
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

