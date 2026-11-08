
#ifndef BST_H
#define BST_H

typedef struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(int value);
Node *insertNode(Node *root, int value, int *inserted);
Node *searchNode(Node *root, int value, int *comparisons);
Node *deleteNode(Node *root, int value, int *deleted);
Node *findMin(Node *root);

void inorder(Node *root);
void preorder(Node *root);
void postorder(Node *root);

void displayTree(Node *root, int space);
int treeHeight(Node *root);
int countNodes(Node *root);
void freeTree(Node *root);

#endif
