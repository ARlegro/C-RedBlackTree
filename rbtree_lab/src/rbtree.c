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
      break;
    }
    
    // 아직 nil 만나기 전 
    parent = cur;
    if (cur->key < new_node->key) {
      cur = cur->right;
    } else {
      cur = cur->left;
    }
  }

  // 끼워주기 
  if (parent->key > new_node->key) {
    parent->left = new_node;
  } else {
    parent->right = new_node;
  }
  new_node->parent = parent;
}



// new_sub_root가 nil노드가 아니라는 증명은 이 메서드를 호출하는 쪽에서 
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


void left_rotate(rbtree *t, node_t *old_sub_root){
  node_t *new_sub_root = old_sub_root->right;
  
  // 새로운 subRoot의 자식들 넘겨주기 
  old_sub_root->right = new_sub_root->left;
  if (new_sub_root->left != t->nil){
    new_sub_root->left->parent = old_sub_root;
  }

  // subRoot의 부모를 연결시키기 
  update_parent_link(t, old_sub_root, new_sub_root);
  
  // 자손관계 역전 
  old_sub_root->parent = new_sub_root;
  new_sub_root->left = old_sub_root;
}

void right_rotate(rbtree *t, node_t *old_sub_root){
  node_t *new_sub_root = old_sub_root->left;
  
  // 새로운 subRoot의 자식들 넘겨주기 
  node_t *detached_node = new_sub_root->right;
  old_sub_root->left = detached_node;
  if (detached_node != t->nil){
    detached_node->parent = old_sub_root;
  }

  // subRoot의 부모를 연결시키기 
  update_parent_link(t, old_sub_root, new_sub_root);

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
      node_t *old_sub_root = g;
      node_t *new_sub_root = old_sub_root->left;
      
      old_sub_root->color = RBTREE_RED;
      new_sub_root->color = RBTREE_BLACK;

      right_rotate(t, old_sub_root);

    } else if (p == g->right){
      // 지그재그인 경우 
      if (p->left == cur) {
        right_rotate(t, p);
      }

      // Case 3
      node_t *old_sub_root = g;
      node_t *new_sub_root = old_sub_root->right;

      g->color = RBTREE_RED;
      new_sub_root->color = RBTREE_BLACK;

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
  // 아무것도 없는 경우 
  if (t->root == t->nil) {
    return NULL;
  }

  node_t *cur = t->root;
  // node_t *prev = t->root;
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
  if (t->root == t->nil) {
    return NULL;
  }

  node_t *cur = t->root;
  while (cur != NULL)
  {
    node_t *l = cur->left;
    if (l == t->nil) {
      break;
    }
    cur = l;
  }

  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  if (t->root == t->nil) {
    return NULL;
  }

  node_t *cur = t->root;
  while (cur != NULL)
  {
    node_t *right_node = cur->right;
    if (right_node == t->nil) {
      break;
    }
    cur = right_node;
  }

  return cur;
}

node_t *find_successor(const rbtree *t, const node_t *origin_node){
  node_t *right_sub_root = origin_node->right;
  node_t *min_node = right_sub_root;
  while (1)
  { 
    node_t *left_node = min_node->left;
    if (left_node == t->nil){
      break;
    }
    min_node = left_node;
  }
  return min_node;
}


void rbtree_transplant(rbtree *t, node_t *origin_child, node_t *new_child){
  node_t *p = origin_child->parent;

  if (p == t->nil){
    t->root = new_child;
  } else if (p->left == origin_child){
    p->left = new_child;
  } else {
    p->right = new_child;
  }
  
  if (new_child != t->nil){
    new_child->parent = p;
  }
}

#include <stdio.h>


const char *color_str[] = { "RED", "BLACK" };
//printf("색깔 = %s\n", color_str[c]);   

