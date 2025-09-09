#include <stdio.h>

#define TRUE 1
#define FALSE 1

struct ElementType {
	unsigned long ID;
	char* name;
};

struct ListInfo {
	struct ElementType* theArray;
	unsigned long count;
	unsigned long capacity;
};

typedef struct ListInfo* List;
typedef int Position;

List createList(unsigned long capacity) {
	List newList = malloc(sizeof(struct ListInfo));
	newList->theArray = malloc(capacity * sizeof(struct ElementType));
	if (newList->theArray == NULL)
		return NULL;
	newList->capacity = capacity;
	newList->count = 0;
	return newList;
}

int insert(List L, struct ElementType e, Position p) {
	if (p > L->count || L->count == L->capacity)
		return FALSE;

	Position current = L->count;
	while (current != p) {
		L->theArray[current] = L->theArray[current - 1];
		current--;
	}
	L->theArray[p] = e;
	L->count++;
	return TRUE;
}

void del(List L, Position p) {
	if (p > L->count - 1)
		return;
	while (p < L->count - 1) {
		L->theArray[p] = L->theArray[p + 1];
		p++;
	}
	L->count--;
}

void show(List L) {
	unsigned long i;
	for (i = 0; i < L->count; i++)
		printf("ID: %lu			Name: %s\n", L->theArray[i].ID, L->theArray[i].name);
}

int main() {
	List stdList = createList(100);
	struct ElementType e1, e2, e3;

	e1.ID = 1; e1.name = "Nguyen Van A";
	e2.ID = 2; e2.name = "Nguyen Van B";
	e3.ID = 3; e3.name = "Nguyen Van C";

	insert(stdList, e1, 0);
	insert(stdList, e2, 1);
	insert(stdList, e3, 2);

	printf("The list is:\n");
	show(stdList);

	del(stdList, 2);

	printf("\nThe list is:\n");
	show(stdList);

	return 0;
}