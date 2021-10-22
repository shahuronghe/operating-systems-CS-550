TARGET1 = one
TARGET2 = two
TARGET3 = three
all:
	gcc -o $(TARGET1) partC1.c -lpthread
	gcc -o $(TARGET2) partC2.c -lpthread
	gcc -o $(TARGET3) partC3.c -lpthread

clean:
	rm $(TARGET1) $(TARGET2) $(TARGET3)
	$(info Binary files has been cleaned from this directory)
