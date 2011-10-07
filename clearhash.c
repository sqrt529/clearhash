/*  clearhash - A program to find the cleartext of crypt and md5 hashes. (Based on permutate.c also written by me in 2006)
    Copyright (C) 2010 Joachim "Joe" Stiegler <blablabla@trullowitsch.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	Version: 0.1

	--

	This is proof of concept code and for testing purposes only!
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>
#include <signal.h>

char *tmpword;

void sighandler() {
	signal(SIGHUP, sighandler);
	puts(tmpword);
}

int usage() {
	printf ("Syntax: clearhash -abcd -l [0-9] -h <hash>\n\n");
	printf (" -a: chars: [a-z]\n");
	printf (" -b: chars: [A-Z]\n");
	printf (" -c: chars: [0-9]\n");
	printf (" -d: chars: [^°!\"§$%&/()=?`´\\}][{³²@€|<>-_.:,;'#+~*µäÄöÖüÜß]\n\n");
	printf ("Example: 'clearhash -a -l 4 -h BhPXW2ifDuhV2' will generate all permutations from aaaa to zzzz including a-z, aa-zz and aaa-zzz\n");
	printf ("and tries to find the cleartext of the crypt hash BhPXW2ifDuhV2\n\n");
	printf ("You can also use md5 hashes. If your shell will interpret the $ sign as a special char, you must escape it like: \\$1\\$x9akHvmC\\$PP8/C6wTh7MtwWsRx79gy.\n\n");
	printf ("Send a SIGHUP signal to get the actual word\n");

	return 1;
}

int main(int argc, char *argv[]) {
	char *chars_a = "abcdefghijklmnopqrstuvwxyz";
	char *chars_b = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *chars_c = "0123456789";
	char *chars_d = "^°!\"§$%&/()=?`´\\}][{³²@€|<>-_.:,;'#+~*µäÄöÖüÜß";

	char *perm_chars = malloc(strlen(chars_a) + strlen(chars_b) + strlen(chars_c) + strlen(chars_d) + 1);

	int c;

	int flaga = 0;
	int flagb = 0;
	int flagc = 0;
	int flagd = 0;
	int flagl = 0;
	int flagh = 0;
	
	long int perm_chars_num;
	char *hash;

	while ((c = getopt (argc, argv, "abcdl:h:")) != -1) {
		switch (c) {
			case 'a':
				if (flaga == 0) {
					flaga = 1;
					strcat(perm_chars, chars_a);
				}
				break;
			case 'b':
				if (flagb == 0) {
					flagb = 1;
					strcat(perm_chars, chars_b);
				}
				break;
			case 'c':
				if (flagc == 0) {
					flagc = 1;
					strcat(perm_chars, chars_c);
				}
				break;
			case 'd':
				if (flagd == 0) {
					flagd = 1;
					strcat(perm_chars, chars_d);
				}
				break;
			case 'l':
				if (optopt != 'l') {
					flagl = 1;
					perm_chars_num = strtol(optarg, NULL, 10);
				}
				break;
			case 'h':
				if (optopt != 'h') {
					flagh = 1;
					hash = optarg;
				}
				break;
			default:
				usage();
		}
	}

	if ( (flaga == 1 || flagb == 1 || flagc == 1 || flagd == 1) && (flagl == 1 && flagh == 1) ) {
		/*FIXME:
		 * The first and the last permutation of each iteration (in the first for loop) 
		 * is the same because tmp_char mod len == 0, if tmp_char==0 and tmp_char==len
		 */

		unsigned long current = 0;
		unsigned long tmp_char = 0;

		size_t len = strlen(perm_chars);

		tmpword = malloc(len + 1);
		char *empty = "";

		char *result;

		signal(SIGHUP,sighandler);

		printf("Using chars: %s\n", perm_chars);
		printf("Trying to decrypt hash %s ...\n", hash);

		/* This loop is for generating the permutation blocks like a-z, aa-zz and so on */
		for (int i = 1; i <= perm_chars_num; i++) {

			tmp_char = 0;
			current = 0;

			/* Permutation of one block, like aaa-zzz, is ready if tmp_char == 1 */
			while(!tmp_char) {

				tmp_char = current;
				
				/* The permutation of each word (aa,...,cb,...,xy,...,zz) */
				for (int j = 0; j < i; j++) {

					/* tmp_char mod len results to 0,1,2,...,len  */
					strncat(tmpword, &perm_chars[tmp_char % len], 1);

					/* until tmp_char == len, the while loop will continue, because in this case tmp_char will be 1 */
					tmp_char /= len;
				}

				/* crypt the word with the salt of the hash */
				result = crypt(tmpword, hash);

				/* compare the result with the given hash */
				if (strcmp(result, hash) == 0) {
					printf("Result: %s\n", tmpword);
					return 0;
				}
				
				/* reset tmpword */
				strcpy(tmpword, empty);

				/* This will be the next tmp_char */
				current++;
			}
		}
		return 1;
	}
	else {
		usage();
	}
}

