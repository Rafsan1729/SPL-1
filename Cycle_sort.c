/* cycle_sort.c - step-by-step Cycle Sort demo
 * Build: gcc -Wall -Wextra -o cycle_sort cycle_sort.c   Run: ./cycle_sort */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 20

/* ------------------------------------------------------------------ */
/* Small UI helpers                                                    */
/* ------------------------------------------------------------------ */

static int step_mode = 1; /* 1 = wait for ENTER after each step, 0 = run straight through */

static void line(char c, int len)
{
    for (int i = 0; i < len; i++)
        putchar(c);
    putchar('\n');
}

static void clear_input(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

/* Reads an int in [lo, hi]; keeps asking until valid. */
static int read_int(const char *prompt, int lo, int hi)
{
    int v;
    for (;;)
    {
        printf("%s", prompt);
        if (scanf("%d", &v) == 1)
        {
            clear_input();
            if (v >= lo && v <= hi)
                return v;
        }
        else
        {
            if (feof(stdin))
                exit(0);
            clear_input();
        }
        printf("  Invalid input. Enter a number between %d and %d.\n", lo, hi);
    }
}

/* Waits for the user between steps (if step mode is on). */
static void pause_step(void)
{
    if (!step_mode)
        return;
    printf("  [Press ENTER for next step, or type 'r' + ENTER to run to the end] ");
    int c = getchar();
    if (c == 'r' || c == 'R')
        step_mode = 0;
    if (c != '\n' && c != EOF)
        clear_input();
    printf("\n");
}

/*
 * Prints the array as boxes with an index row.
 * mark1 / mark2 are indices to highlight with a label (-1 = none).
 */
static void print_array(const int a[], int n, int mark1, const char *label1,
                        int mark2, const char *label2)
{
    printf("  index: ");
    for (int i = 0; i < n; i++)
        printf("%5d ", i);
    printf("\n  value: ");
    for (int i = 0; i < n; i++)
        printf("[%3d] ", a[i]);
    printf("\n         ");
    for (int i = 0; i < n; i++)
    {
        if (i == mark1 && i == mark2)
            printf(" ^^^^ ");
        else if (i == mark1 || i == mark2)
            printf("  ^   ");
        else
            printf("      ");
    }
    printf("\n");
    if (mark1 >= 0)
        printf("         ^ at index %d = %s\n", mark1, label1);
    if (mark2 >= 0 && mark2 != mark1)
        printf("         ^ at index %d = %s\n", mark2, label2);
}

/* Lets the user type an array, or use a built-in sample. */
static int read_array(int a[], int need_sorted)
{
    printf("\nInput options:\n");
    printf("  [1] Enter my own array\n");
    printf("  [2] Use sample array %s\n",
           need_sorted ? "{10, 20, 30, 40, 50}" : "{5, 2, 4, 1, 3}");
    int choice = read_int("Choice: ", 1, 2);

    int n;
    if (choice == 2)
    {
        int sample_sort[] = {5, 2, 4, 1, 3};
        int sample_srch[] = {10, 20, 30, 40, 50};
        n = 5;
        memcpy(a, need_sorted ? sample_srch : sample_sort, n * sizeof(int));
        return n;
    }

    char prompt[64];
    snprintf(prompt, sizeof prompt, "How many elements (1-%d)? ", MAX_N);
    n = read_int(prompt, 1, MAX_N);

    for (;;)
    {
        printf("Enter %d integers separated by spaces: ", n);
        int ok = 1;
        for (int i = 0; i < n; i++)
        {
            if (scanf("%d", &a[i]) != 1)
            {
                ok = 0;
                break;
            }
        }
        clear_input();
        if (feof(stdin))
            exit(0);
        if (ok && need_sorted)
        {
            for (int i = 1; i < n; i++)
            {
                if (a[i] < a[i - 1])
                {
                    printf("  Interpolation search needs SORTED (ascending) data.\n");
                    ok = 0;
                    break;
                }
            }
        }
        if (ok)
            return n;
        printf("  Please try again.\n");
    }
}

static void cycle_sort_demo(void)
{
    int a[MAX_N];
    int n = read_array(a, 0);

    printf("\n");
    line('=', 70);
    printf("CYCLE SORT  -  step-by-step trace   (N = %d)\n", n);
    line('=', 70);
    printf("Idea: for each item, count how many elements are smaller. That count\n");
    printf("is its final position. Rotate each cycle so every item is written\n");
    printf("to memory at most once -> minimum number of writes.\n\n");
    printf("[START]\n");
    print_array(a, n, -1, "", -1, "");
    printf("\n");

    int writes = 0, comparisons = 0, step = 0;

    for (int cycle_start = 0; cycle_start <= n - 2; cycle_start++)
    {
        int item = a[cycle_start];

        /* Step A: find where 'item' belongs */
        int pos = cycle_start;
        for (int i = cycle_start + 1; i < n; i++)
        {
            comparisons++;
            if (a[i] < item)
                pos++;
        }

        printf("--- Cycle starting at index %d (item = %d) ---\n", cycle_start, item);
        printf("[STEP %d] Count elements smaller than %d to the right of index %d -> pos = %d\n",
               ++step, item, cycle_start, pos);

        if (pos == cycle_start)
        {
            printf("         %d is already in its correct place. No write needed.\n", item);
            print_array(a, n, cycle_start, "already in place", -1, "");
            pause_step();
            continue;
        }

        /* Skip duplicates */
        while (item == a[pos])
            pos++;

        /* Put the item in its right place */
        int tmp = a[pos];
        a[pos] = item;
        item = tmp;
        writes++;
        printf("[STEP %d] Write %d into index %d, pick up displaced value %d   (writes = %d)\n",
               ++step, a[pos], pos, item, writes);
        print_array(a, n, pos, "just written", cycle_start, "cycle start");
        pause_step();

        /* Rotate the rest of the cycle */
        while (pos != cycle_start)
        {
            pos = cycle_start;
            for (int i = cycle_start + 1; i < n; i++)
            {
                comparisons++;
                if (a[i] < item)
                    pos++;
            }
            while (item == a[pos])
                pos++;

            if (item != a[pos])
            {
                tmp = a[pos];
                a[pos] = item;
                item = tmp;
                writes++;
                printf("[STEP %d] Item belongs at index %d. Write %d there, pick up %d   (writes = %d)\n",
                       ++step, pos, a[pos], item, writes);
                print_array(a, n, pos, "just written", cycle_start, "cycle start");
                pause_step();
            }
        }
        printf("         Cycle closed. Array now: ");
        for (int i = 0; i < n; i++)
            printf("%d ", a[i]);
        printf("\n\n");
    }

    line('-', 70);
    printf("[FINAL] Sorted array:\n");
    print_array(a, n, -1, "", -1, "");
    printf("\n  Total writes      : %d\n",
           writes);
    printf("  Total comparisons : %d\n", comparisons);
    printf("  Extra space       : O(1)      Time: O(N^2)\n");
    line('-', 70);
}

int main(void)
{
    cycle_sort_demo();
    return 0;
}