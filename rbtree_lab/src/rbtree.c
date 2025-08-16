#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = (node_t *) malloc(sizeof(node_t));
  
  p->nil->color = RBTREE_BLACK;
  p->nil->key = 0;
  p->nil->parent = p->nil;
  p->nil->left = p->nil;
  p->nil->right = p->nil;
  p->root = p->nil;

  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

/* 시작 
1. 타고 내려간다 : NULL이 아니라 nil 노드인거를 확인 
2. 삽입할 자리 만들면 node만들고 대입  
*/
void insert_node(rbtree *t, node_t *new_node){
  node_t *cur = t->root;
  node_t *parent = t->nil;
  while (1) {

    // nil을 만났을 경우 
    if (cur == t->nil) {
      
      // 왼쪽인 경우 
      if (parent->key > new_node->key) {
        parent->left = cur;
      } else {
        parent->right = cur;
      }
      
      new_node->parent = parent;
      cur = new_node;
      break;
    }
    
    // 아직 nil 만나기 전 
    if (cur->key < new_node->key) {
      cur = cur->right;
    } else {
      cur = cur->left;
    }
    
    parent = cur;
  }
}

void left_rotate(rbtree *t, node_t *old_sub_root){
  node_t *new_sub_root = old_sub_root->right;
  
  // 새로운 subRoot의 자식들 넘겨주기 
  old_sub_root->right = new_sub_root->left;
  if (new_sub_root->left != t->nil){
    new_sub_root->left->parent = old_sub_root;
  }

  // subRoot의 부모를 연결시키기 
  update_parent_link(t, old_sub_root, new_rbtree);
  
  // 자손관계 역전 
  old_sub_root->parent = new_sub_root;
  new_sub_root->left = old_sub_root;
}

void update_parent_link(rbtree *t, node_t *old_sub_root, node_t *new_sub_root){
  node_t* p = old_sub_root->parent;
  if (p == t->nil){
    t->root = new_sub_root;
  } else if (p->right == old_sub_root){
    p->right = new_sub_root;
  } else {
    p->left = new_sub_root;
  }
  new_sub_root->parent = p;  
}

void right_rotate(rbtree *t, node_t *old_sub_root){
  node_t *new_sub_root = old_sub_root->left;
  
  // 새로운 subRoot의 자식들 넘겨주기 
  old_sub_root->left = new_sub_root->right;
  if (new_sub_root->right != t->nil){
    new_sub_root->right->parent = old_sub_root;
  }

  // subRoot의 부모를 연결시키기 
  update_parent_link(t, old_sub_root, new_rbtree);

  // 자손관계 역전 
  old_sub_root->parent = new_sub_root;
  new_sub_root->right = old_sub_root;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->parent = t->nil;
  new_node->left = t->nil;
  new_node->right = t->nil;

  // 처음인 경우 
  if (t->root == t->nil) {
    new_node->color = RBTREE_BLACK;
    t->root = new_node;
    return new_node;
  }

  insert_node(t, new_node);

  node_t *cur = new_node;

  // 보조 
  while (cur->parent->color == RBTREE_RED)
  {
    /* code */
    node_t *g = cur->parent->parent;
    // case 1 : 부모 삼촌이 Red인 경우 
    if (g->left->color == RBTREE_RED && g->right->color == RBTREE_RED){
      g->left->color = RBTREE_BLACK;
      g->right->color = RBTREE_BLACK;
      g->color = RBTREE_RED;
      cur = g;
      continue;
    }

    // 부모만 Red일 경우(삼촌은 Black)
    node_t *p = cur->parent;
    if (p == g->left) {
      
      // 지그재그인 경우 - 펴주기 
      if (p->right == cur) {
        left_rotate(t, p);
      }

      // Case 3. 
      node_t *old_sub_root = (node_t *)cur->parent;
      
      cur->color = RBTREE_BLACK;
      old_sub_root->color = RBTREE_RED;

      right_rotate(t, old_sub_root);


    } else if (p == g->right){
      // 지그재그인 경우 
      if (p->left == cur) {
        right_rotate(t, p);
      }

      // Case 3
      node_t *old_sub_root = (node_t *)cur->parent;

      cur->color = RBTREE_BLACK;
      cur->parent->color = RBTREE_RED;

      left_rotate(t, old_sub_root);
    }
  }

  // 마지막
  if (t->root->color == RBTREE_RED){
    t->root->color = RBTREE_BLACK;
  }

  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find

  // 아무것도 없는 경우 
  if (t->root == t->nil) {
    return NULL;
  }

  node_t *cur = t->root;
  while (cur->key != key){
    // 없으면 null 
    if (cur == t->nil) {
      cur = NULL;
      break;
    }
    
    if (cur->key < key) {
      cur = cur->right;
    } else {
      cur = cur->left;
    }
  }

  return cur;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
