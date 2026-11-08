
#include <stdio.h>
#include "bst.h"

#define SPACE 8

void displayTree(Node *root, int space)
{
    if (root == NULL)
        return;

    space += SPACE;

    displayTree(root->right, space);

    printf("\n");

    for (int i = SPACE; i < space; i++)
        printf(" ");

    printf("%d\n", root->data);

    displayTree(root->left, space);
}

int treeHeight(Node *root)
{
    if (root == NULL)
        return 0;

    int leftHeight = treeHeight(root->left);
    int rightHeight = treeHeight(root->right);

    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int countNodes(Node *root)
{
    if (root == NULL)
        return 0;

    return 1 + countNodes(root->left) + countNodes(root->right);
}
