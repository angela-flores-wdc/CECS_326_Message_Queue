#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {

	cout << "Sender997 is Connected\tPID Number: " << getpid() << endl;// show Sender997 is running

	int qid = msgget(ftok(".",'u'), 0);// connects to msgQ created by Receiver2

	// declare my message buffer
	struct buf {
		long mtype;// required
		int number;// mesg content
	};
	
	buf msg;// creates a message buffer
	int size = sizeof(msg) - sizeof(long);// get the size of the message

	bool terminate = false;// keeps track if Sender997 has been terminated
	bool Receiver1 = false;// checks if Receiver1 has been terminated
	bool Receiver2 = false;// checks if Receiver2 has been terminated

	msgrcv(qid, (struct msgbuf *)&msg, size, 23, 0);// received message from Receiver2

	while(terminate == false){// while Sender997 has not been terminated
		int randNum = rand();// generate random number
		while(randNum % 997 != 0){// while random number is not divisible by 997
			randNum = rand();// generate new random number
			if(randNum < 100){// if random number is smaller than 100
				terminate = true;// terminate Sender 997
				cout << "Sending Last Message to Receivers, Then Terminating Sender997" << endl;
			}
		}
		if(Receiver2 == false){// if Receiver2 has not been terminated
			if(terminate == true)//if Sender997 is going to terminate
				msg.number = 99;
			else
				msg.number = randNum;	
			msg.mtype = 997;

			cout << "Sending Receiver2 a Message" << endl;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);

			if(terminate == false){// if Sender997 has not been terminated
				msgrcv(qid, (struct msgbuf *)&msg, size, 23, 0);// received message from Receiver2
			}
		}
		if(Receiver1 == false){// if Receiver1 has not been terminated
			if(terminate == true)// if Sender997 is going to terminate
				msg.number = 99;
			else
				msg.number = randNum;	
			msg.mtype = 997;

			cout << "Sending Receiver1 a Message" << endl;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);

			if(terminate == false)// if Sender997 has not been terminated
				msgrcv(qid, (struct msgbuf *)&msg, size, 23, 0);// received message from Receiver1
		}
	}
	
	cout << "Exit Sender997" << endl;// sends message that Sender997 has been terminated

	exit(0);// exits program
}
