#include <iostream>
#include <fstream>
#include <cstring> 
#include "picosha2.h"
using namespace picosha2;
using namespace std; 
int n,m,sec;
int color[601];
int G[601][601];
int open[601];
int main(int argc,char **argv){
    sscanf(argv[1],"%d",&n);
    sscanf(argv[2],"%d",&m);
    sscanf(argv[3],"%d",&sec);
    cerr<<argc<<" "<<n<<" "<<m<<" "<<sec<<endl;
    int NUM=-sec*log(2)/log(1.0*(m-1)/m);

    RandomnessBuffer pg("random");
    //PRG pg(0);

    ifstream fans("answer");
    for(int i=0;i<n;i++)
        fans>>color[i];

    RandomnessBuffer prg("ExpansionRandomOutput.txt");
    //PRG prg(0);
    ofstream fo("open");
    for(int T=0;T<NUM;T++){  
        memset(open,0,sizeof open);

        open[pg.rand()%n]=1;
        open[pg.rand()%n]=1;

        int perm[3];
        for(int i=0;i<3;i++)
            perm[i]=i;
        for(int i=1;i<3;i++)
            swap(perm[i],perm[prg.rand()%i]);        
        for(int i=0;i<n;i++){
            string h;
            unsigned char r[32];
            for(int j=0;j<32;j++)
                r[j]=prg.rand()&0xff;
            if(open[i]){
                for(int j=0;j<32;j++)   
                    fo<<hex<<(int)r[j]<<" ";
                fo<<endl;
            }
        }
    }
}
