all: control.o write.o
	gcc -o control control.o
	gcc -o write write.o

control: control.c
	gcc -c control.c

write: write.c
	gcc -c write.c

runc:
	./control create

runr:
	./control remove

runw:
	./write

clean:
	rm *.o