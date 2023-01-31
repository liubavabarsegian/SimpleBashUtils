#include "grep.h"

int main(int argc, char **argv) { grep(argc, argv); }

void grep(int argc, char **argv) {
  if (argc > 1) {
    flags options = {0};
    char *pattern = NULL;

    for (int opt = fill_flags(argc, argv, &options, &pattern); opt < argc;
         opt++) {
      do_file(argv[optind], options, pattern);
      optind++;
    }
    if (pattern) {
      free(pattern);
    }
  }
}

int fill_flags(int argc, char **argv, flags *options, char **pattern) {
  char opt;
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != (-1)) {
    switch (opt) {
      case 'e':
        set_pattern_for_e(options, pattern, optarg);
        break;
      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'f':
        set_pattern_file_for_f(options, pattern, optarg);
        break;
      case 'o':
        options->o = 1;
        break;
    }
  }
  if ((options->e || options->f) == 0) {
    read_pattern(pattern, argv[optind]);
    optind++;
  }
  // printf("pattern: %s\n", *pattern);
  count_files(options, optind, argc, argv);
  return optind;
}

void count_files(flags *options, int optind, int argc, char **argv) {
  int count;
  count = 0;
  while (optind < argc) {
    if (argv[optind]) {
      count++;
    }
    optind++;
  }
  options->count_files = count;
}

void set_pattern_for_e(flags *options, char **pattern, char *optarg) {
  *pattern = malloc(strlen(optarg) + 1);
  if (*pattern) {
    strcpy(*pattern, optarg);
  }
  if (strcmp(*pattern, optarg) == 0) {
    options->e = 1;
  }
}

void set_pattern_file_for_f(flags *options, char **pattern, char *optarg) {
  FILE *file;
  file = fopen(optarg, "r");

  *pattern = (char *)malloc(sizeof(optarg) + 1);
  if (*pattern) {
    strcpy(*pattern, optarg);
  }
  if (strcmp(*pattern, optarg) == 0 && file) {
    options->f = 1;
  }
  if (file) {
    fclose(file);
  }
  if ((!file)) {
    fprintf(stderr, "grep: %s: No such file or directory\n", *pattern);
  }
}

void read_pattern(char **pattern, char *arg) {
  *pattern = malloc(strlen(arg) + 1);
  if (*pattern) {
    strcpy(*pattern, arg);
  }
}

void do_file(char *filename, flags options, char *pattern) {
  FILE *file;
  char *line = NULL;
  size_t len = 0;
  options.line_num = 1;
  int count_matching_lines = 0;

  file = fopen(filename, "r");
  if (!file && !options.s) {
    fprintf(stderr, "grep: %s: No such file or directory\n", filename);
  }
  if (file) {
    while (getline(&line, &len, file) != -1) {
      do_line(pattern, line, options, &count_matching_lines, filename);
      options.line_num++;
      
    }
    if (options.c) {
      if (!options.h) {
        if (options.count_files > 1) {
          printf("%s:", filename);
        }
      }
      if (options.l) {
        if (count_matching_lines > 0) {
          printf("1\n");
        }
        else {
          printf("0\n");
        }
      } else {
        printf("%d\n", count_matching_lines);
      }
    }
    if (options.l) {
      if (count_matching_lines >= 1) {
        printf("%s\n", filename);
      }
    }
    free(line);
    fclose(file);
  }
}

int match(char *string, char *pattern, flags options) {
  regex_t re;
  if (options.i) {
    if (regcomp(&re, pattern, REG_ICASE) != 0) {
      regfree(&re);
      return 0;
    }
  } else if (regcomp(&re, pattern, REG_EXTENDED ) != 0) {
    regfree(&re);
    return 0;
  }
  int status = regexec(&re, string, 0, NULL, 0);
  regfree(&re);
  if (status != 0) return 0;
  return 1;
}

void do_line(char *pattern, char *line, flags options,
             int *count_matching_lines, char *filename) {
  if (options.v) {
      if (options.f && matches_none_from_file(pattern, line, options)) {
        (*count_matching_lines)++;
        do_n_h_o(pattern, line, options, filename);
      }
      else if (!options.f && !match(line, pattern, options)) {
        (*count_matching_lines)++;
        do_n_h_o(pattern, line, options, filename);
      }
  } else {
    if (options.f && matches_any_from_file(pattern, line, options)) {
        (*count_matching_lines)++;
        do_n_h_o(pattern, line, options, filename);
    }
    else if (!options.f && match(line, pattern, options)) {
      (*count_matching_lines)++;
      do_n_h_o(pattern, line, options, filename);
    }
  }
}

int matches_none_from_file(char *pattern_file, char *line, flags options)
{
  FILE *file;
  file = fopen(pattern_file, "r");
  char *pattern_line = NULL;
  size_t len = 0;
  if (file) {
    while (getline(&pattern_line, &len, file) != -1) {
      if (match(line, pattern_line, options)) {
        fclose(file);
        free(pattern_line);
        return 0;
      }
    }
    free(pattern_line);
    fclose(file);
  }
  return 1;
}

int matches_any_from_file(char *pattern_file, char *line, flags options)
{
  FILE *pfile;
  pfile = fopen(pattern_file, "r");
  char *pattern_line = NULL;
  size_t len = 0; 
  
  if (pfile) {
    // printf("AAA");
    // почему-то не заходит сюда  ./s21_grep -ssi -f pattern_file.txt test3.txt
    while (getline(&pattern_line, &len, pfile) != -1) {
      // printf("%s\n", line);
      if (match(line, pattern_line, options)) {
        
        fclose(pfile);
        free(pattern_line);
        return 1;
      }
    }
    free(pattern_line);
    fclose(pfile);
  }
  return 0;
}

void do_n_h_o(char *pattern, char *line, flags options, char *filename) {
  regex_t regex;
  (regcomp(&regex, pattern,REG_EXTENDED));
  if (!options.l && !options.c) {
    
    if (!options.h) {
      if (options.count_files > 1) {
        printf("%s:", filename);
      }
    }

    if (options.n) {
      printf("%d:", options.line_num);
    }
    if (options.o) {
      do_o(regex, line, options);
    } else {
      printf("%s", line);
    }
  }
  regfree(&regex);
}

void do_o(regex_t regex, char *line, flags options) {
  while (regexec(&regex, line, 0, NULL, 0) == options.v) {
    char *output = (char *)malloc(strlen(line) + 1);
    int end = strlen(line);
    int start = 0;
    int i = strlen(output);

    strcpy(output, line);
    while (regexec(&regex, output, 0, NULL, 0) == options.v) {
      end--;
      output[strlen(output) - 1] = 0;
    }
    output[strlen(output)] = line[strlen(output)];
    
    while (regexec(&regex, output, 0, NULL, 0) == options.v && strlen(output) > 0) {
      int j = 0;
      start++;
      while (output[j] != 0) {
        output[j] = output[j + 1];
        j++;
      }
    }
    start--;
    while (i != 0) {
      output[i] = output[i - 1];
      i--;
    }
    output[0] = line[start];
    printf("%s\n", output);
    free(output);
    i = start + 1;
    while (line[i] != 0) {
      line[i - start - 1] = line[i];
      i++;
    }
    line[i - start - 1] = 0;
  }
}