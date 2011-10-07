DESCRIPTION

	clearhash - A program to find the cleartext of crypt and md5 hashes.

	This is proof of concept code and for testing purposes only!

OPTIONS

	Syntax: clearhash -abcd -l [0-9] -h <hash>

	-a: chars: [a-z]
	-b: chars: [A-Z]
	-c: chars: [0-9]
	-d: chars: [^°!"§$%&/()=?`´\}][{³²@€|<>-_.:,;'#+~*µäÄöÖüÜß]

EXAMPLE

	'clearhash -a -l 4 -h BhPXW2ifDuhV2' will generate all permutations from aaaa to zzzz including a-z, aa-zz and aaa-zzz and tries to find the cleartext of the crypt hash BhPXW2ifDuhV2

	You can also use md5 hashes. If your shell will interpret the $ sign as a special char, you must escape it like: \$1\$x9akHvmC\$PP8/C6wTh7MtwWsRx79gy.

	Send a SIGHUP signal to get the actual word
