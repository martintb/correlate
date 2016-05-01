#include <vector>
#include <cmath>
#include "histogram.hpp"

using namespace std;
void inter_mol_histogram(
                          vector<unsigned long> &hist,
                          vector<float> &x1, vector<float> &y1, vector<float> &z1,
                          vector<float> &x2, vector<float> &y2, vector<float> &z2,
                          vector<int> &mol1, vector<int> &mol2,
                          vector<float> &box, float rmax, float dr,unsigned long &pair_count
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
  float rmaxsq = rmax*rmax;

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

        float dist = dx*dx + dy*dy + dz*dz;
        if (dist<rmaxsq) {
          int bindex = static_cast<int>(sqrt(dist)/dr);
          hist[bindex] += 1;
        }
      }
    }
  }

};
