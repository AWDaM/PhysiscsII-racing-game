#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;
struct VehicleInfo;
#define MAX_ACCELERATION 10000.0f
#define TURN_DEGREES 45.0f * DEGTORAD
#define BRAKE_POWER 1000.0f


class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	void SetCarToStart();
	update_status Update(float dt);
	bool CleanUp();
	void LoadCarFromXML();

public:

	VehicleInfo* car;
	float matrix[16];
	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	bool restart = false;
	bool dead = false;
	Timer restarting;
};