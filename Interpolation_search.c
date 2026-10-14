/* interpolation_search.c - step-by-step Interpolation Search demo
 * Build: gcc -Wall -Wextra -o interpolation_search interpolation_search.c   Run: ./interpolation_search */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 20

/* ------------------------------------------------------------------ */
/* Small UI helpers                                                    */
/* ------------------------------------------------------------------ */

static int step_mode = 1; /* 1 = wait for ENTER after each step, 0 = run straight through */

static void line(char c, int len) {
    for (int i = 0; i < len; i++) putchar(c);
    putchar('\n');
}

static void clear_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Reads an int in [lo, hi]; keeps asking until valid. */
static int read_int(const char *prompt, int lo, int hi) {
    int v;
    for (;;) {
        printf("%s", prompt);
        if (scanf("%d", &v) == 1) {
            clear_input();
            if (v >= lo && v <= hi) return v;
        } else {
            if (feof(stdin)) exit(0);
            clear_input();
        }
        printf("  Invalid input. Enter a number between %d and %d.\n", lo, hi);
    }
}

/* Waits for the user between steps (if step mode is on). */
static void pause_step(void) {
    if (!step_mode) return;
    printf("  [Press ENTER for next step, or type 'r' + ENTER to run to the end] ");
    int c = getchar();
    if (c == 'r' || c == 'R') step_mode = 0;
    if (c != '\n' && c != EOF) clear_input();
    printf("\n");
}

/*
 * Prints the array as boxes with an index row.
 * mark1 / mark2 are indices to highlight with a label (-1 = none).
 */
static void print_array(const int a[], int n, int mark1, const char *label1,
                        int mark2, const char *label2) {
    printf("  index: ");
    for (int i = 0; i < n; i++) printf("%5d ", i);
    printf("\n  value: ");
    for (int i = 0; i < n; i++) printf("[%3d] ", a[i]);
    printf("\n         ");
    for (int i = 0; i < n; i++) {
        if (i == mark1 && i == mark2) printf(" ^^^^ ");
        else if (i == mark1 || i == mark2) printf("  ^   ");
        else printf("      ");
    }
    printf("\n");
    if (mark1 >= 0) printf("         ^ at index %d = %s\n", mark1, label1);
    if (mark2 >= 0 && mark2 != mark1) printf("         ^ at index %d = %s\n", mark2, label2);
}

/* Lets the user type an array, or use a built-in sample. */
static int read_array(int a[], int need_sorted) {
    printf("\nInput options:\n");
    printf("  [1] Enter my own array\n");
    printf("  [2] Use sample array %s\n",
           need_sorted ? "{10, 20, 30, 40, 50}" : "{5, 2, 4, 1, 3}");
    int choice = read_int("Choice: ", 1, 2);

    int n;
    if (choice == 2) {
        int sample_sort[] = {5, 2, 4, 1, 3};
        int sample_srch[] = {10, 20, 30, 40, 50};
        n = 5;
        memcpy(a, need_sorted ? sample_srch : sample_sort, n * sizeof(int));
        return n;
    }

    char prompt[64];
    snprintf(prompt, sizeof prompt, "How many elements (1-%d)? ", MAX_N);
    n = read_int(prompt, 1, MAX_N);

    for (;;) {
        printf("Enter %d integers separated by spaces: ", n);
        int ok = 1;
        for (int i = 0; i < n; i++) {
            if (scanf("%d", &a[i]) != 1) { ok = 0; break; }
        }
        clear_input();
        if (feof(stdin)) exit(0);
        if (ok && need_sorted) {
            for (int i = 1; i < n; i++) {
                if (a[i] < a[i - 1]) {
                    printf("  Interpolation search needs SORTED (ascending) data.\n");
                    ok = 0;
                    break;
                }
            }
        }
        if (ok) return n;
        printf("  Please try again.\n");
    }
}

static void interpolation_search_demo(void) {
    int a[MAX_N];
    int n = read_array(a, 1);
    int x;
    printf("Enter the target value to search for: ");
    while (scanf("%d", &x) != 1) {
        if (feof(stdin)) exit(0);
        clear_input();
        printf("  Not a number. Enter the target value: ");
    }
    clear_input();

    printf("\n");
    line('=', 70);
    printf("INTERPOLATION SEARCH  -  step-by-step trace   (target = %d)\n", x);
    line('=', 70);
    printf("Formula: pos = low + ((x - A[low]) * (high - low)) / (A[high] - A[low])\n");
    printf("It guesses WHERE the target should be from its value, instead of\n");
    printf("always splitting in half like binary search.\n\n");
    printf("[ARRAY] (indices 0..%d)\n", n - 1);
    print_array(a, n, -1, "", -1, "");
    printf("\n");

    int low = 0, high = n - 1, probes = 0, found = -1;

    while (low <= high && x >= a[low] && x <= a[high]) {
        probes++;
        printf("[PROBE %d] low = %d (val: %d), high = %d (val: %d)\n",
               probes, low, a[low], high, a[high]);

        int pos;
        if (a[high] == a[low]) {
            pos = low;
            printf("  -> A[low] == A[high], cannot divide. pos = low = %d\n", pos);
        } else {
            /* use long long so (x - A[low]) * (high - low) cannot overflow */
            long long num = (long long)(x - a[low]) * (high - low);
            long long den = (long long)(a[high] - a[low]);
            pos = low + (int)(num / den);
            printf("  -> Evaluating pos formula:\n");
            printf("     pos = %d + ((%d - %d) * (%d - %d)) / (%d - %d)\n",
                   low, x, a[low], high, low, a[high], a[low]);
            printf("     pos = %d + (%lld) / %lld = %d\n", low, num, den, pos);
        }

        print_array(a, n, pos, "pos (probe)", -1, "");
        printf("  -> Checking A[%d] == %d ... A[%d] is %d\n", pos, x, pos, a[pos]);

        if (a[pos] == x) {
            printf("  MATCH FOUND!\n");
            found = pos;
            pause_step();
            break;
        } else if (a[pos] < x) {
            printf("  %d < %d  -> target is to the RIGHT. low = %d\n", a[pos], x, pos + 1);
            low = pos + 1;
        } else {
            printf("  %d > %d  -> target is to the LEFT. high = %d\n", a[pos], x, pos - 1);
            high = pos - 1;
        }
        pause_step();
    }

    line('-', 70);
    if (found >= 0)
        printf("[SUCCESS] Target %d located at index %d in %d probe%s.\n",
               x, found, probes, probes == 1 ? "" : "s");
    else
        printf("[NOT FOUND] Target %d is not in the array (%d probe%s used).\n",
               x, probes, probes == 1 ? "" : "s");
    printf("  Extra space: O(1)   Time: O(log log N) average on uniform data,\n");
    printf("  O(N) worst case on skewed data.\n");
    line('-', 70);
}

int main(void) {
    interpolation_search_demo();
    return 0;
}