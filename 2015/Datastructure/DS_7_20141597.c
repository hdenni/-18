#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define MAX_ELEMENTS 201
#define HEAP_FULL(n) (n==MAX_ELEMENTS-1)
#define HEAP_EMPTY(n) (!n)

typedef struct {
	int key;
} element;

element heap[MAX_ELEMENTS];
int n;		/* heap size */

void insert_min_heap(element item);
element delete_min_heap();
void heapsort(int size);
void adjust(int root, int n);
void print_min_heap();

int main()
{
	int menu;
	element item;
	n = 0;
	while (1)
	{
		printf("---------------------------------------------------------\n");
		printf("-  1. Insert an element into the heap\t\t\t-\n");
		printf("-  2. Remove the key with lowest value\t\t\t-\n");
		printf("-  3. Change the priority of an arbitrary element\t-\n");
		printf("-  4. Print the status of a heap\t\t\t-\n");
		printf("-  5. Quit\t\t\t\t\t\t-\n");
		printf("---------------------------------------------------------\n");
		scanf("%d", &menu);

		switch (menu)
		{
		case 1: 
			printf("삽입할 key값\n");
			scanf("%d", &item.key);
			insert_min_heap(item);
			break;
		case 2:
			item = delete_min_heap();
			printf("'%d'를 삭제하였습니다.\n", item.key);
			break;
		case 3:
			heapsort(n);
			break;
		case 4:
			print_min_heap();
			break;
		case 5:
			return 0;
		default: 
			printf("없는 메뉴 번호입니다.\n"); 
			break;
		}
		printf("아무키나 누르면 메인 메뉴로 이동합니다.\n");
		getch();
	}
	return 0;
}

void insert_min_heap(element item) {
	int i;
	if (HEAP_FULL(n)) {
		fprintf(stderr, "Heap이 가득 찼습니다.\n");
		exit(1);
	}

	i = ++n;
	while ((i != 1) && (item.key < heap[i / 2].key)) {
		heap[i] = heap[i / 2];
		i /= 2;
	}
	heap[i] = item;
}
element delete_min_heap() {
	int parent, child;
	element item, temp;
	if (HEAP_EMPTY(n)) {
		fprintf(stderr, "Heap이 비어있습니다.\n");
		exit(1);
	}

	item = heap[1];
	temp = heap[n--];
	parent = 1;		child = 2;

	while (child <= n) {
		if ((child < n) && (heap[child].key > heap[child + 1].key)) child++;
		if (temp.key <= heap[child].key) break;
		heap[parent] = heap[child];
		parent = child;		child *= 2;
	}

	heap[parent] = temp;
	return item;
}
void heapsort(int size){
	int i;
	element temp;

	for (i = size / 2; i > 0; i--)
		adjust(i, size);
	for (i = size - 1; i > 0; i--)
	{
		temp = heap[i + 1];
		heap[i + 1] = heap[1];
		heap[1] = temp;

		adjust(1, i);
	}		
}

void adjust(int root, int n)
{
	int child, rootkey;
	element temp;

	temp = heap[root];
	rootkey = heap[root].key;

	child = 2 * root;
	while (child <= n) {
		if ((child < n) && (heap[child].key < heap[child + 1].key)) child++;
		if (rootkey > heap[child].key) break;
		else {
			heap[child / 2] = heap[child];
			child *= 2;
		}
	}
	heap[child / 2] = temp;
}
void print_min_heap(){
	int i;
	for (i = 1; i <= n; i++)
		printf("%d 번째: %d\n", i, heap[i]);
}