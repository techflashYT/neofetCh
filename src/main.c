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
	struct utsname unamePointer;

	uname(&unamePointer);

	struct passwd* p = getpwuid(getuid());  // Check for NULL!

	char hostname[65];
	memset(hostname, '\0', 65);
	gethostname(hostname, sizeof(hostname));  // Check the return value!

	// Print "username@hostname"
	printf("%s@%s\n", p->pw_name, hostname);

	int lengthOfLine = 0;
	lengthOfLine += strlen(p->pw_name) + strlen(hostname) + 1;

	for (size_t i = 0; i < lengthOfLine; i++)
	{
		putchar('-');
	}

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

	char *osName = malloc(64);

	char *ret = strstr(fileBuffer, "PRETTY_NAME=");
	strcpy(osName, "Unkown");
	char *ch = "\0\0";
	if (ret != NULL)
	{
		fseek(ptr, 12, SEEK_CUR);
		strcpy(osName, "\0\0\0\0\0\0\0");
		for (uint8_t i = 0; *(ch + i) != '\r' || *(ch + i) != '\n'; i++)
		{
			fread(ch + i, 1, 1, ptr);
			printf("address: 0x%x\r\ndref: %c\r\n\r\n", (ch + i), *(ch + i));
			strcat(osName, (ch + i));
		}
	}
	
	printf("\nOS: %s\n", osName);
	printf("Kernel: %s", unamePointer.release);

}