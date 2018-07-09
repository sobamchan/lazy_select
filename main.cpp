#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <algorithm>
#include <cmath>
#include <time.h>

using namespace std;

int N = 1000000;
int K = N / 2;
vector<int> S(N);
vector<int> R;
vector<int> P;
int RSIZE = pow(sqrt(sqrt(N)), 3);
int a, b;
int smallerThanA;
int biggerThanB;
int samplingFailN = 0;


void prepareS() {
  // while(S.size() < N) {
  for (int i = 0; i < N; i++) {
    // newI = rand();
    // if not S.has(newI)
    //   S[i] = newI;
    S[i] = rand();
  }
  // }
}


void printVector(vector<int> v) {
  for (int i = 0; i < v.size(); i++) {
    cout << v[i] << endl;
  }
}


int getRandom(int min, int max) {
  return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}


void debugP() {
  cout << "debug" << endl;
}


void printInt(int n) {
  cout << n << endl;
}

bool run() {
  srand((unsigned int)time(NULL));

  prepareS();

  // make R
  R.clear();
  while (R.size() < RSIZE) {
    R.push_back(S[getRandom(0, S.size())]);
  }
  // cout << "R size: " << R.size() << endl;

  // sort R
  sort(R.begin(), R.end());

  // determine a, b
  int aIdx = K * pow(sqrt(sqrt(N)), -1) - sqrt(N);
  int bIdx = K * pow(sqrt(sqrt(N)), -1) + sqrt(N);
  a = R[aIdx];
  b = R[bIdx];
  // cout << "a: " << a << ", b: " << b << endl;
  // cout << "a index: " << aIdx << ", b index: " << bIdx << endl;

  // make P
  P.clear();
  smallerThanA = 0;
  biggerThanB = 0;
  for (int i = 0; i < S.size(); i++) {
    if (a >= S[i]) {
      smallerThanA++;
    } else if (b <= S[i]) {
      biggerThanB++;
    } else {
      P.push_back(S[i]);
    }
  }

  // sort P
  sort(P.begin(), P.end());

  // check conditions
  if (smallerThanA < K && K < biggerThanB) {
    cout << "not satisfied (ra, rb)" << endl;
    cout << "ra: " << smallerThanA << endl;
    cout << "rb: " << biggerThanB << endl;
    samplingFailN++;
    return false;
  } else if (P.size() >= 4 * pow(sqrt(sqrt(N)), 3)) {
    cout << "not satisfied (P is too large)" << endl;
    cout << "P size: " << P.size() << endl;
    samplingFailN++;
    return false;
  }
  return true;
}


float runOneExperiment() {
  // Returns time duration (lazy - sorting)

  clock_t start_lazy = clock();
  // <START> LAZY SELECT
  bool flag;
  // cout << "RUN" << endl;
  flag = run();
  while (flag == false) {
    cout << "FAILED!" << endl;
    cout << "RUN" << endl;
    run();
  }
  // <END> LAZY SELECT
  clock_t end_lazy = clock();
  double duration_lazy = (double)(end_lazy - start_lazy);

  // print result
  int ra = smallerThanA + 1;
  // cout << "result: " << P[K - ra + 1] << endl;

  clock_t start_sort = clock();
  // <START> SORT BASELINE
  sort(S.begin(), S.end());
  // <END> SORT BASELINE
  clock_t end_sort = clock();
  double duration_sort = (double)(end_sort - start_sort);

  // cout << "middle: " << S[K] << endl;
  // cout << "smallest: " << S[0] << endl;
  // cout << "biggest: " << S[S.size() - 1] << endl;

  return duration_sort / duration_lazy;
  // return duration_sort - duration_lazy;
}

int main() {
  int experimentN = 1000;
  float resultDurationsSum = 0;
  for (int i = 0; i < experimentN; i++) {
    resultDurationsSum += runOneExperiment();
  }
  cout << "N: " << N << endl;
  cout << "result time duration mean: " << resultDurationsSum / experimentN << endl;
  cout << "sampling failed N: " << samplingFailN << endl;
}
