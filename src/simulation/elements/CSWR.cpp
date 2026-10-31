#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_CSWR()
{
	Identifier = "DEFAULT_PT_CSWR";
	Name = "CSWR";
	Colour = 0x505050_rgb;
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
	Meltable = 1;          // can melt when overloaded
	Hardness = 1;

	Weight = 100;
	HeatConduct = 250;
	Description = "Custom Wire. Uses deco colour as wire ID. Set amp rating with PROP.";

	Properties = TYPE_SOLID | PROP_CONDUCTS | PROP_LIFE_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1673.0f;          // melts around here when overloaded
	HighTemperatureTransition = PT_LAVA;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	// Temporary simple behaviour
	// tmp  = max amp rating (default 5A for now)
	// tmp2 = current flowing through it
	// life = heat buildup / overload timer

	if (parts[i].tmp <= 0)
		parts[i].tmp = 5;   // default 5 amp rating

	// Very basic heating if current > rating (will improve later)
	if (parts[i].tmp2 > parts[i].tmp)
	{
		parts[i].temp += (parts[i].tmp2 - parts[i].tmp) * 0.8f;
		parts[i].life++;
	}
	else if (parts[i].life > 0)
	{
		parts[i].life--;
	}

	return 0;
}