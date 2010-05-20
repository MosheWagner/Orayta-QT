#Install books

COPY          = cp -fr
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

install_books:
	@$(CHK_DIR_EXISTS) /usr/share/ToratEmet/Books || $(MKDIR) /usr/share/ToratEmet/Books
	$(COPY) * /usr/share/ToratEmet/Books
	chmod 777 /usr/share/ToratEmet/Books
	
	find * -prune | grep -v "Makefile" | sed 's\^\/usr/share/ToratEmet/Books/\g' | sed 's/\(.*\)/\x27\1\x27/g' > installedfiles
	chmod 777 installedfiles

uninstall_books: 
	xargs rm -r < installedfiles
	rm installedfiles

install: install_books

uninstall: uninstall_books
