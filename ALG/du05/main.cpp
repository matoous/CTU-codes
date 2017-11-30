// by Matous Dzivjak <dzivjak@matous.me>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>

using namespace std;

#define len(x) (int)x.size()

typedef struct node_t {
  int value;
  int count;
  int depth;
  struct node_t* parent;
  struct node_t* left;
  struct node_t* right;
} node_t;

// Holds all trees after the deletion is performed
vector<node_t*> forest;

// Create new node
node_t* newNode(int value, node_t* parent){
  node_t* node = (node_t*)malloc(sizeof(node_t));
  node->value = value;
  node->parent = parent;
  node->left = node->right = NULL;
  node->count = node->depth = 0;
  return node;
}

// Insert value into the binary search tree
node_t* insert(node_t* root, int key, node_t* parent) {
  if(root == NULL)
    return root = newNode(key, parent);
  if(key < root->value)
    root->left = insert(root->left, key, root);
  else
    root->right = insert(root->right, key, root);
  root->count = ((root->left != NULL) ? root->left->count+1 : 0) + ((root->right != NULL) ? root->right->count+1 : 0);
  return root;
}

// Propagates children count change to the parent
void propagateChange(node_t* node, int change){
  if(node == NULL) return;
  node->count -= change;
  propagateChange(node->parent, change);
}

// Removes given interval from the tree
void remove(node_t* node, int A, int B, bool hasParent){
  if(node == NULL) return;
  // This node should be deleted
  if(node->value <= B && node->value >= A){
    if(node->value >= A)
      remove(node->left, A, B, false);
    if(node->value <= B)
      remove(node->right, A, B, false);
    if(hasParent)
      propagateChange(node, node->count+1);
    node = NULL;
  }
  else {
    // This node will live
    if(node->value > B){
      remove(node->left, A, B, true);
      if(node->left != NULL && node->left->value <= B && node->left->value >= A)
        node->left = NULL;
    }
    if(!hasParent)
      forest.push_back(node);
    if(node->value < A) {
      remove(node->right, A, B, true);
      if(node->right != NULL && node->right->value <= B && node->right->value >= A)
        node->right = NULL;
    }
  }
}

// Find minimal child under given node
node_t* min(node_t* node){
  if(node->left == NULL)
    return node;
  else
    return min(node->left);
}

// Find maximal child under given node
node_t* max(node_t* node){
  if(node->right == NULL)
    return node;
  else
    return max(node->right);
}

// Value to print at the end
int O = 0, P = 0;

// Breath first search the binary search tree to get the results
void bfs(node_t* root){
  int curr_cnt = 0, prev_cnt = 0, lvl = 0;

  queue<node_t*> Q;

  root->depth = 0;
  lvl = 0;
  Q.push(root);

  while(!Q.empty()){
    auto curr = Q.front(); Q.pop();
    if(curr->depth != lvl){
      lvl = curr->depth;
      prev_cnt = curr_cnt;
      curr_cnt = 0;
    }
    curr_cnt++;
    if(curr->left != NULL){
      curr->left->depth = curr->depth + 1;
      Q.push(curr->left);
    }
    if(curr->right != NULL){
      curr->right->depth = curr->depth + 1;
      Q.push(curr->right);
    }
  }
  O = lvl;
  P = prev_cnt;
}

// Merge function as described in the assignment
node_t* merge(vector<node_t*> roots){
  if(len(roots) == 0)
    return NULL;
  if(len(roots) == 1)
    return roots[0];

  // Get the middle node
  int k = 0, tmp = 0;
  node_t* ts = NULL;
  vector<node_t*> lefts, rights;

  for(auto tree : roots)
    k += tree->count+1;
  int m = (k+1)/2;
  for(auto tree : roots){
    tmp += tree->count+1;
    // Middle node
    if (tmp >= m && ts == NULL) {
      ts = tree;
      continue;
    }
    if(ts == NULL)
      lefts.push_back(tree);
    else
      rights.push_back(tree);
  }

  // Do the merge
  min(ts)->left = merge(lefts);
  max(ts)->right = merge(rights);
  return ts;
}

int main(){
  int N, X, A, B;
  scanf("%d", &N);

  // Build the tree
  node_t* root = NULL;
  for(int i = 0; i < N; i++){
    scanf("%d", &X);
    root = insert(root, X, NULL);
  }

  // Delete given interval
  scanf("%d %d", &A, &B);
  remove(root, A, B, false);

  // Merge remaining trees
  root = merge(forest);

  // Do breath first search
  bfs(root);
  printf("%d %d\n", O, P);
  return 0;
}
