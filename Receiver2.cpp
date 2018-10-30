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
	char begin;
	cout << "Reciever 2 PID: " << getpid() << endl;


	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	// declare my message buffer
	struct buf {
		long mtype;// required
		int number;// mesg content
	};
	buf msg;// creates a message buffer
	int size = sizeof(msg) - sizeof(long);//size of the msg

    int count = 0;
	bool send997 = true;
	int nMessages = 0;// counts how many messages have been received

	cout << "Enter anything when ready to begin: " << endl;
	cin >> begin;

	for(int i = 20; i <= 23; i++){
		msg.number = 15;
		msg.mtype = i; 
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	}
	
	while(nMessages <= 5000){
		cout << "257 message receive" << endl;
		msgrcv(qid, (struct msgbuf *)&msg, size, 257, 0);
		nMessages++;
		cout << count << "-From: " << msg.mtype << " Message: " << msg.number << endl;
		if(nMessages > 5000){
			cout << "Message limit reached first if" << endl;
			msg.number = 99;
			send997 = false;
		}
		else
			msg.number = 15;
		msg.mtype = 20;
		cout << "sending 257 message" << endl;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	
		if(send997){
			cout << "997 message receive" << endl;
			msgrcv(qid, (struct msgbuf *)&msg, size, 997, 0);
			nMessages++;
			cout << nMessages << "-From: " << msg.mtype << " Message: " << msg.number << endl;
			if(msg.number == 99){
				cout << "997 has finished setting it to false" << endl;
				send997 = false;
			}
			if(send997){
				if(nMessages >= 5000)
					msg.number = 99;
				else
					msg.number = 101;
				msg.mtype = 23;
				cout << "sending 997 message" << endl;
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			}
		}
	}

	cout << "Reciever 2 has finished it's task. ~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Enter any single key to delete the Queue when ready" << endl;
	char over;
	cin >> over;

	// now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);

	exit(0);
}
