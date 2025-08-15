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

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  node_t *newNode = (node_t *)malloc(sizeof(node_t));
  newNode->color = RBTREE_RED;
  newNode->key = key;
  newNode->parent = NULL;
  newNode->left = NULL;
  newNode->right = NULL;

  // 처음인 경우 
  if (t->root == t->nil) {
    newNode->color = RBTREE_BLACK;
    newNode->parent = newNode;
    newNode->left = (node_t *) malloc(sizeof(node_t));
    newNode->right = (node_t *) malloc(sizeof(node_t));

    t->root = newNode;
  
    return t;
  }


  /* 시작 
  1. 타고 내려간다 : NULL이 아니라 nil 노드인거를 확인 
  2. 삽입할 자리 만들면 node만들고 대입  
  */
  node_t *cur = t->root;
  while (1)
  {
    /* code */
    // 중복이면 게임 끝
    if (cur->key == key) {
      return t;
    }

    // nil을 만났을 경우 
    if (!cur->key) {
      newNode->parent = cur->parent;
      cur = newNode;
      break;
    }
    
    if (cur->key < key) {
      cur = cur->right;
    } else {
      cur = cur->left;
    }
  }
  
  // 보조 
  while (cur->parent->color == RBTREE_RED)
  {
      /* code */
    node_t *g = cur->parent->parent;
    // 부모 삼촌이 Red인 경우 
    if (g->left == RBTREE_RED && g->right == RBTREE_BLACK){
      g->left->color = RBTREE_BLACK;
      g->right->color = RBTREE_BLACK;
      g->color = RBTREE_RED;
      cur = g;
      continue;
    }

    // 부모만 Red일 경우(삼촌은 Black)
    // 부모가 조부모의 왼쪽일 경우
    node_t *p = cur->parent;
    if (p == g->left) {
      
      // 지그재그인 경우 - 펴주기 
      if (p->right == cur) {
        g->left = cur;
        cur->parent = g;
        p->right = cur->left;
        if (cur->left != t->nil) {
          cur->left->parent = p;
        }
        
        cur->left = p;
        p->parent = cur;
      }

      // Case 3. 

      //// 1. 색깔 바꾸고 
      cur->color = RBTREE_BLACK;
      cur->parent->color = RBTREE_RED;
      
      //// 2. 우회전 
      node_t *originSubRoot = (node_t *)cur->parent;

      // 현재 기준 조부모와의 합의 
      if (originSubRoot->parent == t->nil){ // 루트인 경우 
        t->root = cur;
      } else if (originSubRoot->parent->left = originSubRoot){
        originSubRoot->parent->left = cur;
      } else {
        originSubRoot->parent->right = cur;
      }
      cur->parent = originSubRoot->parent;

      // cur의 오른쪽을 넘겨주기 
      originSubRoot->left = cur->right;
      if (cur->right != t->nil){  // nil일 경우 조심 
        cur->right->parent = originSubRoot;
      }

      cur->right = originSubRoot;
      originSubRoot->parent = cur;

    } else if (p == g->right){
      // 지그재그인 경우 
      if (p->left == cur) {
        cur->parent = p->parent;
        p->parent->right = cur;
        
        p->left = cur->right;
        if (cur->right != t->nil) {
          cur->right->parent = p;
        }
        
        cur->right = p;
        p->parent = cur;  
      }

      // Case 3 - cur이 중간에 와있는 상황 
      // 회전
      node_t *originSubRoot = cur->parent;
      cur->color = RBTREE_BLACK;
      originSubRoot->color = RBTREE_RED;

      originSubRoot->right = cur->left;
      if (cur->left != t->nil) {
        cur->left->parent = originSubRoot;
      }

      // subTree Root의 부모와 cur 연결하기(cur이 subTree Root될 것)
      if (originSubRoot->parent == t->nil){ // root인 경우 
        t->root = cur;
      } else if (originSubRoot->parent->left == originSubRoot){
        originSubRoot->parent->left = cur;
      } else {
        originSubRoot->parent->right= cur;
      }
      cur->parent = originSubRoot->parent;
      
      cur->left = originSubRoot;
      originSubRoot->parent = cur;
    }
  }

  // 마지막 : rott 
  if (t->root == RBTREE_RED){
    t->root = RBTREE_BLACK;
  }

  return t->root;
}

// 회전은 언제나 조부모기준 
void rotate_right(rbtree *t, node_t *grandParent){

}

void left_right(rbtree *t, node_t *grandParent){
  
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
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
