/*
 * main.c - Graphical Visualizer for Cycle Sort & Interpolation Search using raylib
 */

#include "C:/raylib/raylib/src/raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 12
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 650

typedef enum
{
    MODE_CYCLE_SORT,
    MODE_INTERPOLATION_SEARCH
} AlgoMode;

typedef struct
{
    int array[MAX_N];
    int n;
    int target;
    int mark_cycle_start;
    int mark_pos;
    int mark_low;
    int mark_high;
    int mark_found;
    char status_msg[128];
    int comparisons;
    int writes;
    int probes;
    int stage;
    int cycle_start;
    int i_ptr;
    int item;
    int pos;
    int tmp;
    bool is_finished;
} VisualizerState;

static VisualizerState state;
static AlgoMode current_mode = MODE_CYCLE_SORT;
static float frame_timer = 0.0f;
static float step_delay = 0.6f;

void ResetState(void)
{
    state.n = 8;
    state.target = 42;
    state.mark_cycle_start = -1;
    state.mark_pos = -1;
    state.mark_low = -1;
    state.mark_high = -1;
    state.mark_found = -1;
    state.comparisons = 0;
    state.writes = 0;
    state.probes = 0;
    state.stage = 0;
    state.is_finished = false;

    if (current_mode == MODE_CYCLE_SORT)
    {
        int sample[] = {25, 12, 42, 5, 18, 30, 9, 3};
        memcpy(state.array, sample, sizeof(sample));
        state.cycle_start = 0;
        strcpy(state.status_msg, "Cycle Sort Ready. Press SPACE to step, TAB to switch, R to reset.");
    }
    else
    {
        int sample[] = {5, 12, 18, 25, 30, 36, 42, 50};
        memcpy(state.array, sample, sizeof(sample));
        state.mark_low = 0;
        state.mark_high = state.n - 1;
        strcpy(state.status_msg, "Interpolation Search Ready. Looking for target = 42.");
    }
}

void StepCycleSort(void)
{
    if (state.is_finished)
        return;

    if (state.cycle_start <= state.n - 2)
    {
        if (state.stage == 0)
        {
            state.item = state.array[state.cycle_start];
            state.pos = state.cycle_start;
            state.i_ptr = state.cycle_start + 1;
            state.mark_cycle_start = state.cycle_start;
            state.stage = 1;
            sprintf(state.status_msg, "Cycle start at index %d (item = %d). Finding position...", state.cycle_start, state.item);
        }
        else if (state.stage == 1)
        {
            if (state.i_ptr < state.n)
            {
                state.comparisons++;
                if (state.array[state.i_ptr] < state.item)
                {
                    state.pos++;
                }
                state.mark_pos = state.i_ptr;
                state.i_ptr++;
            }
            else
            {
                if (state.pos == state.cycle_start)
                {
                    sprintf(state.status_msg, "Item %d is already in place. Next cycle.", state.item);
                    state.cycle_start++;
                    state.stage = 0;
                    state.mark_pos = -1;
                }
                else
                {
                    while (state.item == state.array[state.pos])
                        state.pos++;
                    state.tmp = state.array[state.pos];
                    state.array[state.pos] = state.item;
                    state.item = state.tmp;
                    state.writes++;
                    state.mark_pos = state.pos;
                    sprintf(state.status_msg, "Writing %d to index %d. Displaced: %d", state.array[state.pos], state.pos, state.item);
                    state.stage = 2;
                }
            }
        }
        else if (state.stage == 2)
        {
            if (state.pos != state.cycle_start)
            {
                state.pos = state.cycle_start;
                for (int j = state.cycle_start + 1; j < state.n; j++)
                {
                    state.comparisons++;
                    if (state.array[j] < state.item)
                        state.pos++;
                }
                while (state.item == state.array[state.pos])
                    state.pos++;

                if (state.item != state.array[state.pos])
                {
                    state.tmp = state.array[state.pos];
                    state.array[state.pos] = state.item;
                    state.item = state.tmp;
                    state.writes++;
                    state.mark_pos = state.pos;
                    sprintf(state.status_msg, "Rotating cycle: writing %d to index %d", state.array[state.pos], state.pos);
                }
                else
                {
                    state.cycle_start++;
                    state.stage = 0;
                    state.mark_pos = -1;
                    state.mark_cycle_start = -1;
                }
            }
            else
            {
                state.cycle_start++;
                state.stage = 0;
                state.mark_pos = -1;
                state.mark_cycle_start = -1;
            }
        }
    }
    else
    {
        state.is_finished = true;
        state.mark_cycle_start = -1;
        state.mark_pos = -1;
        strcpy(state.status_msg, "Cycle Sort Complete! Array is fully sorted.");
    }
}

