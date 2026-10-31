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
	// tmp = current draw
	if (parts[i].tmp <= 0)
		parts[i].tmp = 1;

	// life > 0 means currently powered
	if (parts[i].life > 0)
	{
		parts[i].life--;
	}

	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	if (cpart->life > 0)
	{
		// Lit up
		*pixel_mode |= PMODE_GLOW;
		*colr = 255;
		*colg = 255;
		*colb = 180;
		*firea = 80;
		*firer = 255;
		*fireg = 255;
		*fireb = 150;
	}
	else
	{
		// Off
		*colr = 80;
		*colg = 80;
		*colb = 80;
	}
	return 0;
}
