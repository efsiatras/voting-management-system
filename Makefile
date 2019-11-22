CC=gcc
CFLAGS=-Wall -lm -g

OBJECTS=runelection.o election.o utils/file.o utils/primenumber.o utils/array.o records/record.o bf/bf.o rbt/rbt.o postcodelist/postcodelist.o postcodelist/recordlist.o

runelection: $(OBJECTS)
	$(CC) $(OBJECTS) -o runelection $(CFLAGS)

runelection.o: runelection.c
	$(CC) -c runelection.c $(CFLAGS)

election.o: election.c
	$(CC) -c election.c $(CFLAGS)

utils/file.o: utils/file.c
	$(CC) -c utils/file.c -o utils/file.o $(CFLAGS)

utils/primenumber.o: utils/primenumber.c
	$(CC) -c utils/primenumber.c -o utils/primenumber.o $(CFLAGS)

utils/array.o: utils/array.c
	$(CC) -c utils/array.c -o utils/array.o $(CFLAGS)

records/record.o: records/record.c
	$(CC) -c records/record.c -o records/record.o $(CFLAGS)

bf/bf.o: bf/bf.c
	$(CC) -c bf/bf.c -o bf/bf.o $(CFLAGS)

rbt/rbt.o: rbt/rbt.c
	$(CC) -c rbt/rbt.c -o rbt/rbt.o $(CFLAGS)

postcodelist/recordlist.o: postcodelist/recordlist.c
	$(CC) -c postcodelist/recordlist.c -o postcodelist/recordlist.o $(CFLAGS)

postcodelist/postcodelist.o: postcodelist/postcodelist.c
	$(CC) -c postcodelist/postcodelist.c -o postcodelist/postcodelist.o $(CFLAGS)

.PHONY: clean

clean:
	rm -f runelection $(OBJECTS)