void StepInterpolationSearch(void)
{
    if (state.is_finished)
        return;

    if (state.mark_low <= state.mark_high && state.target >= state.array[state.mark_low] && state.target <= state.array[state.mark_high])
    {
        state.probes++;
        if (state.array[state.mark_high] == state.array[state.mark_low])
        {
            state.pos = state.mark_low;
        }
        else
        {
            long long num = (long long)(state.target - state.array[state.mark_low]) * (state.mark_high - state.mark_low);
            long long den = (long long)(state.array[state.mark_high] - state.array[state.mark_low]);
            state.pos = state.mark_low + (int)(num / den);
        }

        state.mark_pos = state.pos;
        sprintf(state.status_msg, "Probe %d: Probing index %d (value: %d)", state.probes, state.pos, state.array[state.pos]);

        if (state.array[state.pos] == state.target)
        {
            state.mark_found = state.pos;
            state.is_finished = true;
            sprintf(state.status_msg, "Success! Target %d found at index %d.", state.target, state.pos);
        }
        else if (state.array[state.pos] < state.target)
        {
            state.mark_low = state.pos + 1;
        }
        else
        {
            state.mark_high = state.pos - 1;
        }
    }
    else
    {
        state.is_finished = true;
        sprintf(state.status_msg, "Search Complete: Target %d not found.", state.target);
    }
}

void DrawVisualizer(void)
{
    BeginDrawing();
    ClearBackground((Color){24, 28, 36, 255});

    DrawText(current_mode == MODE_CYCLE_SORT ? "Cycle Sort Visualizer" : "Interpolation Search Visualizer",
             30, 20, 24, RAYWHITE);
    DrawText("Press TAB: Switch Algo | SPACE: Step Forward | R: Reset", 30, 52, 16, GRAY);

    DrawRectangleRounded((Rectangle){580, 20, 290, 90}, 0.1, 4, (Color){38, 44, 56, 255});
    if (current_mode == MODE_CYCLE_SORT)
    {
        DrawText(TextFormat("Comparisons: %d", state.comparisons), 595, 35, 18, LIGHTGRAY);
        DrawText(TextFormat("Memory Writes: %d", state.writes), 595, 65, 18, SKYBLUE);
    }
    else
    {
        DrawText(TextFormat("Target Value: %d", state.target), 595, 35, 18, YELLOW);
        DrawText(TextFormat("Probes Count: %d", state.probes), 595, 65, 18, ORANGE);
    }

    int startX = 50;
    int startY = 340;
    int boxWidth = 70;
    int boxGap = 15;
    int maxHeight = 170;

    for (int i = 0; i < state.n; i++)
    {
        int posX = startX + i * (boxWidth + boxGap);
        int val = state.array[i];
        int barHeight = (val * maxHeight) / 60;
        int posY = startY - barHeight;

        Color boxColor = (Color){52, 63, 82, 255};

        if (i == state.mark_found)
            boxColor = GREEN;
        else if (i == state.mark_pos)
            boxColor = ORANGE;
        else if (i == state.mark_cycle_start)
            boxColor = SKYBLUE;
        else if (current_mode == MODE_INTERPOLATION_SEARCH && i >= state.mark_low && i <= state.mark_high)
        {
            boxColor = (Color){65, 80, 110, 255};
        }

        DrawRectangleRounded((Rectangle){posX, posY, boxWidth, barHeight}, 0.15, 4, boxColor);
        // Fixed: Removed extra argument (2) so it matches the 4 expected arguments correctly[cite: 8]
        DrawRectangleRoundedLines((Rectangle){posX, posY, boxWidth, barHeight}, 0.15, 4, RAYWHITE);

        DrawText(TextFormat("%d", val), posX + boxWidth / 2 - MeasureText(TextFormat("%d", val), 20) / 2,
                 posY + barHeight / 2 - 10, 20, RAYWHITE);
        DrawText(TextFormat("[%d]", i), posX + boxWidth / 2 - MeasureText(TextFormat("[%d]", i), 14) / 2,
                 startY + 15, 14, GRAY);

        if (i == state.mark_cycle_start)
            DrawText("Start", posX + 15, startY + 40, 14, SKYBLUE);
        if (i == state.mark_pos)
            DrawText("Probe", posX + 15, startY + 58, 14, ORANGE);
        if (current_mode == MODE_INTERPOLATION_SEARCH)
        {
            if (i == state.mark_low)
                DrawText("Low", posX + 20, startY + 76, 14, YELLOW);
            if (i == state.mark_high)
                DrawText("High", posX + 15, startY + 76, 14, PURPLE);
        }
    }

    DrawRectangle(30, SCREEN_HEIGHT - 70, SCREEN_WIDTH - 60, 45, (Color){38, 44, 56, 255});
    DrawText(state.status_msg, 45, SCREEN_HEIGHT - 58, 18, GOLD);

    EndDrawing();
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Interactive Algorithm Visualizer");
    SetTargetFPS(60);

    ResetState();

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_TAB))
        {
            current_mode = (current_mode == MODE_CYCLE_SORT) ? MODE_INTERPOLATION_SEARCH : MODE_CYCLE_SORT;
            ResetState();
        }
        if (IsKeyPressed(KEY_R))
        {
            ResetState();
        }

        frame_timer += GetFrameTime();
        if (IsKeyPressed(KEY_SPACE) || frame_timer >= step_delay)
        {
            frame_timer = 0.0f;
            if (!state.is_finished)
            {
                if (current_mode == MODE_CYCLE_SORT)
                    StepCycleSort();
                else
                    StepInterpolationSearch();
            }
        }

        DrawVisualizer();
    }

    CloseWindow();
    return 0;
}