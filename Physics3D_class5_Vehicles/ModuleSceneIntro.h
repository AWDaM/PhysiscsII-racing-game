#pragma once
#include "Module.h"
#include "p2List.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
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
	void AddBridgeConstrain();
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/
	p2List<Cube> mapObjects;

	PhysBody3D* firstObstacle;
	PhysBody3D* secondObstacle;
	PhysBody3D* thirdObstacle;
	PhysBody3D* Obstacle4;
	PhysBody3D* Obstacle5;
	PhysBody3D* Obstacle6;
	PhysBody3D* Obstacle7;
	PhysBody3D* Obstacle8;
	PhysBody3D* Obstacle9;

	PhysBody3D* firstObstaclePhys;
	PhysBody3D* secondObstaclePhys;
	PhysBody3D* thirdObstaclePhys;
	PhysBody3D* Obstacle4Phys;
	PhysBody3D* Obstacle5Phys;
	PhysBody3D* Obstacle6Phys;
	PhysBody3D* Obstacle7Phys;
	PhysBody3D* Obstacle8Phys;
	PhysBody3D* Obstacle9Phys;

	Cube firstObstacleRender;
	Cube secondObstacleRender;
	Cube thirdObstacleRender;
	Cube Obstacle4Render;
	Cube Obstacle5Render;
	Cube Obstacle6Render;
	Cube Obstacle7Render;
	Cube Obstacle8Render;
	Cube Obstacle9Render;

	Cube* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;
};
