/*
Written by Cassius Ali
Email: pcali41@gwu.edu
*/
#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

typedef struct linked_list {
  int length;
  struct node *head;
  struct node *tail;
} linked_list;

typedef struct node {
  void *value;
  struct node *next;
  struct node *prev;
} node;

linked_list *ll_create(void) {
  linked_list *makelist=NULL;
  makelist = malloc(sizeof(linked_list));
  makelist->head = NULL;
  makelist->tail = NULL;
  makelist->length = 0;
  return makelist;
}

void ll_destroy(linked_list *ll) {
  node *x = ll->head;
  node *x_prev = x;
  if (x != NULL) {
    while (x->next!=NULL) {
      x = x->next;
      free(x_prev);
      x_prev = x;
    }
    free(x);
  }
  free(ll);
}

void ll_add(linked_list *ll, void *value) {
  node *n = malloc(sizeof(node));
  n->next = ll->head;
  n->prev = NULL;
  n->value = value;
  if (ll->head == NULL) {
    ll->head = n;
    ll->tail = ll->head;
  }
  else {
    ll->head->prev = n;
    ll->head = n;
  }
  ll->length++;
}

void ll_add_end(linked_list *ll, void *value) {
  node *n = malloc(sizeof(node));
  n->next = NULL;
  n->prev = ll->tail;
  n->value = value;
  if (ll->head == NULL) {
    ll->head = n;
    ll->tail = ll->head;
  }
  else {
    ll->tail->next = n;
    ll->tail = n;
  }
  ll->length++;
}

int ll_length(linked_list *ll) {
  return ll->length;
}

void *ll_remove_first(linked_list *ll) {
  node *x=ll->head; void *value;
  if (x == NULL) return NULL;
  ll->head = x->next;
  value = x->value;
  free(x);
  ll->length--;
  return value;
}

int ll_contains(linked_list *ll, void *value) {
  node *x=ll->head;
  if(x->value == value) return 1;
  while(x->value!=value && x->next!=NULL) {
    x = x->next;
    if (x->value == value) return 1;
  }
  return 0;
}

void ll_view(linked_list *ll) {
  node *x = ll->head;
  if (ll->length > 0) {
      printf("%d", (int)x->value);
    while(x->next != NULL) {
      x = x->next;
      printf("->%d", (int)x->value);
    }
  }
}
#endif // LINKED_LIST_H_INCLUDED
