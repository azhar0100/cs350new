#include <stdio.h>
#include <stdlib.h>

char *mystery1_inputA = "hijkl";
char *mystery1_inputB = "hijkl";

char *mystery2_inputA = "hijkl"; 
char *mystery2_inputB = "ABCDE";

int mystery1(char *s1, const char *s2)
{
	for( ; *s1 != '\0' && *s2 != '\0' ; s1++, s2++ )
		if (*s1 != *s2)
			return 0;
	return 1;
}

int mystery2(char *s1, const char *s2)
{
	while(*s1 != '\0')
		++s1;
	for( ; *s1 = *s2 ; s1++, s2++ )
		; /* empty statement */
}

int main(void)
{

	int mystery1_retval = -1;
	int mystery2_retval = -1;

/*	mystery1_retval = mystery1(mystery1_inputA, mystery1_inputB); */
	mystery2_retval = mystery2(mystery2_inputA, mystery2_inputB);

	printf("Mystery1 reval = %d\n  String1: %s\n  String2: %s\n", mystery1_retval, mystery1_inputA, mystery1_inputB);
	printf("Mystery2 reval = %d\n  String1: %s\n  String2: %s\n", mystery2_retval, mystery2_inputA, mystery2_inputB);


	exit(0);
}
