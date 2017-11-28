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

node_t* singleRightRotate(node_t* &t){
  node_t* u = t->lch;
  t->lch = u->rch;
  u->rch = t;
  t->height = max(height_of(t->lch), height_of(t->rch))+1;
  u->height = max(height_of(u->lch), t->height)+1;
  return u;
}

node_t* singleLeftRotate(node_t* &t){
  node_t* u = t->rch;
  t->rch = u->lch;
  u->lch = t;
  t->height = max(height_of(t->lch), height_of(t->rch))+1;
  u->height = max(height_of(t->rch), t->height)+1 ;
  return u;
}

node_t* doubleLeftRotate(node_t* &t){
  t->rch = singleRightRotate(t->rch);
  return singleLeftRotate(t);
}

node_t* doubleRightRotate(node_t* &t){
  t->lch = singleLeftRotate(t->lch);
  return singleRightRotate(t);
}

int balance_of(node_t* x){
  return height_of(x->lch) - height_of(x->rch);
}

node_t* split(node_t* x, int c){
  int small, mid, big; // Split the node and add this number
  if(c < min_of(x)){
    small = c;
    mid = min_of(x);
    big = max_of(x);
  } else if(c > max_of(x)) {
    big = c;
    small = min_of(x);
    mid = max_of(x);
  } else {
    mid = c;
    small = min_of(x);
    big = max_of(x);
  }
  x->lch = new_node(small);
  x->rch = new_node(big);
  x->v0 = mid;
  x->v1 = 0;
  x->height++;
  return x;
}

// Insert node into the tree
node_t* insert(node_t* x, int c){
  if(c == x->v0 || c == x->v1) return x;
  if(is_internal(x)){ // Internal node
    if(c < min_of(x)){
      x->lch = insert(x->lch, c);
      if(balance_of(x) == 2 && rc != rct){
        rc++;
        if(c <= min_of(x->lch))
          x = singleRightRotate(x);
        else
          x = doubleRightRotate(x);
      }
    }
    else if(max_of(x) < c){
      x->rch = insert(x->rch, c);
      if(balance_of(x) == -2 && rc != rct){
        rc++;
        if(c >= max_of(x->rch))
          x = singleLeftRotate(x);
        else
          x = doubleLeftRotate(x);
      }
    }
    else {
      int k = min_of(x);
      set_min(x, c);
      c = k;
      x->lch = insert(x->lch, c);
      if(balance_of(x) == 2 && rc != rct){
        rc++;
        if(c <= min_of(x->lch))
          x = singleRightRotate(x);
        else
          x = doubleRightRotate(x);
      }
    }
    x->height = max(height_of(x->lch), height_of(x->rch))+1;
    return x;
  }
  // Leaf
  if(!x->v1){ // Place in the node as second number
    x->v1 = c;
    return x;
  }
  // Leaf
  x = split(x, c);
  NN += 2; // Increment total nodes count by 2
  return x;
}

// nodes for new graph after reduce
vector<int> reduced_nodes;
int idx, bonus, step;

// Get all the numbers in sorted order from the tree
// that should be left in the graph after reduce procedure
void condensate(node_t* n){
  // Must be inorder!
  if(is_internal(n->lch)) condensate(n->lch);
  reduced_nodes.push_back(n->v0);
  if(n->v1)
    reduced_nodes.push_back(n->v1);
  if(is_internal(n->rch)) condensate(n->rch);
}

// Build new tree from reduced AVL tree
node_t* build(int depth){
  if(depth == step){ // leaf
    node_t* z = new_node(reduced_nodes[idx++]);
    if(bonus){ // can put 2 numbers in this node
      z->v1 = reduced_nodes[idx++];
      bonus--;
    }
    return z;
  } else { // inner node
    node_t* z = new_node(0);
    z->height += step - depth;
    z->lch = build(depth+1); // add left children
    z->v0 = reduced_nodes[idx++];
    if(bonus){ // can put 2 numbers in this node
      z->v1 = reduced_nodes[idx++];
      bonus--;
    }
    z->rch = build(depth+1); // add right children
    return z;
  }
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
