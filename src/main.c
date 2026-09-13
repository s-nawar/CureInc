#include "raylib.h"
#include "types.h"
#include "virus.h"
#include "region.h"
#include "cure.h"
#include "ui.h"
#include "events.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Check win/lose conditions - Balanced for challenging but fair gameplay */
static void check_win_lose(GameState *gs)
{
    int collapsed = 0;
    for (int i = 0; i < MAX_REGIONS; i++)
        if (gs->regions[i].overloadedDays >= 25) collapsed++;  /* Increased from 20 to give more time */

    if (gs->virus.globalDead >= 0.35f) {  /* Increased from 0.30 to be less punishing */
        gs->screen = SCREEN_LOSE;
        gs->endReason = "Deaths exceeded 35% of the population!";
    } else if (collapsed >= 6) {
        gs->screen = SCREEN_LOSE;
        gs->endReason = "Healthcare system collapsed in 6+ regions!";
    } else {
        float living = 1.0f - gs->virus.globalDead;
        if (living > 0.0f && gs->cure.phase == PHASE_DISTRIBUTION &&
            gs->cure.globalDistributed >= 0.90f &&  /* Reduced from 0.95 to be more achievable */
            gs->virus.globalInfected / living < 0.03f) {  /* Increased from 0.02 to be more forgiving */
            gs->screen = SCREEN_WIN;
            gs->endReason = "Pandemic defeated! 90% vaccinated, infection below 3%!";
        }
    }
}

/* Day tick - runs once per game day */
static void day_tick(GameState *gs)
{
    if (gs->day % 7 == 0) events_trigger_random(gs);  /* Changed back to 7 days for less chaos */

    if (virus_try_mutate(&gs->virus, gs->day)) {
        gs->cure.stability -= 0.03f;  /* Reduced from 0.05 so mutations are less devastating */
        if (gs->cure.stability < 0.60f) gs->cure.stability = 0.60f;  /* Increased floor from 0.50 */
        events_add(gs, "Virus Mutated", virus_trait_name(gs->virus.lastMutation));
    }

    virus_update(gs);
    cure_update(gs, 1.0f);
    virus_refresh_totals(gs);
    region_update_states(gs);
    check_win_lose(gs);
}

/* Reset game state */
static void reset_game(GameState *gs)
{
    GameScreen keepScreen = gs->screen;
    *gs = (GameState){0};
    gs->screen = keepScreen;
    gs->dayLength = DEFAULT_DAY_LENGTH;
    gs->gameSpeed = 1;
    gs->selectedRegionIndex = 2;

    virus_init(&gs->virus);
    region_init(gs);
    cure_init(&gs->cure);
    events_init(gs);
    virus_refresh_totals(gs);
    region_update_states(gs);
    UI_ResetGameplayState();
}

/* Main function */
int main(void)
{
    srand((unsigned int)time(NULL));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cure Inc.");
    SetTargetFPS(60);
    InitUI();

    GameState state = {0};
    state.screen = SCREEN_MENU;
    state.selectedRegionIndex = 2;
    Rectangle regionNode = { 400, 300, 200, 40 };

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime() * state.gameSpeed;

        if (state.screen == SCREEN_GAME) {
            state.dayTimer += dt;
            if (state.dayTimer >= state.dayLength) {
                state.dayTimer -= state.dayLength;
                state.day++;
                day_tick(&state);
            }
            events_update(&state, GetFrameTime());
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        if (state.screen == SCREEN_MENU) {
            UIAction action = UI_DrawMainMenu(state.screen);
            if (action == UI_START_GAME) {
                state.screen = SCREEN_GAME;
                reset_game(&state);
            } else if (action == UI_EXIT) {
                break;
            }
        } else if (state.screen == SCREEN_GAME || state.screen == SCREEN_PAUSED) {
            UI_DrawGameplay(&state, regionNode);
        } else if (state.screen == SCREEN_WIN || state.screen == SCREEN_LOSE) {
            UIAction action = UI_DrawEndScreen(&state);
            if (action == UI_MAIN_MENU) state.screen = SCREEN_MENU;
        }
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}