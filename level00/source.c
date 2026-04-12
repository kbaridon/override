#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int attempt[4];

	puts("***********************************");
	puts("* \t     -Level00 -\t\t  *");
	puts("***********************************");
	printf("Password:");
	scanf("%d", attempt);
	if (attempt[0] != 5276)
		return (puts("\nInvalid Password!"), 1);
	puts("\nAuthenticated!");
	system("/bin/sh");

	return 0;
}
