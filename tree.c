//
//  tree.c
//  tree
//
//  Created by William McCarthy on 134//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//
// clang tree.c utils.c -o test

// MAKE SURE YOU ENTER AT YOUR FINAL WORD OF THE FILE OR ELSE IT WONT COUNT THE LAST WORD OF THE FILE

//    WITH FILE USEAGE: ./test FILENAME NumberOfCharactersToCompare
// WITHOUT FILE USEAGE: doesnt work.......just make a file
// example ./test tree.c 5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "tree.h"
#include "utils.h"

//-------------------------------------------------
tnode* tnode_create(const char* word) {
  tnode* p = (tnode*)malloc(sizeof(tnode));
  p->word = strdup(word);    // copy of word allocated on heap
  p->count = 1;
  p->left = NULL;
  p->right = NULL;
  p->index = 0;
  return p;
}

//====================================================================
void tnode_delete(tnode* p) {
    free((void*)p->word);
    free(p);
}

//====================================================================
tree* tree_create(void) {
  tree* p = (tree*)malloc(sizeof(tree));
  p->root = NULL;
  p->size = 0;
  return p;
}

//====================================================================
static void tree_deletenodes(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_deletenodes(t, p->left);
  tree_deletenodes(t, p->right);
  tnode_delete(p);
  t->size--;
}

//====================================================================
void tree_clear(tree* t) {
  tree_delete(t);
  t->root = NULL;
  t->size = 0;
}

//====================================================================
void tree_delete(tree* t) { tree_deletenodes(t, t->root); }

//====================================================================
bool tree_empty(tree* t)  { return t->size == 0; }

//====================================================================
size_t tree_size(tree* t) { return t->size; }

//====================================================================
int charCmpNum = 9;
int numline = 1;
bool checkCMP = 0;
static tnode* tree_addnode(tree* t, tnode** p, const char* word) {
  int compare;
  
  if (*p == NULL) {
    *p = tnode_create(word);
    (*p)->list[(*p)->index++] = numline;
  } else if (checkCMP == 1 && (compare = strncmp(word, (*p)->word, charCmpNum)) == 0) {
      (*p)->count++;
      (*p)->list[(*p)->index++] = numline;
  } else if (checkCMP == 0 && (compare = strcmp(word, (*p)->word)) == 0) {
      (*p)->count++;
      (*p)->list[(*p)->index++] = numline;
  } else if (compare < 0) { tree_addnode(t, &(*p)->left, word);
  } else {
    tree_addnode(t, &(*p)->right, word);
  }

  return *p;
}

//====================================================================
static char* str_process(char* s, char* t) {
  char* p = s;
  char ignore[] = "\'\".,;;?!()/’";
  while (*t != '\0') {
    if (strchr(ignore, *t) == NULL || (*t == '\'' && (p != s || p != s + strlen(s) - 1))) {
      *p++ = tolower(*t);
    }
    ++t;
  }
  *p++ = '\0';
  return s;
}

//====================================================================
tnode* tree_add(tree* t, char* word) {
  char buf[100];
  
  if (word == NULL) { return NULL; }
  str_process(buf, word);
  
  tnode* p = tree_addnode(t, &(t->root), buf);
  t->size++;

  return p;
}

//====================================================================
static void tree_printme(tree* t, tnode* p) {
  if (p->count > 1) { printf("%5d -- ", p->count); }
  else {
    printf("         ");
  }
  printf("%-18s", p->word);
  printf("        [");
  for(int i = 0; i < p->index; i++){
    if(i == p->index - 1)
      printf("%d]", p->list[i]);
    else printf("%d, ", p->list[i]);
  }
  printf("\n");
}

//====================================================================
static void tree_printnodes(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_printnodes(t, p->left);
  tree_printme(t, p);
  tree_printnodes(t, p->right);
}

//====================================================================
static void tree_printnodes_preorder(tree* t, tnode* p) {
  if (p == NULL) { return; }

  tree_printme(t, p);
  tree_printnodes(t, p->left);
  tree_printnodes(t, p->right);
}

//====================================================================
static void tree_printnodes_postorder(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_printnodes_postorder(t, p->left);
  tree_printnodes_postorder(t, p->right);
  tree_printme(t, p);
}

//====================================================================
static void tree_printnodes_reverseorder(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_printnodes_reverseorder(t, p->right);
  tree_printme(t, p);
  tree_printnodes_reverseorder(t, p->left);
}

// typedef struct inode inode;
// struct inode{
//   tnode* current;
//   inode* next;
// };
// typedef struct ilist ilist;
// struct ilist{
//   inode* head;
//   inode* tail;
//   size_t size;
// };
// ilist* sorted[100];

// inode* inode_create(tnode* p, inode* next){
//   inode* b = (inode*)malloc(sizeof(inode));
//   b->current = p;
//   b->next = next;
//   return b;
// }

// ilist* ilist_create(void){
//   ilist* p = (ilist*)malloc(sizeof(ilist));
//   p->head = NULL;
//   p->tail = NULL;
//   p->size = 0;
//   return p;
// }

