FLAGS = -Wall -std=c99 -O3

FILES = $(wildcard *.c)
HEADERS = $(wildcard *.h)


backbone: $(patsubst %.c,%.o,$(FILES)) $(HEADERS)
	gcc -o backbone $(FLAGS) $(patsubst %.c,%.o,$(FILES)) 

%.o: %.c $(HEADERS)
	gcc -c -o $@ $(patsubst %.o,%.c,$@) $(FLAGS)

clean: 
	rm backbone
	rm *.o