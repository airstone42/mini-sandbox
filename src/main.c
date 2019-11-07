#include "cli.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt = 0;
    opterr = 0;

    struct cli_arg_t cli_arg;
    bzero(&cli_arg, sizeof(cli_arg));

    if (argc == 1) {
        cli_help();
        return 0;
    }

    while ((opt = getopt(argc, argv, "hcrpl:f:e:d:i:o:x:")) != -1) {
        switch (opt) {
            case 'h':
                cli_help();
                return 0;
            case 'c':
                if (!cli_set_option(&cli_arg, CLI_COMP))
                    fprintf(stderr, "%s", CLI_BAD_OPT);
                break;
            case 'r':
                if (!cli_set_option(&cli_arg, CLI_EXEC))
                    fprintf(stderr, "%s", CLI_BAD_OPT);
                break;
            case 'p':
                if (!cli_set_option(&cli_arg, CLI_PROC))
                    fprintf(stderr, "%s", CLI_BAD_OPT);
                break;
            case 'l':
                cli_set_src_type(&cli_arg, optarg);
                break;
            case 'f':
                cli_set_src_file(&cli_arg, optarg);
                break;
            case 'e':
                cli_set_exe_file(&cli_arg, optarg);
                break;
            case 'd':
                if (!cli_set_java_path(&cli_arg, optarg))
                    fprintf(stderr, "%s", CLI_BAD_OPT);
                break;
            case 'i':
                cli_set_io(&cli_arg, optarg, FILE_IN);
                break;
            case 'o':
                cli_set_io(&cli_arg, optarg, FILE_OUT);
                break;
            case 'x':
                cli_set_io(&cli_arg, optarg, FILE_ERR);
                break;
            default:
                fprintf(stderr, CLI_BAD_ARG, opt);
                return 1;
        }
    }
    cli_default_option(&cli_arg);

    struct tlimit_t limit = tlimit_default();
    struct tusage_t usage;
    bzero(&usage, sizeof(usage));

    switch (cli_arg.option) {
        case CLI_COMP:
            compile(&limit, &usage, &cli_arg.comp);
            break;
        case CLI_EXEC:
            execute(&limit, &usage, &cli_arg.exec);
            break;
        case CLI_PROC:
            compile(&limit, &usage, &cli_arg.comp);
            execute(&limit, &usage, &cli_arg.exec);
            break;
        default:
            fprintf(stderr, CLI_BAD_ARG, opt);
            return 1;
    }

    delete_cli_arg(&cli_arg);
    return 0;
}
