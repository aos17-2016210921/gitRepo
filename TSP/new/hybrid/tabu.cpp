#include<iostream>
#include<vector>
#include<random>
#include<string>
#include<math.h>
#include<sys/time.h>
#include<cstdlib>
#include<algorithm>
#include<fstream>
#define MAX 165332
using namespace std;
typedef struct P{
	double possibility;
	int city;
}P;
double power2(double x){
	return x*x;
}
int strToInt(string s){
	int sum=0;
	for(int i=0;i!=s.length();i++){
		sum*=10;
		sum+=(s[i]-'0');
	}
	return sum;
}
double randV(){
	default_random_engine generator(random_device{}());
	uniform_real_distribution<double> fun(0.0,1.0);
	return fun(generator);
}
void update(double* phrom,int *path,double ro,int min,int dim){
	double* tmp;
	tmp=new double [dim*dim];
	for(int i=0;i!=dim;i++)
		for(int j=0;j!=dim;j++)
			tmp[i*dim+j]=phrom[i*dim+j];
	for(int i=0;i!=dim;i++)
		for(int j=0;j!=dim;j++){
			phrom[i*dim+j]=(1-ro)*tmp[i*dim+j];
		}
	for(int i=0;i!=dim;i++){
		phrom[i*dim+i]=0;
	}
	for(int t=1;t!=dim+1;t++){
		int i=path[t-1];
		int j=path[t];
		phrom[i*dim+j]=(1-ro)*tmp[i*dim+j]+ro/min;
	}
}
void init(double *mat,double *dis,int dim){
	for(int i=0;i!=dim;i++)
		for(int j=0;j!=dim;j++)
			dis[i*dim+j]=mat[i*dim+j];
}
void init(double *dis,int dim,ifstream &file){
  	string s;
        for(int i=0;i!=18;i++){
                file>>s;
        }
        int* city;
	city=new int[dim*3];
        for(int i=0;i!=dim;i++){
                for(int j=0;j!=3;j++){
                        file>>s;
                        city[i*3+j]=strToInt(s);
                }
        }
        for(int i=0;i!=dim;i++){
                for(int j=i;j!=dim;j++){
                        double tmp=power2(city[i*3+1]-city[j*3+1])+power2(city[i*3+2]-city[j*3+2]);
                        dis[i*dim+j]=sqrt(tmp);
                        dis[j*dim+i]=sqrt(tmp);
                }
        }
        for(int i=0;i!=dim;i++)
                dis[i*dim+i]=0;
	delete [] city;
}
double maxColSum(double *dis,int dim){
	double maxSum=0;
	for(int i=0;i!=dim;i++){
		double max=0;
		for(int j=0;j!=dim;j++){
			if(dis[i*dim+j]>max){
				max=dis[i*dim+j];
			}
		}
		maxSum+=max;
	}
	return maxSum;
}
void redis(double* dis,int dim,int maxSum){
	for(int i=0;i!=dim;i++)
		for(int j=0;j!=dim;j++){
			if(i!=j&&dis[i*dim+j]==0)
				dis[i*dim+j]=maxSum;
		}
}
void printM(double *dis,int dim){
	for(int i=0;i!=dim;i++){
		for(int j=0;j!=dim;j++){
			cout<<dis[i*dim+j]<<" ";
		}
		cout<<endl;
	}

}
void printV(int *vec,int dim){
	for(int i=0;i!=dim;i++)
		cout<<vec[i]<<" ";
	cout<<endl;
}
void varHL(int* gene,int dim,int local){//high local pos exchange,note that local must be smaller than dim and mod 2 == 0 
	if(local%2){
		local++;
	}
	if(local>dim)
		local=dim/4;
	int medium=local/2;
	int *tmp;
	tmp=new int [medium];
	for(int i=0;i!=medium;i++){
		tmp[i]=gene[dim-medium+i]
	}
	for(int i=0;i!=medium;i++){
		gene[dim-medium+i]=gene[dim-local+i];
		gene[dim-local+i]=tmp[i];
	}
	delete [] tmp;
}
void varLL(int* gene,int dim,int local){//low local pos exchange,note that local must be smaller than dim and mod 2 == 0
	if(local%2){
		local++;
	}
	if(local>dim){
		local=dim/4;
	}
	int medium=local/2;
	int *tmp;
	tmp=new int[medium];
	for(int i=0;i!=medium;i++){
		tmp[i]=gene[i];
	}
	for(int i=0;i!=medium;i++){
		gene[i]=gene[i+medium];
		gene[i+medium]=tmp[i];
	}
	delete [] tmp;
}
void var3(int* gene,int dim){//exchange the whole gene
	int medium=dim/2;
	if(dim%2){
		int* tmp;
		tmp=new int [medium];
		for(int i=0;i!=medium;i++)
			tmp[i]=gene[i];
		for(int i=0;i!=medium;i++){
			gene[i]=gene[medium+1+i];
			gene[medium+1+i]=tmp[i];
		}	
		delete [] tmp;
	}else{
		int *tmp;
		tmp=new int [medium];
		for(int i=0;i!=medium;i++)
			tmp[i]=gene[i];
		for(int i=0;i!=medium;i++){
			gene[i]=gene[medium+i];
			gene[medium+i]=tmp[i];
		}
		delete [] tmp;
	}
}
double ACO(double *phrom,double *dis,int dim,double ro,int cons,double maxSum){
	P* prob;
	int* tabu;
	prob=new P [(dim-1)];
	tabu=new int[dim];
	double L=maxSum;
	bool flag=true;
	int count=0;
	vector<int*> solution;
	while(flag){
                //cout<<"begin"<<endl;
                double* length;
                length=new double [dim];
                for(int i=0;i!=dim;i++)
                        length[i]=0.0;
                for(int i=0;i!=dim-1;i++){
                        int* solve;
                        solve=new int[dim+1];
                        for(int i=0;i!=dim;i++)
                                tabu[i]=0;
                        solve[0]=0;
                        tabu[0]=1;
                        for(int j=1;j!=dim;j++){

                                double sigmaPhrom=0.0;//
                                for(int t=0;t!=dim;t++){
                                        if(tabu[t]!=1){
                                                sigmaPhrom+=phrom[solve[j-1]*dim+t];
                                        }
                                }
                                int s=0;
                                for(int t=0;t!=dim;t++){
                                        if(tabu[t]!=1){
                                                if(s==0){
                                                        prob[s].possibility=phrom[solve[j-1]*dim+t]/sigmaPhrom;
                                                }else{
                                                        prob[s].possibility=prob[s-1].possibility+phrom[solve[j-1]*dim+t]/sigmaPhrom;
                                                }
                                                prob[s].city=t;
                                                s++;
                                        }
                                }
                                double p=randV();
                                for(int k=0;k!=s;k++){//no repeat
                                        if(p<=prob[k].possibility){
                                                solve[j]=prob[k].city;
                                                tabu[prob[k].city]=1;
                                                break;
                                        }
                                }
                                length[i]+=dis[solve[j-1]*dim+solve[j]];
                                //cout<<"tabu: ";
                                //printV(tabu,dim);
                        }
                        solve[dim]=0;
			length[i]+=dis[solve[dim-1]*dim];
                        //printV(solve,dim+1);
                        solution.push_back(solve);
                }//generate dim-1 ant and its route and related length
                double min=maxSum;
                int pos=0;
                for(int i=0;i!=dim-1;i++){
                        if(length[i]<min){
                                pos=i;
                                min=length[i];
                        }
                }
/*              cout<<"solve:";
                for(int i=0;i!=dim+1;i++){
                        cout<<solution[pos][i]<<" ";
                }
                cout<<endl;
                cout<<"min"<<min<<" "<<maxSum<<endl;*/
                update(phrom,solution[pos],ro,min,dim);
                if(L-min==0){
                        count++;
                }else{
                        count=0;
                }
                if(count>=cons){
                        flag=false;
			cout<<"haha"<<endl;
			double ratio=0.8;
                        int alive=int(ratio*(dim-1));
			alive=dim-alive;
                        double* sortA;
                        sortA=new double[dim-1];
                        for(int i=0;i!=dim-1;i++)
                                sortA[i]=length[i];
                      /*quick sort*/
                	sort(sortA,sortA+dim-1);
                        double bench=sortA[alive];
                        vector<int*> gene;
                        int i=0;
                        for(vector<int*>::iterator it=solution.begin();it!=solution.end();it++){
                                if(length[i]>=bench)
                                        gene.push_back(*it);
                                i++;
                        }
                        double var=0.5;
			double cross=0.2;
			int popul=gene.size();
			for(int i=0;i!=popul-1;i=i+2){
				int *p1,*p2;
				p1=gene[i];
				p2=gene[i+1];
				int *offspring;
				offspring=new int[dim+1];
				for(int t=0;t!=dim+1;t++){
					double p=randV();
					if(p>=0.5){
						offspring[t]=p1[t];
					}else{
						offspring[t]=p2[t];
					}
				}
				gene.push_back(offspring);
				if(gene.size()==solution.size())
					break;
			}
			for(int i=0;i!=gene.size();i++){
				double p0=randV();
				if(p0>var){
				}else{
					double p=randV();
					if(p<=0.33){
						varHL(gene[i]+1,dim-1,10);
					}else if(p<=0.66){
						varLL(gene[i]+1,dim-1,10);
					}else{
						var3(gene[i]+1,dim-1);
					}
				}	
			}
                        gene.clear();
                }
                L=min;
                cout<<L<<endl;
                /*gabage reuse*/
                for(vector<int*>::iterator it=solution.begin();it!=solution.end();it++){
                        delete [] *it;
                }
                solution.clear();
                delete [] length;
        }
	return L;
}
int main(int argc,char *argv[]){
	ifstream file("pr107.tsp");
	double* dis;
	double* phrom;
	P* prob;
	int dim=107;
	int* tabu;
	dis=new double [dim*dim];
	//init(mat,dis,dim);
	init(dis,dim,file);
	file.close();
	tabu=new int [dim];
	phrom=new double [dim*dim];
	prob=new P [(dim-1)];
	double initV=1./(dim*(dim-1));
	double ro=atof(argv[1]);
	int cons=atoi(argv[2]);
//	cout<<"ro:"<<ro<<" cons:"<<cons<<" ";
	for(int i=0;i!=dim;i++){
		for(int j=0;j!=dim;j++){
			phrom[i*dim+j]=initV;
		}
	}
	double maxSum=maxColSum(dis,dim);
	//cout<<maxSum<<endl;
	redis(dis,dim,maxSum);
//	printM(dis,dim);
	bool flag=true;
	double L=maxSum;
	int* staticSolve;
	staticSolve=new int [dim+1];
	struct timeval t_end,t_start;
//	cout<<"begin"<<endl;
	gettimeofday(&t_start,NULL);
	L=ACO(phrom,dis,dim,ro,cons,maxSum);
	gettimeofday(&t_end,NULL);
	int timeuse=1000000*(t_end.tv_sec-t_start.tv_sec)+t_end.tv_usec-t_start.tv_usec;
	cout<<"ro: "<<ro<<" cons:"<<cons;
	cout<<" totalLength:"<<L<<" timeConsum:"<<timeuse<<"us"<<endl;
	return 0;
}
