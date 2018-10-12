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
	srand(time(NULL));
	cout << "Sender 997 PID Number: " << getpid() << endl;


	int qid = msgget(ftok(".",'u'), 0);// connects to msgQ created by Receiver2

	// declare my message buffer
	struct buf {
		long mtype;// required
		int number;// mesg content
	};
	buf msg;// creates a message buffer
	int size = sizeof(msg) - sizeof(long);// get the size of the message


	int count = 0;
	int randNum = 997;
	bool end = false;
	bool over = false;
	bool rec1 = true;
	bool rec2 = true;
	msgrcv(qid, (struct msgbuf *)&msg, size, 23, 0);
	while(!over){
		if(end)
			over = true;
		if(rec2){
			if(end)
				msg.number = 99;
			else
				msg.number = randNum;	
			msg.mtype = 997;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			count++;
			cout << "Rec2 Message Number: " << count << " has been sent" << endl;
			if(!end){
				msgrcv(qid, (struct msgbuf *)&msg, size, 23, 0);
				if(msg.number == 99){
					rec2 = false;
					cout << "Receiver2 is ending~~~~~~~~~~~~" << endl;
				}
			}
		}
		if(rec1){
			if(end)
				msg.number = 99;
			else
				msg.number = randNum;	
			msg.mtype = 998;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			count++;
			cout << "Rec1 Message Number: " << count << " has been sent" << endl;
			if(!end)
				msgrcv(qid, (struct msgbuf *)&msg, size, 23, 0);
		}
		randNum = 101;
		while(randNum % 997 != 0){
			randNum = rand();
			if(randNum < 100){
				end = true;
				cout << "Sender997 is done, sending final message" << endl;
			}
		}
	}
	
	cout << getpid() << ": now exits" << endl;

	exit(0);
}
