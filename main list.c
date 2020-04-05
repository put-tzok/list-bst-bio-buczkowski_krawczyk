#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int ns[] = {9000, 17000, 24504, 32054, 40900, 48620 ,50203, 55502 , 69329, 79069};

struct node {
    int key;
    struct node *next;
};

struct node *head = NULL;

struct node* list_insert(int value) {

     if(head==NULL){
        head = malloc(sizeof(head));
        head->key=value;
        head->next=NULL;
        }

     else{
       struct node *wsk = malloc(sizeof(head));
       wsk->key=value;
       wsk->next=head;
       head=wsk;
     }
    return head;
}

struct node* list_search(int value) {

     struct node *tpp = head ;
     while (value!=tpp->key && tpp!=NULL) {
     tpp=tpp->next ;
     }
     return tpp ;
}

void list_delete(int value) {

   if(head->key==value){
      head=head->next;
      }

   else {
    struct node *prev = head ;
    struct node *iter=head->next;

    while(iter!=NULL && iter->key!=value){
        prev=iter;
        iter=iter->next; }

    if(iter!=NULL) prev->next=iter->next;
     }
}

 unsigned int list_size() {

    int size=0;
    struct node *wskaznik = head ;
    while (wskaznik!=NULL) {
      size++ ;
      wskaznik=wskaznik->next ;
    }
    return size;
}

void fill_increasing(int *t, int n) {
  for (int i = 0; i < n; i++) {
        t[i] = i;
    }

}

void shuffle(int *t, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % i;
        int temp = t[i];
        t[i] = t[j];
        t[j] = temp;
    }
}

int main() {
    bool no_yes[] = { false, true };

    for (unsigned int i = 0; i < sizeof(no_yes) / sizeof(*no_yes); i++) {
        bool enable_shuffle = no_yes[i];

        for (unsigned int j = 0; j < sizeof(ns) / sizeof(*ns); j++) {
            unsigned int n = ns[j];

          int  *t =(int*) malloc(n * sizeof(*t));
            fill_increasing(t, n);

            if (enable_shuffle) {
                shuffle(t, n);
            }

            clock_t insertion_time = clock();
            for (unsigned int k = 0; k < n; k++) {
                struct node *iter = list_insert(t[k]);
                assert(iter != NULL);
                assert(iter->key == t[k]);
            }
            insertion_time = clock() - insertion_time;

            shuffle(t, n);

            clock_t search_time = clock();
            for (unsigned int k = 0; k < n; k++) {
                struct node *iter = list_search(t[k]);
                assert(iter != NULL);
                assert(iter->key == t[k]);
            }
            search_time = clock() - search_time;

            shuffle(t, n);

            for (unsigned int k = 0, l = n; k < n; k++, l--) {
                assert(list_size() == l);
                list_delete(t[k]);
            }
            assert(list_size() == 0);
            assert(head == NULL);

            free(t);

            printf("%d\t%s\t%f\t%f\n", n, enable_shuffle ? "true" : "false",
                    (double)insertion_time / CLOCKS_PER_SEC,
                    (double)search_time / CLOCKS_PER_SEC);
        }
    }
    return 0;
}
