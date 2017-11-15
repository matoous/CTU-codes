// basic file operations
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

int main () {
  ofstream myfile;
  int x, y;
  myfile.open("test5.in");
  printf("give me x and y:\n");
  scanf("%d %d", &x, &y);
  myfile << x << " " << y << endl;
  for(int i = 0; i < x-1; i++){
    myfile << i << " " << i+1 << " " << rand()%500 << endl;
  }
  myfile << rand()%x << " " << rand() % x << " " << rand()%500 << endl;
  map<int, bool> key;
  for(int i = 0; i < y; i++){
    int c = rand()%x;
    while(key[c]) c = rand()%x;
    myfile << c << " ";
    key[c]= true;
  }
  myfile << endl;
  myfile.close();
  return 0;
}
