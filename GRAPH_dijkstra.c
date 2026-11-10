
#include <stdio.h>
#include <time.h>

#define MAX 20
#define INF 999999

int graph[MAX][MAX];
int distance[MAX];
int visited[MAX];
int parent[MAX];

int comparisons = 0;
int relaxations = 0;

void printPath(int vertex)
{
    if (vertex == -1)
        return;

    printPath(parent[vertex]);
    printf("%d ", vertex + 1);
}

void displayDistances(int n)
{
    printf("\nVertex\tDistance\tStatus\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d\t", i + 1);

        if (distance[i] == INF)
            printf("INF\t\t");
        else
            printf("%d\t\t", distance[i]);

        printf("%s\n", visited[i] ? "Visited" : "Pending");
    }
}

void dijkstra(int n, int source)
{
    clock_t start = clock();

    for (int i = 0; i < n; i++)
    {
        distance[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    distance[source] = 0;

    printf("\n========== DIJKSTRA VISUALIZATION ==========\n");

    for (int step = 1; step <= n; step++)
    {
        int u = -1;
        int minimum = INF;

        for (int i = 0; i < n; i++)
        {
            comparisons++;

            if (!visited[i] && distance[i] < minimum)
            {
                minimum = distance[i];
                u = i;
            }
        }

        if (u == -1)
            break;

        visited[u] = 1;

        printf("\nSTEP %d\n", step);
        printf("Selected Vertex: %d\n", u + 1);
        printf("Current Distance: %d\n", distance[u]);

        for (int v = 0; v < n; v++)
        {
            if (!visited[v] && graph[u][v] != INF)
            {
                int newDistance = distance[u] + graph[u][v];

                printf("Checking edge %d -> %d\n", u + 1, v + 1);

                if (newDistance < distance[v])
                {
                    distance[v] = newDistance;
                    parent[v] = u;
                    relaxations++;

                    printf("Distance Updated: %d\n", newDistance);
                }
                else
                {
                    printf("No Update Required\n");
                }
            }
        }

        displayDistances(n);
    }

    clock_t end = clock();

    printf("\n========== FINAL RESULTS ==========\n");

    for (int i = 0; i < n; i++)
    {
        printf("\nVertex %d: ", i + 1);

        if (distance[i] == INF)
        {
            printf("No path available\n");
        }
        else
        {
            printf("Shortest Distance = %d\n", distance[i]);

            printf("Shortest Path = ");
            printPath(i);
            printf("\n");
        }
    }

    printf("\n PERFORMANCE ANALYSIS \n");
    printf("Comparisons: %d\n", comparisons);
    printf("Successful Relaxations: %d\n", relaxations);
    printf("Execution Time: %.6f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    printf("Time Complexity: O(V^2)\n");
    printf("Space Complexity: O(V^2)\n");
}

int main()
{
    int n, e, directed, source;
    printf("     DIJKSTRA GRAPH ALGORITHM\n");

    printf("Enter number of vertices (1-%d): ", MAX);
    scanf("%d", &n);

    if (n < 1 || n > MAX)
    {
        printf("Invalid number of vertices.\n");
        return 1;
    }

    printf("Enter number of edges: ");
    scanf("%d", &e);

    if (e < 0 || e > n * (n - 1))
    {
        printf("Invalid number of edges.\n");
        return 1;
    }

    printf("Enter graph type (1=Directed, 0=Undirected): ");
    scanf("%d", &directed);

    if (directed != 0 && directed != 1)
    {
        printf("Invalid graph type.\n");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            graph[i][j] = (i == j) ? 0 : INF;
        }
    }

    printf("\nEnter edges in format: Source Destination Weight\n");

    for (int i = 0; i < e; i++)
    {
        int u, v, w;

        printf("Edge %d: ", i + 1);
        scanf("%d %d %d", &u, &v, &w);

        if (u < 1 || u > n || v < 1 || v > n ||
            u == v || w < 0 || w >= INF)
        {
            printf("Invalid edge. Try again.\n");
            i--;
            continue;
        }

        if (w < graph[u - 1][v - 1])
        {
            graph[u - 1][v - 1] = w;
        }

        if (!directed && w < graph[v - 1][u - 1])
        {
            graph[v - 1][u - 1] = w;
        }
    }

    printf("\nEnter starting vertex (1-%d): ", n);
    scanf("%d", &source);

    if (source < 1 || source > n)
    {
        printf("Invalid starting vertex.\n");
        return 1;
    }

    printf("\nStarting Dijkstra's Algorithm...\n");

    dijkstra(n, source - 1);

    return 0;
}
