#include<stdio.h>
#include<stdlib.h>

typedef struct Node {
    int contents;
    struct Node* leftChild;
    struct Node* rightSibling;
} Node;

int count_leaves(struct Node* head) {
    int result = 0;

    if (head->leftChild == NULL) {
       result += 1;
    }
    else {
        result += count_leaves(head->leftChild);
    }

    if (head->rightSibling != NULL) {
        result += count_leaves(head->rightSibling);
    }

    return result;
}

int count_branches(struct Node* n) {
    int result = 0;

    if (n->leftChild != NULL) {
        result = 1 + count_branches(n->leftChild);
    }

    if (n->rightSibling != NULL) {
        result = result + 1 + count_branches(n->rightSibling);
    }

    return result;
}

int max(int a, int b, int c) {
    int max = 0;
    if (a > max) {
        max = a;
    }
    if (b > max) {
        max = b;
    }
    if (c > max) {
        max = c;
    }
    return max;
}

int max_branches(struct Node* head) {
    struct Node* curr = head;
    int max_b = 1;
    int max_rc_b = 0;
    int max_lc_b = 0;

    if(head->rightSibling == NULL && head->leftChild == NULL) {
        return 0;
    }

    while(curr->rightSibling != NULL) {
        max_b += 1;
        curr = curr->rightSibling;
    }

    if(head->rightSibling != NULL) {
        max_rc_b = max_branches(head->rightSibling);
    }
    
    if (head->leftChild != NULL) {
        max_lc_b = max_branches(head->leftChild);
    }

    return max(max_b, max_lc_b, max_rc_b);
}

/*
 *          A(5)
 *      B(7)   C(12)
 *   D(1)         F(4)
 *  E(9)        G(10) H(9) I(8)
 *
 * */


int main() {
    struct Node* a;
    struct Node* b;
    struct Node* c;
    struct Node* d;
    struct Node* e;
    struct Node* f;
    struct Node* g;
    struct Node* h;
    struct Node* i;

    a = (struct Node*)malloc(sizeof(struct Node));
    b = (struct Node*)malloc(sizeof(struct Node));
    c = (struct Node*)malloc(sizeof(struct Node));
    d = (struct Node*)malloc(sizeof(struct Node));
    e = (struct Node*)malloc(sizeof(struct Node));
    f = (struct Node*)malloc(sizeof(struct Node));
    g = (struct Node*)malloc(sizeof(struct Node));
    h = (struct Node*)malloc(sizeof(struct Node));
    i = (struct Node*)malloc(sizeof(struct Node));
    
    a->leftChild = b;
    a->rightSibling = NULL;

    b->leftChild = d;
    b->rightSibling = c;

    c->leftChild = f;
    c->rightSibling = NULL;

    d->leftChild = e;
    d->rightSibling = NULL;

    e->leftChild = NULL;
    e->rightSibling = NULL;

    f->leftChild = g;
    f->rightSibling = NULL;

    g->leftChild = NULL;
    g->rightSibling = h;

    h->leftChild = NULL;
    h->rightSibling = i;

    i->leftChild = NULL;
    i->rightSibling = NULL;

    int res = max_branches(c);

    printf("%d\n", res);

    return 0;
}
