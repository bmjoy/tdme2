#include <signal.h>

#include <iostream>

#include <tdme/network/udpclient/NIOUDPClient.h>
#include <tdme/network/udpclient/NIOUDPClientMessage.h>
#include <tdme/os/network/Network.h>
#include <tdme/os/threading/Thread.h>

#include <tdme/utils/Console.h>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;

using tdme::utils::Console;
using tdme::os::network::Network;
using tdme::os::threading::Thread;
using tdme::network::udpclient::NIOUDPClient;
using tdme::network::udpclient::NIOUDPClientMessage;

NIOUDPClient* client = NULL;

class InputThread: public Thread {
public:
	InputThread(): Thread("inputthread") {
	}

	virtual void run() {
		while (isStopRequested() == false) {
			string input;
			stringstream* frame = new stringstream();
			cin >> input;
			*frame << input;
			if (client != nullptr) {
				client->sendMessage(client->createMessage(frame), true);
			}
		}
	}
};

InputThread* inputThread = NULL;

void sigHandlerINT(int signal) {
	Console::println("Interrupt signal catched");
	if (inputThread != nullptr) {
		inputThread->stop();
	}
	if (client != nullptr) {
		client->stop();
	}
}

int main(int argc, char *argv[]) {
	// install SIGNINT handler
	if (signal(SIGINT, sigHandlerINT) == SIG_ERR) {
		Console::println("Can't install signal handler for SIGINT");
	}
	// initialize network module
	Network::initialize();

	// input thread
	inputThread = new InputThread();
	inputThread->start();

	// UDP client
	client = new NIOUDPClient("127.0.0.1", 10000);
	client->start();

	// handle incoming messages
	while(client->isStopRequested() == false) {
		Thread::sleep(1L);
		// process incoming messages
		NIOUDPClientMessage* message = client->receiveMessage();
		if (message != nullptr) {
			if (client->processSafeMessage(message) == true) {
				Console::println("Received message: " + message->getFrame()->str());
			}
			delete message;
		}
	}
}
