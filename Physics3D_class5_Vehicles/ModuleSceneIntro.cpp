#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
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
	return App->physics->AddBody(c, 0.0f);
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{	
	for(p2List_item<Cube>* item = mapObjects.getFirst(); item; item = item->next)
		item->data.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

