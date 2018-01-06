#pragma once
#include "Module.h"
#include "p2List.h"
#include "Globals.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#define MAX_SNAKE 2


struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	 
	bool LoadLevelFromXML();
	PhysBody3D* LoadCubeFromXML(pugi::xml_node node);
	PhysBody3D* LoadSensorFromXML(pugi::xml_node node);
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	SensorType GetTypeFromInt(int type);

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/
	p2List<Cube> mapObjects;

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;
};
