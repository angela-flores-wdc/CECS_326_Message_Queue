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
	cout << "Reciever 1 PID: " << getpid() << endl;


	int qid = msgget(ftok(".",'u'), 0);// connects to msgQ created by Receiver2

	// declare my message buffer
	struct buf {
		long mtype;// required
		int number;// mesg content
	};
	buf msg;// creates a message buffer
	int size = sizeof(msg) - sizeof(long);// get the size of the message


	bool termSend997 = false;// checks if Sender997 has terminated
	bool termSend251 = false;// checks if Sender251 has terminated

	msgrcv(qid, (struct msgbuf *)&msg, size, 21, 0);

	while(termSend251 == false || termSend997 == false){// while both Sender251 and Sender997 have not been terminated
		if(termSend251 == false){// if Sender251 has not been terminated
			msgrcv(qid, (struct msgbuf *)&msg, size, 251, 0);		
			cout << "Message From " << msg.mtype << " : " << msg.number << endl;
			if(msg.number == 0){
				termSend251 = true;
			}
			else{
				msg.mtype = 22;
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			}
		}
		if(termSend997 == false){// if Sender997 has not been terminated
			msgrcv(qid, (struct msgbuf *)&msg, size, 998, 0);
			cout << "Message From " << msg.mtype << " : " << msg.number << endl;
			if(msg.number == 99){
				termSend997 = true;
			}
			else{
				msg.mtype = 23;
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			}
		}
		
	}

	cout << "Reciever 1 has finished it's task. ~~~~~~~~~~~~~~~~~~~" << endl;

	exit(0);
}
