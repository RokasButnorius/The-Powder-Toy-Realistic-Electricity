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
	Advection = 0.0f; AirDrag = 0.00f * CFDS; AirLoss = 0.90f;
	Loss = 0.00f; Collision = 0.0f; Gravity = 0.0f; Diffusion = 0.00f;
	HotAir = 0.000f * CFDS; Falldown = 0;
	Flammable = 0; Explosive = 0; Meltable = 0; Hardness = 1;
	Weight = 100; HeatConduct = 150;
	Description = "12V Car Battery. Drains at rate current x voltage.";
	Properties = TYPE_SOLID;
	LowPressure = IPL; LowPressureTransition = NT;
	HighPressure = IPH; HighPressureTransition = NT;
	LowTemperature = ITL; LowTemperatureTransition = NT;
	HighTemperature = 2273.0f; HighTemperatureTransition = PT_PLSM;
	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	// tmp  = initialized flag
	// life = remaining charge
	if (parts[i].tmp == 0)
	{
		parts[i].life = 10000;
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
		parts[i].life -= (int)(total_current * 12.0f / 3.0f);
		if (parts[i].life < 0) parts[i].life = 0;
	}

	return 0;
}
