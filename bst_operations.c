
#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

Node *createNode(int value)
{
    Node *newNode = malloc(sizeof(Node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

Node *insertNode(Node *root, int value, int *inserted)
{
    if (root == NULL)
    {
        *inserted = 1;
        return createNode(value);
    }

    if (value < root->data)
    {
        root->left = insertNode(root->left, value, inserted);
    }
    else if (value > root->data)
    {
        root->right = insertNode(root->right, value, inserted);
    }
    else
    {
        *inserted = 0;
    }

    return root;
}

Node *searchNode(Node *root, int value, int *comparisons)
{
    if (root == NULL)
        return NULL;

    (*comparisons)++;

    if (root->data == value)
        return root;

    if (value < root->data)
        return searchNode(root->left, value, comparisons);

    return searchNode(root->right, value, comparisons);
}

Node *findMin(Node *root)
{
    while (root != NULL && root->left != NULL)
        root = root->left;

    return root;
}

Node *deleteNode(Node *root, int value, int *deleted)
{
    if (root == NULL)
        return NULL;

    if (value < root->data)
    {
        root->left = deleteNode(root->left, value, deleted);
    }
    else if (value > root->data)
    {
        root->right = deleteNode(root->right, value, deleted);
    }
    else
    {
        *deleted = 1;

        if (root->left == NULL && root->right == NULL)
        {
            free(root);
            return NULL;
        }

        if (root->left == NULL)
        {
            Node *temp = root->right;
            free(root);
            return temp;
        }

        if (root->right == NULL)
        {
            Node *temp = root->left;
            free(root);
            return temp;
        }

        Node *temp = findMin(root->right);
        root->data = temp->data;

        int dummy = 0;
        root->right = deleteNode(root->right, temp->data, &dummy);
    }

    return root;
}

void inorder(Node *root)
{
    if (root == NULL)
        return;

    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

void preorder(Node *root)
{
    if (root == NULL)
        return;

    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void postorder(Node *root)
{
    if (root == NULL)
        return;

    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

void freeTree(Node *root)
{
    if (root == NULL)
        return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
