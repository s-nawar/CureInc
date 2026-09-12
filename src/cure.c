/* cure.c */
#include "cure.h"
#include <stddef.h>

void cure_init(CureState *c)
{
    c->phase              = PHASE_DISCOVERY;
    c->researchProgress   = 0.0f;
    c->stability          = 1.0f;
    c->effectiveness      = 1.0f;
    c->productionRate     = 1.0f;
    c->globalDistributed  = 0.0f;
    c->completionDay      = 0;
    
    c->funding            = 50.0f;     /* Start with minimal funding */
    c->fundingPerTick     = 10.0f;    /* Income: first $100 scientist is affordable after 5 days */
    c->researchPoints     = 0.0f;
    c->rpPerTick          = 0.5f;     /* Base research; purchases provide the main speed boosts */
    
    /* Initialize gameplay systems */
    c->scientistCount     = 0;
    c->labLevel           = 0;
    c->productionLevel    = 0;
    c->vaccineStockpile   = 0.0f;
}

/* One vaccine unit supplies doses for 1% of the original world population. */
#define VACCINE_UNIT_SHARE 0.01f
#define INITIAL_STOCK_GOAL 10.0f

float cure_production_rate(const CureState *c)
{
    return 1.0f + c->productionLevel * 0.5f + c->scientistCount * 0.2f;
}

/* Shared by the simulation and HUD: actual progress points per game day. */
float cure_research_rate(const GameState *gs)
{
    const CureState *c = &gs->cure;
    if (c->phase != PHASE_DISCOVERY && c->phase != PHASE_TRIALS)
        return 0.0f;

    /* Balance: a $100 regional grant adds 15 * .008 = .12 base RP/day.
     * Scientists and lab levels add to their own multipliers, not compound. */
    float regionalBoost = 0.0f;
    for (int i = 0; i < MAX_REGIONS; i++)
        regionalBoost += gs->regions[i].cureResearch * 0.008f;

    return (c->rpPerTick + regionalBoost)
        * (1.0f + c->scientistCount * 0.20f)
        * (1.0f + c->labLevel * 0.25f)
        * c->stability * (1.0f - gs->virus.resistance * 0.5f);
}

/* Production progresses through stock, not the unused research counter. */
float cure_phase_progress(const CureState *c)
{
    float progress = c->researchProgress;
    if (c->phase == PHASE_PRODUCTION)
        progress = c->vaccineStockpile / INITIAL_STOCK_GOAL * 100.0f;
    else if (c->phase == PHASE_DISTRIBUTION)
        progress = c->globalDistributed * 100.0f;

    if (progress < 0.0f) return 0.0f;
    if (progress > 100.0f) return 100.0f;
    return progress;
}

void cure_update(GameState *gs, float dtDays)
{
    CureState *c = &gs->cure;
    if (dtDays <= 0.0f) return;

    c->funding += c->fundingPerTick * dtDays;
    c->researchPoints += c->rpPerTick * dtDays;
    c->productionRate = cure_production_rate(c);

    if (c->phase == PHASE_DISCOVERY || c->phase == PHASE_TRIALS) {
        c->researchProgress += cure_research_rate(gs) * dtDays;
        if (c->researchProgress >= 100.0f) {
            c->researchProgress = 0.0f;
            c->phase++;
        }
        return;
    }

    c->vaccineStockpile += c->productionRate * dtDays;
    c->effectiveness = c->stability * (1.0f - gs->virus.resistance * 0.25f);
    if (c->effectiveness < 0.0f) c->effectiveness = 0.0f;
    if (c->effectiveness > 1.0f) c->effectiveness = 1.0f;

    if (c->phase == PHASE_PRODUCTION) {
        if (c->vaccineStockpile >= INITIAL_STOCK_GOAL) {
            c->phase = PHASE_DISTRIBUTION;
            if (c->completionDay == 0) c->completionDay = gs->day;
        }
        return;
    }

    if (c->phase != PHASE_DISTRIBUTION || c->effectiveness <= 0.0f)
        return;

    float totalPop = 0.0f;
    for (int i = 0; i < MAX_REGIONS; i++)
        totalPop += gs->regions[i].population;
    if (totalPop <= 0.0f || c->vaccineStockpile <= 0.0f) return;

    /* Allocate the available stock proportionally by original population.
     * Equal local fractions already give proportional numbers of doses.
     * Unused allocations stay in stock for another day. */
    float offeredFraction = c->vaccineStockpile * VACCINE_UNIT_SHARE;
    float usedUnits = 0.0f;
    for (int i = 0; i < MAX_REGIONS; i++) {
        Region *r = &gs->regions[i];
        float healthy = 1.0f - r->infected - r->dead - r->vaccinated;
        if (healthy < 0.0f) healthy = 0.0f;

        float vaccinatedToday = offeredFraction;
        if (vaccinatedToday > healthy) vaccinatedToday = healthy;

        /* Failed doses consume stock but leave their recipients susceptible.
         * Vaccines protect healthy people; they do not directly cure cases. */
        r->vaccinated += vaccinatedToday * c->effectiveness;
        usedUnits += vaccinatedToday / VACCINE_UNIT_SHARE
            * (r->population / totalPop);
    }
    c->vaccineStockpile -= usedUnits;
    if (c->vaccineStockpile < 0.0f) c->vaccineStockpile = 0.0f;

    /* main.c calls virus_refresh_totals after this, once all regions are updated. */
}

/*
 * cure_hire_scientist - Hire a scientist to boost research speed.
 *                       Cost: 100 funding per scientist.
 *                       Effect: +0.20 to the scientist research multiplier per hire.
 *                       Also adds 0.2 vaccine units/day after research.
 */
int cure_hire_scientist(CureState *c)
{
    const float SCIENTIST_COST = 100.0f;
    
    if (c->funding >= SCIENTIST_COST)
    {
        c->funding -= SCIENTIST_COST;
        c->scientistCount++;
        c->productionRate = cure_production_rate(c);
        return 1;
    }
    return 0;
}

/*
 * cure_upgrade_lab - Upgrade research lab to boost research speed.
 *                    Cost: 150 * (level + 1) funding.
 *                    Effect: +0.25 to the laboratory research multiplier per level.
 *                    Max level: 3
 */
int cure_upgrade_lab(CureState *c)
{
    const int MAX_LAB_LEVEL = 3;
    
    if (c->labLevel >= MAX_LAB_LEVEL)
        return 0; /* already at max level */
    
    float cost = 150.0f * (c->labLevel + 1);
    
    if (c->funding >= cost)
    {
        c->funding -= cost;
        c->labLevel++;
        return 1;
    }
    return 0;
}

/*
 * cure_upgrade_production - Upgrade vaccine production facility.
 *                           Cost: 200 * (level + 1) funding.
 *                           Effect: +0.5 vaccine units/day per level.
 *                           Max level: 3
 */
int cure_upgrade_production(CureState *c)
{
    const int MAX_PRODUCTION_LEVEL = 3;
    
    if (c->productionLevel >= MAX_PRODUCTION_LEVEL)
        return 0; /* already at max level */
    
    float cost = 200.0f * (c->productionLevel + 1);
    
    if (c->funding >= cost)
    {
        c->funding -= cost;
        c->productionLevel++;
        c->productionRate = cure_production_rate(c);
        return 1;
    }
    return 0;
}
