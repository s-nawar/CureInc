#ifndef CURE_H
#define CURE_H
#include "types.h"

void cure_init(CureState *c);
void cure_update(GameState *gs, float dtDays);

float cure_research_rate(const GameState *gs);
float cure_production_rate(const CureState *c);
float cure_phase_progress(const CureState *c);

/* Player decision functions */
int  cure_hire_scientist(CureState *c);     /* returns 1 on success, 0 if insufficient funds */
int  cure_upgrade_lab(CureState *c);        /* returns 1 on success, 0 if insufficient funds or max level */
int  cure_upgrade_production(CureState *c); /* returns 1 on success, 0 if insufficient funds or max level */

#endif