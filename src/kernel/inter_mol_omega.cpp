#include <vector>
#include <cmath>
#include "inter_mol_omega.hpp"

using namespace std;
void inter_mol_omega(
                      vector<float> &hist,
                      vector<float> &x1, vector<float> &y1, vector<float> &z1,
                      vector<float> &x2, vector<float> &y2, vector<float> &z2,
                      vector<int> &mol1, vector<int> &mol2,
                      vector<float> &box, float qmax,float dq,
                      unsigned long &pair_count
                    )
{

  int natoms1 = x1.size();
  int natoms2 = x2.size();
  float bx = box[0];
  float by = box[1];
  float bz = box[2];
  float bx2 = bx/2.0;
  float by2 = by/2.0;
  float bz2 = bz/2.0;
  int kmax = static_cast<int>(qmax/dq);

  for (int i=0;i<natoms1;i++) {
    for (int j=0;j<natoms2;j++) {
      if (mol1[i] != mol2[j]) {
        pair_count+=1;
        float dx = abs(x1[i] - x2[j]);
        float dy = abs(y1[i] - y2[j]);
        float dz = abs(z1[i] - z2[j]);

        if (dx>bx2) dx-=bx;
        if (dy>by2) dy-=by;
        if (dz>bz2) dz-=bz;
        float dist = sqrt(dx*dx + dy*dy + dz*dz);
        for (int k=0;k<kmax;k+=1) {
          hist[k] += sin(k*dq*dist)/dist;
        }
      }
    }
  }

};
