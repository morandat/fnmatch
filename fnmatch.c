#include <fnmatch.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define SYNTAX  "fnmatch [-v] [-i input] [-o output] [-f patterns]* patterns*"
#define HELP    "[-v|-r]       Invert match. Selected lines are those not matching any of the specified patterns.\n" \
                "[-i input]    Read <input> file instead of stdin\n"            \
                "[-o input]    Write to <output> file instead of stdout\n"      \
                "[-f file]     Read patterns from <file>. May be used more than once\n"
#define LINES_INCREMENT 100

struct lines {
    int count;
    int capacity;
    char **data;
};

void exit_if(int cond, const char *prefix)
{
    if (cond) {
        perror(prefix);
        exit(EXIT_FAILURE);
    }
}

void append_line(struct lines *lines, char *line)
{
    if (lines->count >= lines->capacity)
        lines->data = realloc(lines->data,
                (lines->capacity += LINES_INCREMENT)*sizeof(lines->data[0]));
    lines->data[lines->count++] = line;
}

int read_file(const char *fname, struct lines *lines)
{
    FILE *fd = fopen(fname, "r");
    if (fd == NULL) return 0;

    char    *current = NULL;
    ssize_t last_chr;
    size_t  allocated;
    while ((last_chr = getline(&current, &allocated, fd) )> 0) {
        current[last_chr - 1] = 0;
        append_line(lines, current);
        current = NULL;
    }

    return fclose(fd) == 0;
}

int main(int argc, char *argv[])
{
    FILE *in = stdin, *out = stdout;
    int reversed = 0;
    struct lines patterns = {};

    int opt;
    while ((opt = getopt(argc, argv, "vri:o:f:?")) != -1) {
        switch (opt) {
        case 'v':
        case 'r':
            reversed = !reversed;
            break;
        case 'i':
            in = fopen(optarg, "r");
            exit_if(in == NULL, "input file");
            break;
        case 'o':
            out = fopen(optarg, "w");
            exit_if(in == NULL, "ouput file");
            break;
        case 'f':
            exit_if(read_file(optarg, &patterns) == 0, "pattern file");
            break;
        default:
            fprintf((opt == '?') ? stdout : stderr, SYNTAX "\n\n" HELP);
            return (opt == '?') ? EXIT_SUCCESS : EXIT_FAILURE;
        }
    }
    argc -= optind;
    argv += optind;

    for (int i = 0; i < argc ; ++i)
        append_line(&patterns, argv[i]);

    size_t buffer_len = 0;
    char *buffer = NULL;
    int last_chr;
    while ((last_chr = getline(&buffer, &buffer_len, in)) > 0) {
        buffer[last_chr - 1] = 0;
        int found = 0;
        for (int i = 0; i < patterns.count; ++i) {
            found |= fnmatch(patterns.data[i], buffer, 0) != FNM_NOMATCH;
            if (found) break;
        }
        if (found ^ reversed)
            fprintf(out, "%s\n", buffer);
    }

#ifdef CLEAR_MEMORY
    free(buffer);
    patterns.count -= argc;
    while (patterns.count--)
        free(patterns.data[patterns.count]);
    free(patterns.data);
#endif

    if (in != stdin) fclose(in);
    if (out != stdout) fclose(out);
    return EXIT_SUCCESS;
}
