#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

struct Node {
    int k;  // Promijenjeno iz coef u k
    int exp;
    struct Node* next;
};

typedef struct Node n;  // Promijenjeno iz Node u n

void insert(n** lilpoly, int k, int exp) {
    n* temp = (n*)malloc(sizeof(n));
    temp->k = k;
    temp->exp = exp;
    temp->next = NULL;

    if (*lilpoly == NULL || (*lilpoly)->exp < exp) {
        temp->next = *lilpoly;
        *lilpoly = temp;
        return;
    }

    n* current = *lilpoly;
    while (current->next != NULL && current->next->exp > exp) {
        current = current->next;
    }

    if (current->exp == exp) {
        current->k += k;
        free(temp);
    }
    else {
        temp->next = current->next;
        current->next = temp;
    }
}

void print(n* lilpoly) {
    if (lilpoly == NULL) {
        printf("0\n");
        return;
    }

    n* current = lilpoly;
    while (current != NULL) {
        if (current->k > 0 && current != lilpoly) {
            printf("+");
        }
        if (current->exp == 0)
            printf("%d", current->k);
        else if (current->exp == 1)
            printf("%dx", current->k);
        else
            printf("%dx^%d", current->k, current->exp);
        current = current->next;
    }
    printf("\n");
}

n* add(n* lilpoly1, n* lilpoly2) {
    n* result = NULL;

    while (lilpoly1 != NULL || lilpoly2 != NULL) {
        int k = 0;
        int exp = 0;

        if (lilpoly1 != NULL && (lilpoly2 == NULL || lilpoly1->exp > lilpoly2->exp)) {
            k = lilpoly1->k;
            exp = lilpoly1->exp;
            lilpoly1 = lilpoly1->next;
        }
        else if (lilpoly2 != NULL && (lilpoly1 == NULL || lilpoly2->exp > lilpoly1->exp)) {
            k = lilpoly2->k;
            exp = lilpoly2->exp;
            lilpoly2 = lilpoly2->next;
        }
        else {
            k = lilpoly1->k + lilpoly2->k;
            exp = lilpoly1->exp;
            lilpoly1 = lilpoly1->next;
            lilpoly2 = lilpoly2->next;
        }

        insert(&result, k, exp);
    }

    return result;
}

n* multiply(n* lilpoly1, n* lilpoly2) {
    n* result = NULL;

    for (n* p1 = lilpoly1; p1 != NULL; p1 = p1->next) {
        for (n* p2 = lilpoly2; p2 != NULL; p2 = p2->next) {
            int k = p1->k * p2->k;
            int exp = p1->exp + p2->exp;
            insert(&result, k, exp);
        }
    }

    return result;
}

void freeList(n* lilpoly) {
    while (lilpoly != NULL) {
        n* temp = lilpoly;
        lilpoly = lilpoly->next;
        free(temp);
    }
}

int main() {
    FILE* file = fopen("polinomi.txt", "r");
    if (file == NULL) {
        printf("Datoteka nije pronađena.\n");
        return 1;
    }

    n* lilpoly1 = NULL;  
    n* lilpoly2 = NULL; 

    int k, exp;

    // Čitanje polinoma iz datoteke
    while (fscanf(file, "%d %d", &k, &exp) == 2) {
        insert(&lilpoly1, k, exp);
        if (fgetc(file) == '\n') break;
    }

    while (fscanf(file, "%d %d", &k, &exp) == 2) {
        insert(&lilpoly2, k, exp);
        if (fgetc(file) == '\n') break;
    }

    printf("Prvi polinom: ");
    print(lilpoly1);
    printf("Drugi polinom: ");
    print(lilpoly2);

    // Zbrajanje polinoma
    n* sumResult = add(lilpoly1, lilpoly2);
    printf("Zbroj polinoma: ");
    print(sumResult);

    // Množenje polinoma
    n* mulResult = multiply(lilpoly1, lilpoly2);
    printf("Produkt polinoma: ");
    print(mulResult);

    // Oslobađanje memorije
    freeList(lilpoly1);
    freeList(lilpoly2);
    freeList(sumResult);
    freeList(mulResult);

    fclose(file);
    return 0;
}
