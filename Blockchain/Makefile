# -*- MakeFile -*- 

blockchain: main.o functions.o
	gcc main.o functions.o -o blockchain -g

main.o: main.c header.h
	gcc -c main.c -g

functions.o: functions.c header.h
	gcc -c functions.c -g

clean:
	rm -f *.o blockchain
