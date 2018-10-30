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

	cout << "Sender257 is Connected\tPID Number: " << getpid() << endl;// show Sender257 is running

	int qid = msgget(ftok(".",'u'), 0);// connects to msgQ created by Receiver2

	// declare my message buffer
	struct buf {
		long mtype;// required
		int number;// mesg content
	};

	buf msg;// creates a message buffer
	int size = sizeof(msg) - sizeof(long);// get the size of the message

	int randNum = rand();// generate random number
	bool terminate = false;// checks if Sender257 has been terminated

	msgrcv(qid, (struct msgbuf *)&msg, size, 20, 0);// received message from Receiver2

	while(terminate == false){// while Sender257 has not been terminated
		msg.number = randNum;	
		msg.mtype = 257;
		
		cout << "Sending Message To Receiver2" << endl;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);

		while(randNum % 257 != 0){// while random number is not divisible by 257
			randNum = rand();// generate new random number
		}
		msgrcv(qid, (struct msgbuf *)&msg, size, 20, 0);// received message from Receiver2
		if(msg.number == 99)// msg.number == 99 when Receiver2 has been terminated
			terminate = true;// terminate Sender257	
	}

	cout << "Exit Sender257" << endl;// sends message that Sender257 has been terminated

	exit(0);// exits program
}