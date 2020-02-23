#include <stdio.h>

typedef struct {
	int the_thing;
	int the_other; 
} thing;

int main(int argv, char** argc) {
	thing a;
	a.the_thing = 45;
	a.the_other = 20;

	for (int i = 0; i < 10; i++) {
		int curr = a.the_thing * i;
		printf("hello butthead %d\n", curr);
	}

	printf("%d", a.the_other);
	
	return 0;
}

