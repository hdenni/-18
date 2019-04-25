#include <stdio.h>
#include <stdlib.h>

typedef struct node* treePointer;
typedef struct node treeNode;
typedef struct node {
	int data;
	treePointer leftChild, rightChild;
};

treePointer top;

void inorder(treePointer ptr);
void preorder(treePointer ptr);
void postorder(treePointer ptr);

treePointer getNode(int data);
void insertBST(int data);

int main()
{
	FILE* fp = fopen("Data.txt", "r");
	int data;

	top = NULL;
	while (fscanf(fp, "%d", &data) != EOF) {
		insertBST(data);
	}
	fclose(fp);
	
	printf("Inorder Traversal: ");
	inorder(top);
	printf("\n");

	printf("Preorder Traversal: ");
	preorder(top);
	printf("\n");

	printf("Postorder Treversal: ");
	postorder(top);
	printf("\n");
	
	return 0;
}

treePointer getNode(int data)
{
	treePointer newNode = (treePointer)malloc(sizeof(treeNode));
	newNode->data = data;
	newNode->leftChild = NULL;
	newNode->rightChild = NULL;

	return newNode;
}

void insertBST(int data)
{
	treePointer prev = NULL, corr = NULL, newNode = NULL;
	int flag;

	newNode = getNode(data);

	if (top == NULL) top = newNode;
	else {
		corr = top;
		while (1) {
			if (newNode->data < corr->data) {
				prev = corr;
				corr = corr->leftChild;
				flag = 0;
			}
			// corr->data < newNode
			else {
				prev = corr;
				corr = corr->rightChild;
				flag = 1;
			}
			// insert
			if (corr == NULL) {
				if (flag == 0) prev->leftChild = newNode;
				else prev->rightChild = newNode;
				break;
			}
		}
	}
}


void inorder(treePointer ptr) {
	if (ptr) {
		inorder(ptr->leftChild);
		printf("%d ", ptr->data);
		inorder(ptr->rightChild);
	}
}

void preorder(treePointer ptr) {
	if (ptr) {
		printf("%d ", ptr->data);
		preorder(ptr->leftChild);
		preorder(ptr->rightChild);
	}
}

void postorder(treePointer ptr) {
	if (ptr) {
		postorder(ptr->leftChild);
		postorder(ptr->rightChild);
		printf("%d ", ptr->data);
	}
}