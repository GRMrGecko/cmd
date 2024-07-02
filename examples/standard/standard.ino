#include <cmd.h>

// Global command variable.
Cmd *cmd;

// Command entered was invalid, or help is being requested.
void cmd_unrecognized(Cmd *thisCmd, char *command, bool printHelp) {
	// If help is being requested, print the available commands.
	if (printHelp) {
		// Get the command parameters.
		size_t size = cmd->GetSize();	
		PGM_P *commands = cmd->GetCmds();

		// Print each command.
		Serial.println("Available commands:\n");
		for (int i = 0; i < size && commands[i] != NULL; i++) {
			char buf[100];
			sprintf_P(buf, commands[i]);
			Serial.println(buf);
		}
		// Stop here.
		return;
	}
	// No help was requested, so the command provided likely doesn't exist.
	Serial.print("Unrecognized command [");
	Serial.print(command);
	Serial.println("]");
}

// Simple echo ping command.
void cmd_pi(Cmd *thisCmd, char *command, bool printHelp) {
	// If help was requested, print the help for this command.
	if (printHelp) {
		Serial.print(command);
		Serial.println(" *");
		return;
	}
	// Echo back the buffer.
	Serial.println(cmd->GetBuffer());
}

// Demo send command.
void cmd_send(Cmd *thisCmd, char *command, bool printHelp) {
	// If help was requested, print the help.
	if (printHelp) {
		Serial.print(command);
		Serial.println(" address code");
		return;
	}

	// Parse the next available argument.
	char *parsed = cmd->Parse();
	if (parsed == NULL) {
		Serial.println("Invalid address");
		return;
	}
	// Parse integer.
	int address = atoi(parsed);

	// Get the next argument.
	parsed = cmd->Parse();
	if (parsed == NULL) {
		Serial.println("Invalid code");
		return;
	}
	// Parse char.
	unsigned char code = atoi(parsed);

	// Print parsed arguments.
	Serial.print("Sending code: ");
	Serial.print(code);
	Serial.print(" to <");
	Serial.print(address);
	Serial.println(">");
}

void setup() {
	// Setup serial interface.
	Serial.begin(9600);

	// Initialize the command line with 2 commands.
	cmd = new Cmd(2, cmd_unrecognized);
	
	// Add commands.
	cmd->AddCmd(PSTR("pi"), cmd_pi);
	cmd->AddCmd(PSTR("send"), cmd_send);

	// Print a line indicator to inform the user the cli is ready.
	Serial.print(cmd->GetLineIndicator());
}

void loop() {
	// Run the command line loop.
	cmd->Loop();
}
