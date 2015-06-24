all: free-music

#compiler
CC = gcc

#cflags
CFLAGES =`pkg-config --cflags --libs gtk+-2.0 gstreamer-0.10`

#where to install
INSTALLDIR = /opt/free-music

#desktop icon directory
DESKTOPDIR = ~/.local/share/applications

#free-music: main.o list.o player.o scan-mp3.o
#	@$(CC) -o free-music main.o list.o player.o scan-mp3.o

#main.o: main.c main.h list.h player.h scan-mp3.h
#	@$(CC) $(INCLUDE) -c main.c

#list.o: list.c main.h list.h
#	@$(CC) $(INCLUDE) -c list.c

#player.o: player.c list.h player.h
#	@$(CC) $(INCLUDE) -c player.c

#scan-mp3.o: scan-mp3.c scan-mp3.h
#	@$(CC) $(INCLUDE) -c scan-mp3.c


free-music:
	@gcc main.c list.c player.c scan-mp3.c -o free-music `pkg-config --cflags --libs gtk+-2.0 gstreamer-0.10`


clean:
	@rm free-music *.o

install: free-music
	@sudo chown `whoami` /opt \
	mkdir -p $(INSTALLDIR); \
	mkdir -p $(INSTALLDIR)/etc; \
	mkdir -p $(INSTALLDIR)/etc/lists.d; \
	mv free-music $(INSTALLDIR); \
	cp *.png $(INSTALLDIR); \
	cp free-music.desktop $(DESKTOPDIR); \
	if [ -d $(INSTALLDIR) ]; \
		then \
		echo "Complete!"; \
	else \
		echo "Not complete!!!"; \
	fi


uninstall:
	@rm -rf $(INSTALLDIR); \
	rm -f $(DESKTOPDIR)/free-music.desktop; \
	if [ -d $(INSTALLDIR) ]; \
		then \
		echo "Not Complete!"; \
	else \
		echo "Complete!!!"; \
	fi
