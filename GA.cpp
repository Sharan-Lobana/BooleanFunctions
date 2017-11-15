#include <bits/stdc++.h>
using namespace std;
typedef vector<int> vi;
typedef vector<vector<int> > vvi;
typedef pair<vector<int>,int> pvii;
typedef vector<pair<int,int> > vpii;
typedef pair<int,int> pii;
typedef vector<pair<vector<int>,int> >  vpvii;
typedef map<vector<int>, bool> mvib;
#define MAX 100  //number of iterations of the GA algorithm
#define P 10      //number of parents in a generation
#define HC true   //flag to use Hill Climbing
#define N 10       //Cardinality of Boolean Functions
#define BITS (1<<N)

int dot_product[BITS][BITS];
int walsh_hadamard[BITS];

void calculate_walsh_hadamard(vi bf){
	for(int u=0;u<BITS;u++){
		walsh_hadamard[u]=0;
		for(int x=0;x<BITS;x++){
			if(dot_product[u][x]==bf[x])walsh_hadamard[u]++;
			else walsh_hadamard[u]--;
		}
	}
}

int findNonLinearity(vi bf){
	calculate_walsh_hadamard(bf);
	int mx=0;
	for(int u=0;u<BITS;u++)
		mx=max(mx,abs(walsh_hadamard[u]));
	return ((BITS-mx)/2);
}

vi hill_climb(vi bf){
	int iter=4;
	while(true){
		calculate_walsh_hadamard(bf);
		int i,mx=0;
		for(i=0;i<BITS;i++)mx=max(mx,abs(walsh_hadamard[i]));
		for(i=0;i<BITS;i++){
			bool satisfies=true;
			for(int u=0;u<BITS;u++){
				if(walsh_hadamard[u]==mx || walsh_hadamard[u]==(mx-2))
					satisfies&=(bf[i]==dot_product[u][i]);
				if(walsh_hadamard[u]==(-mx) || walsh_hadamard[u]==(2-mx))
					satisfies&=(bf[i]!=dot_product[u][i]);
			}
			if(satisfies){
				bf[i]^=1;
				break;
			}
		}
		if(i==BITS)break;
	}
	return bf;
}

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

int main()
{
  //freopen("output.txt","w",stdout);
  srand(time(NULL));  //set the random number generator seed for reproducibility
  vvi pool; //pool of individual boolean functions

  //Precomputing the dot products
	for(int i=0;i<BITS;i++)for(int j=0;j<BITS;j++){
		dot_product[i][j]=0;
		for(int k=0;k<N;k++){
			dot_product[i][j]^=(((i>>k)&1)*((j>>k)&1));
		}
	}

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
        if(HC)
          child = hill_climb(child);

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
  for(int ind = 0; ind < BITS; ind++)
  cout<<maxNonLinearityBF[ind];
  cout<<endl;
  cout<<maxNonLinearity<<endl;
}
