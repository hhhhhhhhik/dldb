TOP_DIR := .
SUBDIRS := common client server

.PHONY: subdirs $(SUBDIRS)

all: subdirs

subdirs: $(SUBDIRS)

$(SUBDIRS):
	cd $@ && $(MAKE)

clean::
	rm -f ../bin/*
	@for subdir in $(SUBDIRS); do \
		( cd $$subdir && $(MAKE) $@) ; \
	done
