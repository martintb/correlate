#include <functional>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void test_kernel(int natoms1, int natoms2, function<bool (int i,int j)> func);

int main() 
{
  vector<int> mol1 = {1,2,3,4,5,6,7,8,9};
  vector<int> mol2 = {1,2,3,4,5,6,7,8,9};
  function<bool (int i,int j)> func;
  bool equals =true;
  if (equals) {
    func = [&] (int i,int j) {
      bool test1 = false;
      bool test2 = mol1[i]==mol2[j];
      return test1 and test2;
    };
  } else {
    func = [&] (int i,int j) {return mol1[i]!=mol2[j];};
  }
  cout << boolalpha << func(1,1) << endl;
  cout << boolalpha << func(1,2) << endl;


  test_kernel(mol1.size(),mol2.size(),func);
  return EXIT_SUCCESS;
}

void test_kernel(int natoms1, int natoms2, function<bool (int i,int j)> func)
{
  int counter=0;
  for (int i=0;i<natoms1;i++) {
    for (int j=0;j<natoms2;j++) {
      if (func(i,j)) {
        cout << i << " " << j << endl;
        counter++;
      }
    }
  }
  cout << "Final Count: " << counter << endl;
}
