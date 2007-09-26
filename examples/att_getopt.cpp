/* att_getopt.cpp: a public domain implementation of getopt()
 *
 * The following source code is an adaptation of the public domain
 * getopt() implementation presented at the 1985 UNIFORUM conference
 * in Dallas, Texas.  Slight changes have been made to improve style
 * and readability.  The result is released into the public domain
 * like that from which it was derived.
 */

#include <stdio.h>
#include <string.h>

int ag_optind = 1;
int ag_optopt;
const char* ag_optarg;

int
att_getopt(int argc, char* const argv[], const char* ag_opts)
{
	static int sp = 1;
	register int c;
	register const char *cp;

	if (sp == 1) {
		/* If all args are processed, finish */
		if (ag_optind >= argc) {
			return EOF;
		}
		if (argv[ag_optind][0] != '-' || argv[ag_optind][1] == '\0') {
			return EOF;
		}
	}
	else if (!strcmp(argv[ag_optind], "--")) {
		/* No more ag_options to be processed after this one */
		ag_optind++;
		return EOF;
	}

	ag_optopt = c = argv[ag_optind][sp];

	/* Check for invalid ag_option */
	if (c == ':' || (cp = strchr(ag_opts, c)) == NULL) {
		fprintf(stderr, "%s: illegal option -- %c\n", argv[0], c);
		if (argv[ag_optind][++sp] == '\0') {
			ag_optind++;
			sp = 1;
		}

		return '?';
	}

	/* Does this ag_option require an argument? */
	if (*++cp == ':') {
		/* If so, get argument; if none provided output error */
		if (argv[ag_optind][sp + 1] != '\0') {
			ag_optarg = &argv[ag_optind++][sp + 1];
		}
		else if (++ag_optind >= argc) {
			fprintf(stderr,
					"%s: option requires an argument -- %c\n", argv[0], c);
			sp = 1;
			return '?';
		}
		else {
			ag_optarg = argv[ag_optind++];
		}
		sp = 1;
	}
	else {
		if (argv[ag_optind][++sp] == '\0') {
			sp = 1;
			ag_optind++;
		}
		ag_optarg = NULL;
	}

	return c;
}
