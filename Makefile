# $Id$

.PHONY: all distclean ling clean ./Server config

DIRS = ./Server

PLATFORM = $$(echo $$(uname -s)-$$(echo $$(uname -r) | cut -d'.' -f1) | tr '[:upper:]' '[:lower:]')
CACHED = if [ ! "x`test -f config.cache && cat config.cache`" = "x$(PLATFORM)" ]; then
END = ; fi

FLAGS = TOPDIR=..

all: $(DIRS)

./Server:: config
	$(MAKE) -C $@ $(FLAGS)

config:
	@$(CACHED) test -f Config/$(PLATFORM) || echo "Seems like config file for your platform does not exist, so I will ask you questions" $(END)
	@$(CACHED) ./config `test -f Config/$(PLATFORM) && echo Config/$(PLATFORM)` $(END)
	@$(CACHED) $(MAKE) clean $(END)
	@$(CACHED) echo $(PLATFORM) > config.cache $(END)

clean:
	for i in $(DIRS); do \
		$(MAKE) -C $$i clean $(FLAGS) ; \
	done

distclean:
	touch config.mk
	for i in $(DIRS); do \
		$(MAKE) -C $$i clean $(FLAGS) ; \
	done
	rm -f config.h config.mk config.cache
