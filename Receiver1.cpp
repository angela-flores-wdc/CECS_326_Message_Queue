#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
using namespace std;

int main() {

	cout << "Reciever1 is Connected\tPID: " << getpid() << endl;// show Reciever1 is running

	int qid = msgget(ftok(".",'u'), 0);// connects to msgQ created by Receiver2

	// declare my message buffer
	struct buf {
		long mtype;// required
		int number;// mesg content
	};
	buf msg;// creates a message buffer
	int size = sizeof(msg) - sizeof(long);// get the size of the message


	bool Sender997 = false;// checks if Sender997 has terminated
	bool Sender251 = false;// checks if Sender251 has terminated

	msgrcv(qid, (struct msgbuf *)&msg, size, 21, 0);// received message from Receiver2

	while(Sender251 == false || Sender997 == false){// while both Sender251 and Sender997 have not been terminated
		if(Sender251 == false){// if Sender251 has not been terminated
			msgrcv(qid, (struct msgbuf *)&msg, size, 251, 0);// received message from Sender251 	
			cout << "Sender's Identity: " << msg.mtype << "\tSender's Value: " << msg.number << endl;// display sender's identity and value
			if(msg.number == 0){//
				Sender251 = true;// Sender251 has been terminated
			}
			else{
				msg.mtype = 22;//

				cout << "Sending Sender251 a Message" << endl;
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			}
		}
		if(Sender997 == false){// if Sender997 has not been terminated
			msgrcv(qid, (struct msgbuf *)&msg, size, 997, 0);// receives message from Sender997
			cout << "Sender's Identity: " << msg.mtype << "\tSender's Value: " << msg.number << endl;// display sender's identity and value
			if(msg.number == 99){//
				Sender997 = true;// Sender997 has been terminated
			}
			else{
				msg.mtype = 23;//

				cout << "Sending Sender997 a Message" << endl;
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			}
		}
	}

	cout << "Exit Reciever1" << endl;// sends message that Reciever1 has been terminated

	exit(0);// exits program
}


