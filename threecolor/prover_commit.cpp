#include <iostream>
#include <fstream>
#include "picosha2.h"
using namespace picosha2;
using namespace std;
//const int n=100,m=300;
int n,m,sec;
int color[601];
int G[601][601];
void gen(){
    ofstream fout("graph");
    ofstream fans("answer");
    fout<<n<<" "<<m<<endl;
    for(int i=0;i<n;i++){
        color[i]=rand()%3;
        fans<<color[i]<<" ";
        for(int j=0;j<n;j++)
            G[i][j]=0;
    }
    fans<<endl;

    int edges=0;
    while(1){
        int x=rand()%n+1;
        int y=rand()%n+1;
        if(color[x]==color[y] || G[x][y])
            continue;
        G[x][y]=G[y][x]=1;
        if(++edges==m)
            break;
    }
    for(int i=0;i<n;i++)
    for(int j=0;j<n;j++)
        fout<<G[i][j]<<" \n"[j+1==n];

    fans.close();
    fout.close();
}

int main(int argc,char **argv){
    sscanf(argv[1],"%d",&n);
    sscanf(argv[2],"%d",&m);
    sscanf(argv[3],"%d",&sec);
    int NUM=-sec*log(2)/log(1.0*(m-1)/m);
    cout<<"repeat number: "<<NUM<<endl;
    gen();
    ifstream fans("answer");
    for(int i=0;i<n;i++)
        fans>>color[i];
        
    RandomnessBuffer prg("ExpansionRandomOutput.txt");
    
    ofstream fo("commit");
    for(int T=0;T<NUM;T++){ 
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
            hash256_hex_string(r,r+32,h);
            fo<<h<<endl;
            fo<<int(r[0]^perm[color[i]])<<endl;
        }
    }
}
