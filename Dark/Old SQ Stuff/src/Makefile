CC      = gcc -g
PROF    =
NOCRYPT =
C_FLAGS = -O -Wall -ggdb $(PROF) $(NOCRYPT)
L_FLAGS = -O -ggdb -lcrypt $(PROF)

TEST_FILE = test
REAL_FILE = executable

O_FILES = act_comm.o act_info.o act_move.o act_obj.o act_wiz.o \
	  classes.c change.o comm.o const.o db.o emote.o environment.o \
          fight.o fileio.o flora.o handler.o interp.o languages.o limited.o \
	  magic.o mem_manage.o memory.o money.o names.o obj_fun.o \
          olc.o races.o random.o review.o rng.o room_fun.o \
	  save.o sectors.o shoot.o shop.o skills.o special.o update.o \
          weather.o

merc: $(O_FILES)
	rm -f $(TEST_FILE)
	$(CC) $(L_FLAGS) -o $(TEST_FILE) $(O_FILES)
	chmod 700 $(TEST_FILE)
#	chmod 4755 $(TEST_FILE)

#	chown robert $(TEST_FILE)
#	chgrp sq $(TEST_FILE)
#	chmod 4755 $(TEST_FILE)

real: merc
	cp $(TEST_FILE) $(REAL_FILE)

clean:
	rm *.o

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<
