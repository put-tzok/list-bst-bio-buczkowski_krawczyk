#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

unsigned int ns[] = { 900, 1700, 4504, 7054, 10900, 15620, 21032, 29543, 35329, 41069};

struct node {
    int key;
    struct node *left;
    struct node *right;
};

struct node *root = NULL;

struct node **tree_search(struct node **candidate, int value) {

    if(*candidate==NULL){
        return candidate;
    }

    if(value<(*candidate)->key){
        return tree_search(&(*candidate)->left, value);
    }

    if(value>(*candidate)->key){
        return tree_search(&(*candidate)->right, value);
    }
    return candidate;
}

struct node* createNode(int value){

    struct node* new_node = malloc(sizeof(struct node));
    new_node->key= value;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

struct node* tree_insert(int value) {

    if(root==NULL){
       root=createNode(value);
      }

    else{
    struct node **node_insert=tree_search(&root,value);
       *node_insert=createNode(value);
      }
    return NULL;
}

struct node **tree_maximum(struct node **candidate) {

    if((*candidate)->right!=NULL){
      return tree_maximum(&(*candidate)->right);
    }
    return candidate;
}

void tree_delete(int value) {

    struct node **del_node=tree_search(&root, value);

  if((*del_node)->left==NULL && (*del_node)->right==NULL){
     *del_node=NULL;
    }

    else if((*del_node)->left!=NULL && (*del_node)->right==NULL){
      *del_node=(*del_node)->left;
    }

    else if((*del_node)->left==NULL && (*del_node)->right!=NULL){
      *del_node=(*del_node)->right;
    }

    else {
    struct node **max_node=tree_maximum(&(*del_node)->left);
      (*del_node)->key=(*max_node)->key;
      *max_node=(*max_node)->left;
  }
}

unsigned int tree_size(struct node *element) {

    if(element==NULL) return 0;
    else return(tree_size(element->left) + 1 + tree_size(element->right));
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

bool is_bst(struct node *element) {

    if (element == NULL) {
        return true;
    }

    if (element->left == NULL && element->right == NULL) {
        return true;
    }

    if (element->left == NULL && element->right != NULL) {
        return (element->key < element->right->key) && is_bst(element->right);
    }

    if (element->left != NULL && element->right == NULL) {
        return (element->key > element->left->key) && is_bst(element->left);
    }

    return (element->key > element->left->key)
        && (element->key < element->right->key)
        && is_bst(element->left)
        && is_bst(element->right);
}

void insert_increasing(int *t, int n) {
    for (int i = 0; i < n; i++) {
        tree_insert(t[i]);
    }
}

void insert_random(int *t, int n) {
    shuffle(t, n);
    for (int i = 0; i < n; i++) {
        tree_insert(t[i]);
    }
}

void treebinary(int *t, int p, int r){

   if(p==r){
    tree_insert(t[p]);
  }

   else if(r-p==1) {
    tree_insert(t[p]);
    tree_insert(t[r]);
  }

   else {
    int q=p+(r-p)/2;
    tree_insert(t[q]);
    treebinary(t,p,q-1);
    treebinary(t,q+1,r);
  }
}

void insert_binary(int *t, int n) {
    // elements are already sorted
    treebinary(t,0,n-1);
}

char *insert_names[] = { "Increasing", "Random", "Binary" };
void (*insert_functions[])(int*, int) = { insert_increasing, insert_random, insert_binary };

int main(int argc, char **argv) {
    for (unsigned int i = 0; i < sizeof(insert_functions) / sizeof(*insert_functions); i++) {
        void (*insert)(int*, int) = insert_functions[i];

        for (unsigned int j = 0; j < sizeof(ns) / sizeof(*ns); j++) {
            unsigned int n = ns[j];

            int *t = malloc(n * sizeof(*t));
            fill_increasing(t, n);

            clock_t insertion_time = clock();
            insert(t, n);
            insertion_time = clock() - insertion_time;
            assert(tree_size(root) == n);
            assert(is_bst(root));

            shuffle(t, n);

            clock_t search_time = clock();
            for (unsigned int k = 0; k < n; k++) {
                struct node **pnode = tree_search(&root, t[k]);
                struct node *iter = *pnode;
                assert(iter != NULL);
                assert(iter->key == t[k]);
            }
            search_time = clock() - search_time;


            shuffle(t, n);


            for (unsigned int l = 0, m = n; l < n; l++, m--) {
                assert(tree_size(root) == m);
                tree_delete(t[l]);
                assert(is_bst(root));
            }
            assert(tree_size(root) == 0);

            free(root);
            free(t);

            printf("%d\t%s\t%f\t%f\n",
                    n,
                    insert_names[i],
                    (double)insertion_time / CLOCKS_PER_SEC,
                    (double)search_time / CLOCKS_PER_SEC);
        }
    }
    return 0;
}