// static void ilist_pushfront(ilist* list, tnode* c){
//   inode* p = inode_create(c, list->head);
//   list->head = p;
//   if(list->size == 0){ list->tail = p; }
//   ++list->size;
// }

// void ilist_pushback(ilist* list, tnode* c){
//   if(list == NULL){ ilist_create(); }
//   inode* p = inode_create(c, NULL);
//   if(list->size == 0){ 
//     list->head = list->tail = p;
//     ++list->size;
//   }
//   else {
//   list->tail->next = p;
//   list->tail = p;
//   ++list->size;
//   }
// }

// static void tree_storenodes_decreaseorder(tree*t, tnode *p){
//   if (p == NULL){ return; }
//   tree_storenodes_decreaseorder(t, p->left);
//   ilist_pushback(sorted[p->count], p);
//   tree_storenodes_decreaseorder(t, p->right);
// }

// static void inode_printme(tnode* p) {
//   if (p->count > 1) { printf("%5d -- ", p->count); }
//   else {
//     printf("         ");
//   }
//   printf("%-18s", p->word);
//   printf("        [");
//   for(int i = 0; i < p->index; i++){
//     if(i == p->index - 1)
//       printf("%d]", p->list[i]);
//     else printf("%d, ", p->list[i]);
//   }
//   printf("\n");
// }

// static void printdecreasearray(){
//   for(int i = 99; i > 0; i--){
//     if(sorted[i] == NULL){ break; }
//     else{
//       inode* ptr = sorted[i]->head;
//       while(ptr != NULL){
//         inode_printme(ptr->current);
//         ptr = ptr->next;
//       }
//     }
//   }
// }

//====================================================================
//void tree_print_levelorder(tree* t);

//====================================================================
void tree_print(tree* t)              { tree_printnodes(t, t->root);               printf("\n"); }

//====================================================================
void tree_print_preorder(tree* t)     { tree_printnodes_preorder(t, t->root);      printf("\n"); }

//====================================================================
void tree_print_postorder(tree* t)    { tree_printnodes_postorder(t, t->root);     printf("\n"); }

//====================================================================
void tree_print_reverseorder(tree* t) { tree_printnodes_reverseorder(t, t->root);  printf("\n"); }

// void tree_print_decreaseorder(tree*t){
//   tree_storenodes_decreaseorder(t, t->root);
//   printdecreasearray();
// }

//====================================================================
void tree_test(tree* t) {
  printf("=============== TREE TEST =================================\n");
  printf("\n\nprinting in order...========================================\n");
  tree_print(t);
  printf("end of printing in order...=====================================\n\n");

  printf("\n\nprinting in reverse order...================================\n");
  tree_print_reverseorder(t);
  printf("end of printing in reverse order...=============================\n\n");
  printf("tree size is: %zu\n", tree_size(t));

  printf("\n\nprinting in decreasing order...================================\n");
  // tree_print_decreaseorder(t);
  printf("doesnt work commented out... if u want to see it \n");
  printf("end of printing in decreasing order...=============================\n\n");
  printf("tree size is: %zu\n", tree_size(t));
  
  printf("clearing tree...\n");
  tree_clear(t);
  printf("after clearing tree, size is: %zu\n", tree_size(t));
  tree_print(t);
  
  printf("=============== END OF TREE TEST ==========================\n");
}

//====================================================================
tree* tree_from_file(int argc, const char* argv[]) {
  if (argc < 2) { return NULL; }

  FILE* fin;
  const char* filename = argv[1];
  if ((fin = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Could not open file: '%s'\n", filename);
    exit(1);
  }
 
  if(atoi(argv[2]) > 0){
    charCmpNum = atoi(argv[2]);
    checkCMP = 1;
  }
  char buf[BUFSIZ];
  char delims[] = " \n";
  int size = 0;
  memset(buf, 0, sizeof(buf));

  tree* t = tree_create();
  int c;
  while((c = fgetc(fin)) != EOF){
    if(c == ' ' || c == '\n'){
      tree_add(t, buf);
      memset(buf, 0, sizeof(buf));
     }
    else{
    char b = c;
    strncat(buf, &b, 1);
    }
    if(c == '\n'){ numline++; }
  }
  printf("%d words added...\n", size);
  fclose(fin);

  return t;
}


//====================================================================
int main(int argc, const char* argv[]) {
  tree* t = tree_from_file(argc, argv);
  if (t != NULL) { tree_test(t);  tree_delete(t);  return 0; }
  
  char buf[BUFSIZ];
  char delims[] = " .,;?!\'\"()\n";
  int size = 0;
  memset(buf, 0, sizeof(buf));

  t = tree_create();
  int c;
    while (fgets(buf, BUFSIZ, stdin)) {
    char* word = strtok(buf, delims);
    tree_add(t, word);

    while ((word = strtok(NULL, delims)) != NULL) {
      tree_add(t, word);
    }
  }
  printf("%d words added...\n", size);

  tree_test(t);
  tree_delete(t);
  
  return 0;
}

