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
	Meltable = 1;
	Hardness = 1;

	Weight = 100;
	HeatConduct = 250;
	Description = "Wire. tmp=amp rating (PROP). Realistic V=IR. Overloads heat and melt.";

	Properties = TYPE_SOLID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1673.0f;
	HighTemperatureTransition = PT_LAVA;

	Update = &update;
}

static float source_voltage(int type)
{
	switch (type) {
		case PT_CBAT:  return 12.0f;
		case PT_PBAT:  return 3.7f;
		case PT_ABAT:  return 1.5f;
		case PT_AABAT: return 1.5f;
		case PT_MAINS: return 240.0f;
		case PT_USPWR: return 120.0f;
	}
	return 0.0f;
}

static int update(UPDATE_FUNC_ARGS)
{
	// tmp     = amp rating
	// tmp2    = current (amps * 100)
	// tmp3    = powered flag
	// tmp4    = overload counter
	// life    = voltage at this wire
	// pavg[0] = grace counter (ticks with V>0 but no load seen)

	if (parts[i].tmp <= 0)
		parts[i].tmp = 5;

	float rating = (float)parts[i].tmp;
	float R_wire = 100.0f / rating;

	float V = 0.0f;
	float neighbor_I = 0.0f;
	float local_load_I = 0.0f;

	// Pass 1: find voltage (sources + adjacent wires)
	for (int rx = -1; rx <= 1; rx++)
	for (int ry = -1; ry <= 1; ry++)
	{
		if (!rx && !ry) continue;
		int nx = x + rx, ny = y + ry;
		if (nx < 0 || nx >= XRES || ny < 0 || ny >= YRES) continue;

		int r = pmap[ny][nx];
		if (!r) continue;
		int rt = TYP(r);
		int ri = ID(r);

		float src_V = source_voltage(rt);
		if (src_V > 0.0f && parts[ri].life > 0 && src_V > V)
			V = src_V;

		if (rt == PT_CSWR && (float)parts[ri].life > V)
			V = (float)parts[ri].life;
	}

	// Pass 2: local load current + inherited current
	for (int rx = -1; rx <= 1; rx++)
	for (int ry = -1; ry <= 1; ry++)
	{
		if (!rx && !ry) continue;
		int nx = x + rx, ny = y + ry;
		if (nx < 0 || nx >= XRES || ny < 0 || ny >= YRES) continue;

		int r = pmap[ny][nx];
		if (!r) continue;
		int rt = TYP(r);
		int ri = ID(r);

		if (rt == PT_LIGHT && parts[ri].tmp > 0 && V > 0.0f)
		{
			float R_load = 240.0f / (float)parts[ri].tmp;
			local_load_I += V / (2.0f * R_wire + R_load);
		}

		if (rt == PT_CSWR)
		{
			float nI = (float)parts[ri].tmp2 / 100.0f;
			if (nI > neighbor_I) neighbor_I = nI;
		}
	}

	// Current
	float current = 0.0f;
	if (local_load_I > 0.0f)
	{
		current = local_load_I + neighbor_I;
		parts[i].pavg[0] = 0.0f;
	}
	else if (neighbor_I > 0.0f)
	{
		current = neighbor_I;
		parts[i].pavg[0] = 0.0f;
	}
	else if (V > 0.0f)
	{
		parts[i].pavg[0] += 1.0f;
		if (parts[i].pavg[0] > 100.0f)
			current = V / (2.0f * R_wire);   // confirmed short circuit
	}
	else
	{
		parts[i].pavg[0] = 0.0f;
	}

	// Heat from current
	float heat = current * current / rating * 0.05f;
	if (current > rating)
	{
		heat += (current - rating) * 30.0f;
		parts[i].tmp4++;
	}
	else if (parts[i].tmp4 > 0)
	{
		parts[i].tmp4--;
	}
	parts[i].temp += heat;

	// Passive cooling toward ambient
	if (parts[i].temp > 293.15f)
	{
		parts[i].temp -= 1.0f;
		if (parts[i].temp < 293.15f) parts[i].temp = 293.15f;
	}

	// Keep adjacent lights lit
	if (V > 0.0f)
	{
		for (int rx = -1; rx <= 1; rx++)
		for (int ry = -1; ry <= 1; ry++)
		{
			if (!rx && !ry) continue;
			int nx = x + rx, ny = y + ry;
			if (nx < 0 || nx >= XRES || ny < 0 || ny >= YRES) continue;
			int r = pmap[ny][nx];
			if (r && TYP(r) == PT_LIGHT)
				parts[ID(r)].life = 2;
		}
	}

	// Store state
	parts[i].tmp2 = (int)(current * 100.0f);
	parts[i].tmp3 = (V > 0.0f) ? 1 : 0;
	parts[i].life = (int)V;

	return 0;
}
