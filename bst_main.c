
#include <stdio.h>
#include "bst.h"

int main()
{
    Node *root = NULL;
    int choice, value;

    printf("====================================\n");
    printf("     BINARY SEARCH TREE VISUALIZER\n");
    printf("====================================\n");

    while (1)
    {
        printf("\n========== MAIN MENU ==========\n");
        printf("1. Insert a Node\n");
        printf("2. Search a Node\n");
        printf("3. Delete a Node\n");
        printf("4. Display BST\n");
        printf("5. Inorder Traversal\n");
        printf("6. Preorder Traversal\n");
        printf("7. Postorder Traversal\n");
        printf("8. Tree Information\n");
        printf("9. Exit\n");

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            break;
        }

        switch (choice)
        {
        case 1:
        {
            int inserted = 0;

            printf("Enter value to insert: ");

            if (scanf("%d", &value) != 1)
            {
                printf("Invalid input!\n");
                freeTree(root);
                return 1;
            }

            root = insertNode(root, value, &inserted);

            if (inserted)
                printf("Node %d inserted successfully!\n", value);
            else
                printf("Duplicate values are not allowed!\n");

            printf("\nUpdated BST:\n");
            displayTree(root, 0);
            printf("\n");
            break;
        }

        case 2:
        {
            int comparisons = 0;

            printf("Enter value to search: ");

            if (scanf("%d", &value) != 1)
            {
                printf("Invalid input!\n");
                freeTree(root);
                return 1;
            }

            Node *result = searchNode(root, value, &comparisons);

            if (result != NULL)
                printf("Node %d found!\n", value);
            else
                printf("Node %d not found!\n", value);

            printf("Comparisons performed: %d\n", comparisons);
            break;
        }

        case 3:
        {
            int deleted = 0;

            printf("Enter value to delete: ");

            if (scanf("%d", &value) != 1)
            {
                printf("Invalid input!\n");
                freeTree(root);
                return 1;
            }

            root = deleteNode(root, value, &deleted);

            if (deleted)
                printf("Node %d deleted successfully!\n", value);
            else
                printf("Node not found!\n");

            printf("\nUpdated BST:\n");
            displayTree(root, 0);
            printf("\n");
            break;
        }

        case 4:
            if (root == NULL)
                printf("Tree is empty!\n");
            else
            {
                printf("\nBST Visualization:\n");
                displayTree(root, 0);
                printf("\n");
            }
            break;

        case 5:
            printf("Inorder Traversal: ");
            inorder(root);
            printf("\n");
            break;

        case 6:
            printf("Preorder Traversal: ");
            preorder(root);
            printf("\n");
            break;

        case 7:
            printf("Postorder Traversal: ");
            postorder(root);
            printf("\n");
            break;

        case 8:
            printf("\nTotal Nodes: %d\n", countNodes(root));
            printf("Tree Height: %d\n", treeHeight(root));
            printf("Average Search/Insert/Delete: O(log n)\n");
            printf("Worst Case Search/Insert/Delete: O(n)\n");
            printf("Traversal Complexity: O(n)\n");
            break;

        case 9:
            freeTree(root);
            printf("Memory released successfully!\n");
            printf("Thank you for using BST Visualizer!\n");
            return 0;

        default:
            printf("Invalid choice! Try again.\n");
        }
    }

    freeTree(root);
    return 0;
}
