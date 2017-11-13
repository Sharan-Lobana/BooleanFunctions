#include <bits/stdc++.h>
using namespace std;
typedef vector<int> vi;
typedef vector<vector<int> > vvi;
typedef pair<vector<int>,int> pvii;
typedef vector<pair<int,int> > vpii;
typedef pair<int,int> pii;
typedef vector<pair<vector<int>,int> >  vpvii;
typedef map<vector<int>, bool> mvib;
#define MAX 1000  //number of iterations of the GA algorithm
#define P 10      //number of parents in a generation
#define HC true   //flag to use Hill Climbing
#define N 2       //Cardinality of Boolean Functions

vi generateRandomFunction(int n)
{
  vi retval;
  int numIter = pow(2,n);
  for(int i = 0; i < numIter; i++)
    retval.push_back(rand()%2);

  return retval;
}

bool mycomp(pii a, pii b)
{
  return a.second > b.second;
}

vi mergeParents(vi a, vi b)
{
  int hammingdistance = 0;
  int n = a.size();
  for(int i = 0; i < n; i++)
  {
    if(a[i] != b[i])
      hammingdistance++;
  }

  vi retval;

  if(hammingdistance <= pow(2,N-1))
  {
    for(int i = 0; i < n; i++)
    {
      if(a[i] == b[i])
        retval.push_back(a[i]);
      else
        retval.push_back(rand()%2);
    }
  }
  else
  {
    for(int i = 0; i < n; i++)
    {
      if(a[i] != b[i])
        retval.push_back(a[i]);
      else
        retval.push_back(rand()%2);
    }
  }
  return retval;
}
int findNonLinearity(vi a)
{
  return rand()%100000;
}
int main()
{
  srand(time(NULL));  //set the random number generator seed for reproducibility
  vvi pool; //pool of individual boolean functions

  int maxNonLinearity = 0;
  vi maxNonLinearityBF;
  int temp;
  vi foo;
  for(int i = 0; i < P; i++)
  {
  	foo = generateRandomFunction(N);
    pool.push_back(foo);
    temp = findNonLinearity(foo);
    if(temp > maxNonLinearity)
    {
      maxNonLinearity = temp;
      maxNonLinearityBF = foo;
    }
  }

  for(int i = 0; i < MAX; i++)
  {
    mvib children;  //children produced in this generation
    vi child;
    for(int j = 0; j < P; j++)
    {
      for(int k = j+1; k < P; k++)
      {
        child = mergeParents(pool[j],pool[k]);  //generate this child
        children[child] = true;
      }
    }

    mvib::iterator iter;
    vvi selection;
    vpii indices;
    int ind = 0;
    for(iter = children.begin(); iter != children.end(); iter++)
    {
      selection.push_back(iter->first);
      temp = findNonLinearity(iter->first);
      indices.push_back(make_pair(ind,temp));
      ind++;
      if(temp > maxNonLinearity)
      {
        maxNonLinearity = temp;
        maxNonLinearityBF = iter->first;
      }
    }
    sort(indices.begin(), indices.end(), mycomp);
    for(int i = 0; i < P; i++)
    {
      pool[i] = selection[indices[i].first];
    }
  }
  cout<<maxNonLinearity<<endl;
}
