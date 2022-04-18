#include <iostream>
#include <cstring>
using namespace std;

const int M=4;
const int N=2;

const int L=100;
double a=0.01;

double W[N+1];
double X[M][N];
double D[M];

double sing(double y);
double calc(double* X, double* W);

int main(){
	
	X[0][0]=0;
	X[0][1]=0;
	D[0]=-1;
	
	
	X[1][0]=0;
	X[1][1]=1;
	D[1]=1;
	
	
	X[2][0]=1;
	X[2][1]=0;
	D[2]=1;
	
	
	X[3][0]=1;
	X[3][1]=1;
	D[3]=-1;
	
	for(int i=0;i<N;i++){
		W[i]=0;
	}
	
	for(int cnt=0;cnt<L;cnt++){
		for(int m=0;m<M;m++){
			double y;
			y=calc(X[m],W);
			
			if(y*D[m]<0){
				for(int n=0;n<N;n++){
					W[n]+= a*D[m]*X[m][n];
				}
				W[N]+=a*D[m];
			}
		}
	}
	
	double Y[N];
	double y;
	
	while(true){
		cin>>Y[0]>>Y[1];
		system("cls");
		cout<<Y[0]<<"  XOR  "<<Y[1]<<"  =  "<< calc(Y,W) <<"\n\n"<<"Enter new data: "<< endl;		
	} 
	return 0;
}

double sign(double y){
	if(y >= 0) 
		return 1;
	else 
		return -1;
}

double calc(double* X,double* Y){
	double result=0;
	
	for(int i=0;i<N;i++){
		result+=X[i]*W[i];
	}
	
	result+=W[N];
	result=sign(result);
	
	return result; 
}
