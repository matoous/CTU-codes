#include <stdio.h>
#include <iostream>
#include <vector>

#define len(x) (int)x.size()

using namespace std;

typedef struct node_t {
  int v0, v1; // key 1 and 2
  node_t *lch, *rch; // left child and richild
  int height; // height of node
} node_t;

int rc = 0, D = 0, R = 0, NN = 1, rct, N, n;

bool is_internal(node_t* x){
  return x->lch != NULL && x->rch != NULL;
}

int min_of(node_t* node){
  if(node->v1) return node->v0 < node->v1 ? node->v0 : node->v1;
  return node->v0;
}

int max_of(node_t* node){
  if(node->v1) return node->v0 > node->v1 ? node->v0 : node->v1;
  return node->v0;
}

void set_min(node_t* n, int x){
  if(n->v1){
    if(n->v1 < n->v0)
      n->v1 = x;
    else
      n->v0 = x;
  }
}

// Create new node
node_t* new_node(int x){
  node_t* n = new node_t();
  n->v0 = x;
  n->v1 = 0;
  n->height = 0;
  n->lch = NULL;
  n->rch = NULL;
  return n;
}

// Get height of the node
// Can be speeded up by remembering this
int height_of(node_t *temp){
  if(temp == NULL) return 0;
  return temp->height;
}

node_t* singleRightRotate(node_t* t){

}

node_t* singleLeftRotate(node_t* t){

}

node_t* doubleLeftRotate(node_t* t){

}

node_t* doubleRightRotate(node_t* t){
  
}

int balance_of(node_t* x){

}

node_t* split(node_t* x, int c){

}

// Insert node into the tree
node_t* insert(node_t* x, int c){
  if(c == x->v0 || c == x->v1) return x;
  if(is_internal(x)){ // Internal node
    if(c < min_of(x)){

    }
    else if(max_of(x) < c){

    }
    else {

    }

    return x;
  }
  // Leaf

}



void condensate(node_t* n){

}

// Build new tree from reduced AVL tree
node_t* build(int depth){

}

node_t* reduce(node_t* n){
  condensate(n); // condensate AVL
  int x = 1;
  step = 1, idx = 0;
  while(x*2 < len(reduced_nodes)) // find x for: 2*(2^(x+1)-1) < |reduced_nodes|
    x = (1 << (1+step++)) - 1;
  step--;
  NN = x; // set number of nodes
  D = step; // set max depth
  bonus = len(reduced_nodes) - x; // if we place 1 number in each node with how many nodes are we left?
  n = build(0); // build the new tree
  reduced_nodes.clear(); // clear for next time
  R++; // increment reductions counter
  return n; // return new tree
}

int main(){
  node_t* root = NULL;
  cin >> N >> rct;
  for(int q = 0; q < N; ++q){
    cin >> n;
    if(root == NULL){
      root = new_node(n);
    } else {
      root = insert(root, n);
      if(rc == rct){
        rc = 0;
        root = reduce(root);
      }
    }
  }
  cout << NN << " " << height_of(root) << " " << R << endl;
  return 0;
}
