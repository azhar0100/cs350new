all :	question1 question2 question3 question4

question1 :	question1.c
	gcc -Wall -o question1 question1.c

question2 :	question2.c
	gcc -Wall -o question2 question2.c

question3 :	question4.c prompt.c
	gcc -Wall -o question3 question3.c

question4 :	question4.c prompt.c
	gcc -Wall -o question4 question4.c

sloc :
	cat *.c | wc -l

clean :
	rm -f question1 question2 question3 question4
