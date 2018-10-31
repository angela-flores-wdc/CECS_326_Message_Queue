/*
msgget() function that returns a message queue identifier :   int msgget(key_t key, int msgflg)
ftok() function creates an IPC (interprocess communication) key to use as a parameter for msgget
msgsnd()
msgrcv()
*/
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

	cout << "Reciever2 Has Started\tPID: " << getpid() << endl;// show Reciever2 is running

	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	// declare my message buffer
	struct buf {
		long mtype;// required
		int number;// mesg content
	};
	
	buf msg;// creates a message buffer
	int size = sizeof(msg) - sizeof(long);// size of the msg

	char temp; // temp variable for user input
	bool Sender997 = false;// checks if Sender997 has been terminated
	int nMessages = 0;// counts how many messages have been received

	// makes program wait for other programs to connect before continuing with the rest of the code
	cout << "Enter Any Single Key to Start Program " << endl;// prompts user to start the program
	cin >> temp;// user inputs random char to continue program

	// Sends first message to the other programs
	for(int i = 20; i <= 23; i++){
		msg.number = 114;
		msg.mtype = i;// set type to each sender / receiver
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);// send message to sender / receiver
	}
	
	while(nMessages <= 5000){// while Receiver2 has not received 5000 messages

		cout << "Received Message From Sender257" << endl;
		msgrcv(qid, (struct msgbuf *)&msg, size, 257, 0);

		nMessages++;// increment number of messages received
		cout << "Sender's Identity: " << msg.mtype << "\tSender's Value: " << msg.number << endl;// display sender's identity and value

		if(nMessages > 5000){// checks if Receiver2 have recieved 5000 messages
			msg.number = 99;// used to let other programs know Receiver2 has been terminated
			Sender997 = true;// terminate Sender997
		}
		else
			msg.number = 114;
		msg.mtype = 20;// Set type to Sender257

		cout << "Sending Sender257 a Message" << endl;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	
		if(Sender997 == false){// if Sender997 has been terminated

			cout << "Received Message From Sender997" << endl;
			msgrcv(qid, (struct msgbuf *)&msg, size, 997, 0);

			nMessages++;// increment number of messages received
			cout << "Sender's Identity: " << msg.mtype << "\tSender's Value: " << msg.number << endl;// display sender's identity and value
			
			if(msg.number == 99){// if Receiver2 is terminated
				Sender997 = true;// terminate Sender997
			}
			if(Sender997 == false){// if Sender997 has not been terminated
				if(nMessages >= 5000)// checks if Receiver2 have recieved 5000 messages
					msg.number = 99;// used to let other programs know Receiver2 has been terminated
				else
					msg.number = 114;
				msg.mtype = 23;// Set type to Sender997

				cout << "Sending Sender997 a Message" << endl;
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			}
		}
	}

	cout << "Exit Reciever2" << endl;// sends message that Receiver2 has been terminated

	// waits for signal to be safe to delete message queue
	cout << "Enter Any Single Key to Delete the Queue." << endl;// prompts user to continue program
	cin >> temp;// user inputs random char to continue program
	msgctl (qid, IPC_RMID, NULL);// deletes message queue

	exit(0);// exits program
}
