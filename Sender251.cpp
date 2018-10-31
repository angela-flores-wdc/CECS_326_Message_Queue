#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include "get_info.h"
using namespace std;

int main() {

	cout << "Sender251 is Connected\tPID Number: " << getpid() << endl;// show Sender251 is running and display process PID  


	int qid = msgget(ftok(".",'u'), 0);// connects to msgQ created by Receiver2

	// declare my message buffer
	struct buf {
		long mtype;// required
		int number;// mesg content
	};
	buf msg;// creates a message buffer
	int size = sizeof(msg) - sizeof(long);// get the size of the message

	bool terminate = false;// checks if Sender251 has been terminated

	get_info(qid, (struct msgbuf *)&msg, size, 251);// call function get_info
	
	msgrcv(qid, (struct msgbuf *)&msg, size, 22, 0);// received message from Receiver2

	while(terminate == false){// while Sender251 has not been terminated

		int randNum = 114;
		while(randNum % 251 != 0){// while random number is not divisible by 251
			randNum = rand();// generate new random number
		}

		msg.number = randNum;
		msg.mtype = 251;

		cout << "Sending Message to Receiver1" << endl;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);

		cout << "Received Message From Receiver1" << endl;
		msgrcv(qid, (struct msgbuf *)&msg, size, 22, 0);
	}

	cout << "Exit Sender251" << endl;// sends message that Sender251 has been terminated  

	exit(0);// exits program
}

