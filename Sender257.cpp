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
	cout << "Sender 257 PID Number: " << getpid() << endl;


	int qid = msgget(ftok(".",'u'), 0);// connects to msgQ created by Receiver2

	// declare my message buffer
	struct buf {
		long mtype;// required
		int number;// mesg content
	};
	buf msg;// creates a message buffer
	int size = sizeof(msg) - sizeof(long);// get the size of the message


	int count = 0;
	int randNum = 257;
	bool end = false;

	msgrcv(qid, (struct msgbuf *)&msg, size, 20, 0);

	while(!end){
		msg.number = randNum;	
		msg.mtype = 257;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		count++;
		cout << "Message Number: " << count << " has been sent" << endl;

		randNum = 101;
		while(randNum % 257 != 0){
			randNum = rand();
		}
		msgrcv(qid, (struct msgbuf *)&msg, size, 20, 0);
		if(msg.number == 99)
			end = true;
		
	}

	cout << getpid() << ":(Sender257) now exits" << endl;

	exit(0);
}
