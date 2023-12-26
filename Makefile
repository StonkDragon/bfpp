build:
	clang -std=c89 -Wall -Werror -Wextra -pedantic bf.c -o bf

justbuild:
	clang bf.c -o bf
