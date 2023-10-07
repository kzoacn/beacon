#include <iostream>
#include <csignal>
#include <ctime>
#include <string>
#include "TCPServer.h"
#include "base64.hpp"

using websocketpp::base64_encode;
using websocketpp::base64_decode;
TCPServer tcp;
pthread_t msg1[MAX_CLIENT];
int num_message = 0;
int time_send   = 2700;

void close_app(int s) {
	tcp.closed();
	exit(0);
}

void * send_client(void * m) {
        struct descript_socket *desc = (struct descript_socket*) m;

	while(1) {
		if(!tcp.is_online() && tcp.get_last_closed_sockets() == desc->id) {
			cerr << "Connessione chiusa: stop send_clients( id:" << desc->id << " ip:" << desc->ip << " )"<< endl;
			break;
		}
		unsigned long long t = std::time(0);

		string message=base64_decode(desc->message);
		//cerr << "decoding" <<endl;
		//cerr << desc->message <<" "<< message << endl;
		FILE *fm=fopen("message","wb");
		fwrite(message.c_str(),1,message.length(),fm);
		fclose(fm);
		system("./aigis_new s");

		unsigned char sm[3000];
		FILE *fsig=fopen("signature","rb");
		fseek(fsig, 0L, SEEK_END);
		int smlen=ftell(fsig);
		rewind(fsig);
		fread(sm,1,smlen,fsig);
		cerr << smlen << endl;
		auto enc=base64_encode(sm,smlen); 
		tcp.Send(enc, desc->id); 

		fclose(fsig);
		sleep(time_send);
		tcp.clean(desc->id);
		break;
	}
	pthread_exit(NULL);
	return 0;
}

void * received(void * m)
{
        pthread_detach(pthread_self());
	vector<descript_socket*> desc;
	while(1)
	{
		desc = tcp.getMessage();
		for(unsigned int i = 0; i < desc.size(); i++) {
			if( desc[i]->message != "" )
			{
				if(!desc[i]->enable_message_runtime) 
				{

					desc[i]->enable_message_runtime = true;
			                if( pthread_create(&msg1[num_message], NULL, send_client, (void *) desc[i]) == 0) {
						cerr << "ATTIVA THREAD INVIO MESSAGGI" << endl;
					}
					num_message++;
					// start message background thread



				cout << "id:      " << desc[i]->id      << endl
				     << "ip:      " << desc[i]->ip      << endl
				     << "message: " << desc[i]->message << endl
				     << "socket:  " << desc[i]->socket  << endl
				     << "enable:  " << desc[i]->enable_message_runtime << endl;

				}
			}
		}
		usleep(1000);
	}
	return 0;
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cerr << "Usage: ./server port (opt)time-send" << endl;
		return 0;
	}
	if(argc == 3)
		time_send = atoi(argv[2]);
	std::signal(SIGINT, close_app);

	pthread_t msg;
        vector<int> opts = { SO_REUSEPORT, SO_REUSEADDR };

	if( tcp.setup(atoi(argv[1]),opts) == 0) {
		if( pthread_create(&msg, NULL, received, (void *)0) == 0)
		{
			while(1) {
				tcp.accepted();
				cerr << "Accepted" << endl;
			}
		}
	}
	else
		cerr << "Errore apertura socket" << endl;
	return 0;
}
