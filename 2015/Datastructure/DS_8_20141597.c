#include <stdio.h>
#include <stdlib.h>

typedef struct node* treePointer;
typedef struct node {
	int data;
	treePointer left, right;
}treeNode;

treePointer getNode(int data) {
	treePointer newNode = (treePointer)malloc(sizeof(treeNode));
	newNode->data = data;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

void insert(int data, treePointer* top);
int checkBST(treePointer node);

int main()
{
	FILE* fp = fopen("data.txt", "r");
	int data;
	treePointer top = NULL;

	top = NULL;
	while (fscanf(fp, "%d", &data) != EOF) {
		insert(data, &top);
	}
	fclose(fp);

	if (checkBST(top)) printf("This Tree Is a BST\n");
	else printf("This Tree Is Not a BST\n");

}


void insert(int data, treePointer* top)
{
	if ((*top) == NULL) (*top) = getNode(data);
	else if ((*top)->left == NULL) (*top)->left = getNode(data);
	else if ((*top)->right == NULL) (*top)->right = getNode(data);
	else
	{
		if ((*top)->left->right == NULL) insert(data, &((*top)->left));
		else if ((*top)->right->right == NULL) insert(data, &((*top)->right));
		else insert(data, &((*top)->left));
	}
}

int checkBST(treePointer node) {
	if (node == NULL) return 1;

	if (node->left != NULL && node->left->data > node->data)
		return 0;

	if (node->right != NULL && node->right->data < node->data)
		return 0;

	if (!checkBST(node->left) || !checkBST(node->right))
		return 0;

	return 1;
}