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
    ofstream fres("result");
    ifstream fgraph("graph");
    fgraph>>n>>m;
    int NUM=-sec*log(2)/log(1.0*(m-1)/m); 
    cerr<<argc<<" "<<n<<" "<<m<<" "<<sec<<endl;
    for(int i=0;i<n;i++)
    for(int j=0;j<n;j++)
        fgraph>>G[i][j];

    RandomnessBuffer pg("random");
    
    ifstream fcom("commit");
    ifstream fi("open");

    for(int T=0;T<NUM;T++){ 
        memset(open,0,sizeof open);

        int u=pg.rand()%n;
        int v=pg.rand()%n;
        open[u]=1;
        open[v]=1;
        int c[2],cnt=0;
        c[0]=c[1]=-1;
        for(int i=0;i<n;i++){
            string com;
            int xored;
            fcom>>com;
            fcom>>xored;

            unsigned char r[32];
            if(open[i]){
                for(int j=0;j<32;j++){
                    int x;
                    fi>>hex>>x;
                    r[j]=x;
                } 
                c[cnt++]=xored^r[0];
                string h;
                hash256_hex_string(r,r+32,h);
                if(h!=com){
                    fres<<"Error! commitments do not match"<<endl;
                    puts("Error! commitments do not match");
                    exit(-1);
                }
            }
        }
        if(G[u][v] && c[0]==c[1]){
            //puts("failed");
            fres<<"failed"<<endl;
            cout<<u<<" "<<v<<endl;
            cout<<c[0]<<" "<<c[1]<<endl;
            exit(-1);
        }
    }
    cout<<"verify.. Yes"<<endl;
    fres<<"Yes"<<endl;
}
