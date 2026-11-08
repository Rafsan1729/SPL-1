#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VALUES 50

int readNumber(const char *message, int *number)
{
    char line[100];
    char *end;
    long value;

    while (1)
    {
        printf("%s", message);

        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            return 0;
        }

        errno = 0;
        value = strtol(line, &end, 10);

        if (end == line || errno == ERANGE ||
            value < INT_MIN || value > INT_MAX)
        {
            puts("Enter a valid whole number.");
            continue;
        }

        while (*end == ' ' || *end == '\t' ||
               *end == '\n' || *end == '\r')
        {
            end++;
        }

        if (*end != '\0')
        {
            puts("Enter one whole number only.");
            continue;
        }

        *number = (int)value;
        return 1;
    }
}

void showArray(int values[], int count, int sortedFrom, int prefixLength)
{
    int i;

    printf("\n             ");
    for (i = 0; i < count; i++)
    {
        printf("+-----------+ ");
    }

    printf("\nIndex:       ");
    for (i = 0; i < count; i++)
    {
        printf("|%11d| ", i);
    }

    printf("\nValues:      ");
    for (i = 0; i < count; i++)
    {
        printf("|%11d| ", values[i]);
    }

    printf("\n             ");
    for (i = 0; i < count; i++)
    {
        printf("+-----------+ ");
    }

    printf("\n             ");
    for (i = 0; i < count; i++)
    {
        if (i < prefixLength)
        {
            printf("^^^^^^^^^^^^^ ");
        }
        else
        {
            printf("              ");
        }
    }

    if (prefixLength > 0)
    {
        printf("\n             ^ prefix to reverse");
    }

    if (sortedFrom < count)
    {
        printf("\nSorted suffix: ");

        for (i = 0; i < sortedFrom; i++)
        {
            printf("              ");
        }

        for (i = sortedFrom; i < count; i++)
        {
            printf("|  SORTED   | ");
        }
    }

    printf("\n");
}

int findLargest(int values[], int size, int *comparisons)
{
    int largest = 0;
    int i;

    for (i = 1; i < size; i++)
    {
        (*comparisons)++;

        printf("Compare %d with %d: ",
               values[largest], values[i]);

        if (values[i] > values[largest])
        {
            largest = i;
            puts("new largest value");
        }
        else
        {
            puts("largest value stays the same");
        }
    }

    return largest;
}

void flipPrefix(int values[], int length, int count,
                int sortedBefore, int sortedAfter,
                int *flips, int *swaps)
{
    int left = 0;
    int right = length - 1;

    (*flips)++;

    printf("\nFlip the first %d value%s:\n",
           length, length == 1 ? "" : "s");

    showArray(values, count, sortedBefore, length);

    while (left < right)
    {
        int temporary = values[left];

        values[left] = values[right];
        values[right] = temporary;

        (*swaps)++;
        left++;
        right--;
    }

    showArray(values, count, sortedAfter, 0);
}

void pancakeSort(int values[], int count)
{
    int comparisons = 0;
    int flips = 0;
    int swaps = 0;
    int activeSize;

    puts("\nStarting array:");
    showArray(values, count, count, 0);

    for (activeSize = count; activeSize > 1; activeSize--)
    {
        int largestIndex;
        int sortedFrom = activeSize;
        char line[10];

        printf("\n--- Pass %d: sort the first %d values ---\n",
               count - activeSize + 1, activeSize);

        largestIndex = findLargest(values, activeSize, &comparisons);

        printf("Largest value is %d at index %d.\n",
               values[largestIndex], largestIndex);

        if (largestIndex == activeSize - 1)
        {
            puts("It is already in the correct position.");
        }
        else
        {
            if (largestIndex > 0)
            {
                puts("Flip it to the front of the active part.");

                flipPrefix(values, largestIndex + 1, count,
                           sortedFrom, sortedFrom,
                           &flips, &swaps);
            }

            puts("Flip the active part to place its largest value at the end.");

            flipPrefix(values, activeSize, count,
                       activeSize, activeSize - 1,
                       &flips, &swaps);
        }

        printf("Pass complete. The last %d value%s %s sorted.\n",
               count - activeSize + 1,
               count - activeSize + 1 == 1 ? "" : "s",
               count - activeSize + 1 == 1 ? "is" : "are");

        printf("Press Enter for the next pass...");
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            return;
        }
    }

    puts("\nSorting complete!");
    showArray(values, count, count, 0);

    printf("Comparisons: %d\n", comparisons);
    printf("Prefix flips: %d\n", flips);
    printf("Swaps inside flips: %d\n", swaps);

    puts("Time complexity (worst case): O(n^2)");
    puts("Extra space: O(1)");
    puts("Operation counts are for this run; they are not benchmark timings.");
}

void explainPancakeSort(void)
{
    puts("\nPancake Sort uses prefix reversals:");
    puts("1. Find the largest value in the unsorted part.");
    puts("2. Flip it to the front if needed.");
    puts("3. Flip the unsorted part to move it into its final place.");
    puts("4. Repeat with a smaller unsorted part.");
}

int main(void)
{
    int choice;

    puts("====================================");
    puts("     PANCAKE SORT LEARNING TOOL");
    puts("====================================");

    while (1)
    {
        puts("\n1. Run the five-value demo");
        puts("2. Enter my own values");
        puts("3. Learn the algorithm");
        puts("0. Exit");

        if (!readNumber("Choose an option: ", &choice))
        {
            break;
        }

        if (choice == 0)
        {
            puts("Goodbye!");
            break;
        }
        else if (choice == 1)
        {
            int demo[] = {3, 1, 2, 4, 5};
            pancakeSort(demo, 5);
        }
        else if (choice == 2)
        {
            int values[MAX_VALUES];
            int count;
            int i;
            char prompt[50];

            if (!readNumber("Number of values (2-50): ", &count))
            {
                break;
            }

            if (count < 2 || count > MAX_VALUES)
            {
                puts("Please enter a number from 2 to 50.");
                continue;
            }

            for (i = 0; i < count; i++)
            {
                snprintf(prompt, sizeof(prompt), "Value %d: ", i + 1);

                if (!readNumber(prompt, &values[i]))
                {
                    return 0;
                }
            }

            pancakeSort(values, count);
        }
        else if (choice == 3)
        {
            explainPancakeSort();
        }
        else
        {
            puts("Choose 0, 1, 2, or 3.");
        }
    }

    return 0;
}