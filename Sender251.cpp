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
	cout << "Sender 251 PID Number: " << getpid() << endl;


	int qid = msgget(ftok(".",'u'), 0);// connects to msgQ created by Receiver2

	// declare my message buffer
	struct buf {
		long mtype;// required
		int number;// mesg content
	};
	buf msg;// creates a message buffer
	int size = sizeof(msg) - sizeof(long);// get the size of the message


	int randNum = 251;
	bool end = false;
	msg.mtype = 251;			
	msg.number = -1;
	get_info(qid, (struct msgbuf *)&msg, size, 251);
	
	msgrcv(qid, (struct msgbuf *)&msg, size, 22, 0);
	int count = 0;

	while(!end){
		msg.number = randNum;
		msg.mtype = 251;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		cout << "Message Number: " << count << " has been sent" << endl;
		count++;
		msgrcv(qid, (struct msgbuf *)&msg, size, 22, 0);
		randNum = 101;
		while(randNum % 251 != 0){
			randNum = rand();
		}
	}

	cout << getpid() << " (Send251): now exits" << endl;

	exit(0);
}


