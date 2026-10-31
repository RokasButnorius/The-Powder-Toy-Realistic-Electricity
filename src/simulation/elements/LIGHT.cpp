#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_LIGHT()
{
	Identifier = "DEFAULT_PT_LIGHT";
	Name = "LIGHT";
	Colour = 0xAAAAAA_rgb;
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
	HeatConduct = 40;
	Description = "Light. Draws current when powered. Lights up.";

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
	Graphics = &graphics;
}

static int update(UPDATE_FUNC_ARGS)
{
	// tmp = current draw (default ~0.5A for now)
	if (parts[i].tmp <= 0)
		parts[i].tmp = 1;

	// life > 0 