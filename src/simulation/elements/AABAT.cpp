#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_AABAT()
{
	Identifier = "DEFAULT_PT_AABAT";
	Name = "AABAT";
	Colour = 0xC0C0C0_rgb;
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
	HeatConduct = 100;
	Description = "AA 1.5V Battery. Normal capacity.";

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
	if (parts[i].tmp == 0)
	{
		parts[i].life = 2500;
		parts[i].tmp = 1;
	}

	float total_current = 0.0f;
	for (int rx = -1; rx <= 1; rx++)
	for (int ry = -1; ry <= 1; ry++)
	{
		if (!rx && !ry) continue;
		int nx = x + rx, ny = y + ry;
		if (nx < 0 || nx >= XRES || ny < 0 || ny >= YRES) continue;
		int r = pmap[ny][nx];
		if (r && TYP(r) == PT_CSWR)
			total_current += (float)parts[ID(r)].tmp2 / 100.0f;
	}

	if (total_current > 0.0f)
	{
		parts[i].tmp2 += (int)(total_current * 1.5f * 100.0f / 3.0f);
		int drain = parts[i].tmp2 / 100;
		if (drain > 0)
		{
			parts[i].life -= drain;
			parts[i].tmp2 -= drain * 100;
			if (parts[i].life < 0) parts[i].life = 0;
		}
	}

	return 0;
}
