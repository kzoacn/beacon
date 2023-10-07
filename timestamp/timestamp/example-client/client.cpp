#include <iostream>
#include <signal.h>
#include "TCPClient.h"
#include "base64.hpp"

TCPClient tcp;
using websocketpp::base64_encode;
using websocketpp::base64_decode;

void sig_exit(int s)
{
	tcp.exit();
	exit(0);
}

int main(int argc, char *argv[])
{
	if(argc != 3) {
		cerr << "Usage: ./client ip port" << endl;
		return 0;
	}
	signal(SIGINT, sig_exit);


	char m[64];
	FILE *fm=fopen("message","rb");
    fseek(fm, 0L, SEEK_END);
    int sz = ftell(fm);
    rewind(fm);
    fread(m,1,sz,fm);
	fclose(fm);
	string message;
	for(int i=0;i<sz;i++)
		message+=m[i];

	tcp.setup(argv[1],atoi(argv[2]));
	
	tcp.Send(base64_encode(message));
	string rec = base64_decode(tcp.receive());
	if( rec != "" )
	{
		FILE *fsig=fopen("signature","wb");
		fwrite(rec.c_str(),1,rec.length(),fsig);
		fclose(fsig);
	}

	return 0;
}
