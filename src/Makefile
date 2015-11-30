INSTALLDIR = /opt/free-music

DESKTOPDIR = ~/.local/share/applications

CC ?= gcc

PKG_CONFIG = $(shell which pkg-config)

CFLAGS = $(shell $(PKG_CONFIG) --cflags gtk+-2.0 gstreamer-0.10)

LIBS = $(shell $(PKG_CONFIG) --libs gtk+-2.0 gstreamer-0.10)

SRC = main.c player.c scan-mp3.c list.c

OBJS = $(SRC:.c=.o)

all: free-music

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $(@F)

free-music: $(OBJS)
	$(CC) $(LIBS) $(OBJS) -o $(@F)

.PHONY: clean install uninstall

clean:
	-rm free-music *.o

install: free-music
	@mkdir -p $(INSTALLDIR); \
	mkdir -p $(INSTALLDIR)/etc; \
	mkdir -p $(INSTALLDIR)/etc/lists.d; \
	cp ../src/free-music $(INSTALLDIR); \
	cp ../resources/*.png $(INSTALLDIR); \
	cp ../icon/free-music.desktop $(DESKTOPDIR); \
	if [ -d $(INSTALLDIR) ]; \
		then \
		chown -R `whoami` $(INSTALLDIR); \
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
