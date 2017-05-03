install: builtins.c builtins.h cash.c redir.c redir.h
	gcc -o cash builtins.c cash.c redir.c

clean: cash
	rm cash
run: cash
	rlwrap -a ~/cash/cash
copy: cash
	cp ../cash/ ~/cash/ -rf
	