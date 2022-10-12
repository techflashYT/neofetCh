#include <sys/utsname.h>   /* Header for 'uname'  */
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <stdint.h>

#include <signal.h> // for debugging
int main()
{
	// Move the cursor 42 characters to the right
	printf("\x1b[42C");
	struct utsname unamePointer;

	uname(&unamePointer);

	struct passwd* p = getpwuid(getuid());  // Check for NULL!

	char hostname[65];
	memset(hostname, '\0', 65);
	gethostname(hostname, sizeof(hostname));  // Check the return value!

	// Print "username@hostname"
	printf("%s@%s\n", p->pw_name, hostname);
	// Move cursor back to col 42
	printf("\x1b[42C");

	int lengthOfLine = 0;
	lengthOfLine += strlen(p->pw_name) + strlen(hostname) + 1;

	for (size_t i = 0; i < lengthOfLine; i++)
	{
		putchar('-');
	}
	putchar('\n');
	// Move cursor back to col 42
	printf("\x1b[42COS: ");
	FILE* ptr;
	size_t size;
	ptr = fopen("/etc/os-release", "r");

	// Allocate memory for the file
	char *fileBuffer = malloc(sizeof(char) * 1024);

	// Zero out the memory
	memset(fileBuffer, '\0', sizeof(char) * 1024);

	// Get file size
	fseek(ptr, 0L, SEEK_END);
	size = ftell(ptr);
	fseek(ptr, 0L, SEEK_SET);

	// Read the file
	fread(fileBuffer, size, 1, ptr);
	fclose(ptr);
	char *osName = malloc(64);
	memset(osName, '\0', 64);

	char *subStrStart = strstr(fileBuffer, "PRETTY_NAME=");
	if (subStrStart == NULL)
	{
		strcpy(osName, "Unkown");
	}
	else
	{
		subStrStart += 13; // ignore `PRETTY_NAME="`
		for (uint16_t i = 0; subStrStart[i] != '\"'; i++) {
			osName[i] = subStrStart[i];
		}
	}
	
	puts(osName);

	free(osName);
	// Move cursor back to col 42
	printf("\x1b[42CKernel: %s\n", unamePointer.release);
	printf("\x1b[42CUptime: ");

	ptr = fopen("/proc/uptime", "r");

	// Zero out the memory
	memset(fileBuffer, '\0', sizeof(char) * 1024);

	// Get file size
	size = 32;

	// Read the file
	fread(fileBuffer, size, 1, ptr);
	fclose(ptr);
	uint8_t i = 0;
	for (; fileBuffer[i] != ' '; i++) {

	}
	fileBuffer[i] = '\0';
	double uptime = atof(fileBuffer);

	printf("%.0f seconds\r\n", uptime);
	free(fileBuffer);
}