void rbtree_fixup_doubly_black(rbtree *t, node_t *db_node, node_t *parent){
  db_node->parent = parent; // nil 노드일 경우 대비 
  node_t *p = db_node->parent;
  node_t *brother = NULL;

  while (1)
  {

    printf("========================앞쪽=========================\n");
    printf("db_node.key = %d\n", db_node->key);     
    printf("parent.key = %d\n", db_node->parent->key);     
    // doubly_black 노드가 Root인 경우 
    if (db_node->parent == t->nil){
      //t->root = db_node;
      db_node->color = RBTREE_BLACK;
      break;
    }
   
    // p 초기화 
    p = db_node->parent;
    t->nil->parent = t->nil;
    t->nil->color = RBTREE_BLACK;    

    // db_node가 왼쪽일 경우 
    if (p->left == db_node){
      // brother 갱신 
      brother = p->right;
      //printf("시작 => brother의 색 = ")

      // Case 0. 형제가 Red인 경우 : 색변화 + left_rotate 후 brotehr 초기화 
      if (brother->color == RBTREE_RED){
        p->color = RBTREE_RED;
        brother->color = RBTREE_BLACK;
        
        //brother = brother->left; // 새로운 형제로 
        left_rotate(t, p);
        printf("문제아 1");
        continue;
      }

      /**
       * brother가 Black일 경우 
       */
      // Case 1. brother의 오른쪽 자식이 Red
      if (brother->right->color == RBTREE_RED){
        brother->color = p->color; 
        p->color = RBTREE_BLACK;
        brother->right->color = RBTREE_BLACK;
        left_rotate(t, p);
        break;
      }
      
       // Case 2. brother의 자식들이 모두 Black
      if (brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK) {
        brother->color = RBTREE_RED;

        // 부모에게 E.B 물려준 후 부모가 red_and_black일 경우는 색만 바꾸고 종료
        if (p->color == RBTREE_RED) {
          p->color = RBTREE_BLACK; 
          break;
        }

        // 이제 doubly_black은 부모가 됨 
        db_node = p;
        printf("문제아 2");
        continue;
      } 
      
      // Case 3. brotehr의 왼쪽 자식 Red, 오른쪽 자식 Black
      if (brother->left->color == RBTREE_RED && brother->right->color == RBTREE_BLACK){
        printf("brother.key = %d\n", brother->key);     
        // Case 1 을 유도하도록만 
        brother->color = RBTREE_RED;
        brother->left->color = RBTREE_BLACK;
        right_rotate(t, brother);
        printf("문제아 3");
        printf("문제아 3 : new Brother 색 = %s\n", color_str[p->right->color]);
        printf("문제아 3 : new Brother의 Key = %d\n", p->right->key);
        printf("문제아 3 : old brother 색 = %s\n", color_str[brother->color]);
        printf("문제아 3 : old brother의 Key = %d\n", p->right->right->key);
        continue; 
      }
      printf("엄청난 문제아 1");
    } 

    // db_node가 오른쪽일 경우 
    else {
      brother = p->left;
      
      // 형제가 Red일 경우 
      if (brother->color == RBTREE_RED) {
        p->color = RBTREE_RED;
        brother->color = RBTREE_BLACK;

        brother = brother->right;
        right_rotate(t, p);
        printf("문제아 4");
        continue;
      }

      /**
       * brotehr가 Black일 경우의 수 
       */
      // case 1. brother 왼쪽 자녀가 red일 경우 => 처리 후 바로 종료 
      if (brother->left->color == RBTREE_RED) {
        brother->color = p->color;
        p->color = RBTREE_BLACK;
        brother->left->color = RBTREE_BLACK;
        right_rotate(t, p);
        break;
      }

      // case 2. brother의 두 자녀가 Black인 경우 => 부모에 전파 
      if (brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK){
        brother->color = RBTREE_RED;
        
        if (p->color == RBTREE_RED) {
          p->color = RBTREE_BLACK; 
          break;
        }

        db_node = p;
        printf("문제아 5");
        continue;
      }

      // case 3. brother의 왼쪽 자녀가 Black, 오른쪽 자녀가 red
      if (brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_RED) {
        brother->color = RBTREE_RED;
        brother->right->color = RBTREE_BLACK;
        left_rotate(t, brother);
        printf("문제아 6");
        continue;
      }
      printf("엄청난 문제아 2");
    }

    printf("========================뒤쪽=========================");
    printf("db_node.key = %d\n", db_node->key); 
  }
  
  // nil 초기화 (색바꾼것도 위에서 분기 처리없애고 그냥 여기서 처리)
  t->nil->parent = t->nil;
  t->nil->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *target = rbtree_find(t, p->key);
  if (target == NULL) {
    return 0;
  }

  color_t original_coler = target->color;
  node_t *problem_child = NULL;
  node_t *problem_parent = NULL;
  node_t *removed_node = target;
  // 1. 자식 노드가 없는 경우 
  if (target->left == t->nil && target->right == t->nil){ 
    // 하나 밖에 없는 상황은 따로 처리 
    if (target == t->root) {  
      t->root = t->nil;
      free(target);
      return 1;
    } 

    rbtree_transplant(t, target, t->nil);
    problem_child = t->nil;
    problem_parent = removed_node->parent;
  } 
  // 2. 자식 노드가 2개인 경우 
  else if (target->left != t->nil && target->right != t->nil)  
  {     
    // 후임자 찾기 
    node_t *successor = find_successor(t, target);
    target->key = successor->key;
    
    // 연결 바꿔주기  
    problem_parent = successor->parent;
    problem_child = successor->right; // nil node일수도 있음
    rbtree_transplant(t, successor, successor->right);
    
    original_coler = successor->color; // 색깔 바꿔줘야 
    removed_node = successor; // 대신 삭제된걸로 처리     
  }
  
  // 3. 자식 노드가 1개인 경우 
  else { 
    node_t *new_child = (target->left != t->nil) ? target->left : target->right;
    
    // 기존 노드의 부모와 기존 노드의 자식을 연결시키기 
    problem_child = new_child;
    problem_parent = target->parent;
    rbtree_transplant(t, target, new_child);
  }
  free(removed_node); // 이거 수정해야 하나 (뭘 반환하랬는데)

  /**
   * 위반 여부 확인
   * 1. 레드일 경우 위반 X
   * 2. 블랙일 경우 위반 => E.B 추가 
   */
  if (original_coler == RBTREE_RED){
    return 1;
  }

  /**
   * 상황 : 삭제된 노드의 색이 Black일 경우의 로직들
   * E.B 추가됐다고 가정하면서 진행  
   * 크게 2가지 경우 1) R & B  2) B & B 
   */
  // 1. Red_and_Black : 색만 바꾸면 끝
  if (problem_child->color == RBTREE_RED){
    problem_child->color = RBTREE_BLACK;
    return 1;
  }

  // 2. Black_and_Black : Case 나뉨 
  rbtree_fixup_doubly_black(t, problem_child, problem_parent);

  return 1;
}


void recursive_tree(const rbtree *t, key_t *arr, node_t *cur, int *idx, const size_t n){

  if (*idx >= n || cur == t->nil){
    return;
  } 

  recursive_tree(t, arr, cur->left, idx, n);
  arr[*idx] = cur->key;
  (*idx)++;
  recursive_tree(t, arr, cur->right, idx, n);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (n == 0 || t->root == t->nil || t->root == NULL) {
    return 0;
  }

  int idx = 0;
  recursive_tree(t, arr, t->root, &idx, n);
  
  return 1;
}

//{10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};