## Copyright(c) 2022 Sreesankar - All Rights Reserverd
#
# @version 0.1
SHELL:=/bin/bash
build:
	gcc main.c menu.c -o main
	size main
slowrun:
	gcc main.c menu.c -DSLOW -o main
	size main
	socat -d -d pty,raw,echo=0 pty,raw,echo=0 2>&1| grep -o --line-buffered '/dev/pts/[0-9]*' |\
	tee >(head -1 |xargs -I{} ./main {} ) >(head -n 2|tail -1 |xargs -I{} putty -serial {})

run: build
	socat -d -d pty,raw,echo=0 pty,raw,echo=0 2>&1| grep -o --line-buffered '/dev/pts/[0-9]*' |\
	tee >(head -1 |xargs -I{} ./main {} ) >(head -n 2|tail -1 |xargs -I{} putty -serial {})
screen: build
	socat -d -d pty,raw,echo=0 pty,raw,echo=0 2>&1| grep -o --line-buffered '/dev/pts/[0-9]*' |\
	tee >(head -1 |xargs -I{} ./main {} ) >(head -n 2|tail -1 |xargs -I{} alacritty -e screen {} 9600)

# end
