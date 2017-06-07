#include<iostream>
#include<vector>
#include<time.h>
using namespace std;
class TSP{
private:
	int* dis;
	vector<int*> solution;
	int dim;
	int max;
	
public:
	TSP();
	TSP(int* mat,int n);
	void initSolution(int N);
	void initSolution(int* solution,int n);
	void genetic();
	void result(int* result);
/*	double GenProb(){
		srand((unsigned)time(NULL));
		double tmp=rand()%max;
		return tmp/max;
	};*/
	void showData();
	~TSP();
};
TSP::~TSP(){
	for(vector<int *>::iterator it=solution.begin();it!=solution.end();it++){
		delete [] *it;
	}
	delete dis;
}
TSP::TSP(int* mat,int n){//init the block way with the row max-sum
	max=65332;
	dis=new int [n*n];
	dim=n;
	for(int i=0;i!=n;i++){
		for(int j=0;j!=n;j++)
			dis[i*dim+j]=mat[i*n+j];
	}

	int imposible=0;
	int imposible=0;
	for(int i=0;i!=dim;i++){
		int max=0;
		for(int j=0;j!=dim;j++){
			if(dis[i*dim+j]>max){
				max=dis[i*dim+j];
			}
		}
		imposible+=max;
	}
	for(int i=0;i!=dim;i++){
		for(int j=0;j!=dim;j++){
			if(i!=j&&dis[i*dim+j]==0){
				dis[i*dim+j]=imposible;
			}
		}
	}
}
void TSP::showData(){
	for(int i=0;i!=dim;i++){
		for(int j=0;j!=dim;j++){
			cout<<dis[i*dim+j]<<" ";
		}
		cout<<endl;
	}
}
void TSP::initSolution(int N){// generate N solutions
	for(int i=0;i!=N;i++){// this use bsf to generate
		int* tmpS;
		tmpS=new int [dim];
		solution.push_back(tmpS);
	}
	
}
int main(){
	int mat[16]={0,1,2,0,
		     1,0,4,2,			
		     2,4,0,3,
	             0,2,3,0};
	TSP a(mat,4);
	a.showData();
	return 0;
} 
