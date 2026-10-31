#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_CBAT()
{
	Identifier = "DEFAULT_PT_CBAT";
	Name = "CBAT";
	Colour = 0x2A2A2A_rgb;
	MenuVisible = 1;
	MenuSection = SC_NWMD;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f * CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 100;
	HeatConduct = 150;
	Description = "12V Car Battery. High capacity, drains when used.";

	Properties = TYPE_SOLID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 2273.0f;
	HighTemperatureTransition = PT_PLSM;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	// Temporary simple behaviour - will improve later
	// life = remaining charge (higher = more charge)
	if (parts[i].life <= 0)
		parts[i].life = 10000; // default capacity for now

	return 0;
}