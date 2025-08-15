
#include <stdlib.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void){
  rbtree *t = (rbtree *)malloc(sizeof(rbtree));
  node_t *nil = (node_t *)malloc(sizeof(node_t));

  nil->color = RBTREE_BLACK;
  nil->key = 0;
  nil->parent = nil;
  nil->left = nil;
  nil->right = nil;
  t->root = nil;
  t->nil = nil;
  return t;
}

node_t left_ratate(rbtree *t, node_t *x){
  node_t *temp = x->right;
  x->right = temp->left;

  // 회전으로 올리기 전 노드의 왼쪽 자식이 nill 이 아닌 경우 : 부모를 바꿔야 함 
  if (temp->left != t->nil) {
    temp->left->parent = x;
  }
  // nill 인경우 부모를 바꾸지 않는다 Cuz 트리 전체에서 공유하는 노드라 일관성이 깨짐 
  
  temp->parent = x->parent;

  if (x->parent == t->nil) {
    t->root = temp;
  } else{
    if (x == x->parent->left){
      x->parent->left = temp;
    } else {
      x->parent->right = temp;
    }
  }
  
  x->parent = temp;
  temp->left = x;
}

node_t right_rotate(rbtree *t, node_t *x){
  node_t *y = x->left;
  
  x->left = y->right;
  if (y->right != t->nil){
    y->right->parent = x;
  }

  y->parent = x->parent;
  if (x->parent == t->nil){
    t->root = y;
  } else if (x == x->parent->left){
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  
  y->right = x;
  x->parent = y;

  return *y;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {

  // TODO: implement insert
}
