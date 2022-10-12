#include <sys/utsname.h>   /* Header for 'uname'  */
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <stdint.h>

#include <math.h>

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
	double days = uptime;
	double hours = uptime;
	double minutes = uptime;
	double seconds = uptime;
	if ((((uptime / (double)60.0) / (double)60.0) / (double)24.0) < (double)1.0) {
		if (((uptime / (double)60.0) / (double)60.0) < (double)1.0) {
			// Less than 1m
			if ((uptime / (double)60.0) < (double)1.0) {
				printf("%.0f second%c\r\n", uptime, (uptime != 1) ? 's' : ' ');
			}
			// Less than 1h
			else {
				minutes = (uptime / (double)60.0);
				double junk = 0;
				seconds = modf(minutes, &junk);
				seconds *= 60;
				printf("%u minute%s, %u second%s\r\n", (uint32_t)floor(minutes), ((uint32_t)floor(minutes) != 1) ? "s" : "", (uint32_t)floor(seconds), ((uint32_t)floor(seconds) != 1) ? "s" : "");
			}
		}
		else { // 1h+
			hours = (((uptime / (double)60.0) / (double)60.0));
			double junk = 0;
			minutes = modf(hours, &junk);
			minutes *= 60;
			junk = 0;
			seconds = modf(minutes, &junk);
			seconds *= 60;
			printf("%u hour%s, %u minute%s, %u second%s\r\n", (uint32_t)floor(hours), ((uint32_t)floor(hours) != 1) ? "s" : "", (uint32_t)floor(minutes), ((uint32_t)floor(minutes) != 1) ? "s" : "", (uint32_t)floor(seconds), ((uint32_t)floor(seconds) != 1) ? "s" : "");
		}
	}
	else { // 1 day+
		days = ((((uptime / (double)60.0) / (double)60.0)) / (double)24.0);
		double junk = 0;
		hours = modf(days, &junk);
		hours *= 24;
		junk = 0;
		minutes = modf(hours, &junk);
		minutes *= 60;
		junk = 0;
		seconds = modf(minutes, &junk);
		seconds *= 60;
		printf("%u day%s, %u hour%s, %u minute%s, %u second%s\r\n", (uint32_t)floor(days), ((uint32_t)floor(days) != 1) ? "s" : "", (uint32_t)floor(hours), ((uint32_t)floor(hours) != 1) ? "s" : "", (uint32_t)floor(minutes), ((uint32_t)floor(minutes) != 1) ? "s" : "", (uint32_t)floor(seconds), ((uint32_t)floor(seconds) != 1) ? "s" : "");
	}

	free(fileBuffer);
}