TARGET1 = one
TARGET2 = two
TARGET3 = three
all:
	gcc -o $(TARGET1) partB1.c -lpthread
	gcc -o $(TARGET2) partB2.c -lpthread
	gcc -o $(TARGET3) partB3.c -lpthread

clean:
	rm $(TARGET1) $(TARGET2) $(TARGET3)
	$(info Binary files has been cleaned from this directory)
