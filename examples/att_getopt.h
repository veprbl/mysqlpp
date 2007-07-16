#if !defined(ATT_GETOPT_H)
#define ATT_GETOPT_H

extern int att_getopt(int argc, char* const argv[], const char* opts);

extern int ag_optind;
extern int ag_optopt;
extern const char* ag_optarg;

#endif // !defined(ATT_GETOPT_H)

