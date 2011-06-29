/*
  File autogenerated by gengetopt version 2.22.4
  generated with the following command:
  gengetopt --output-dir=./src/3rd/ --file-name=sorter_options 

  The developers of gengetopt consider the fixed text that goes in all
  gengetopt output files to be in the public domain:
  we make no copyright claims on it.
*/

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FIX_UNUSED
#define FIX_UNUSED(X) (void) (X) /* avoid warnings for unused params */
#endif

#include <getopt.h>

#include "sorter_options.h"

const char *gengetopt_args_info_purpose = "This program lets you sort text files.";

const char *gengetopt_args_info_usage = "Usage: Sorter [OPTIONS]...";

const char *gengetopt_args_info_description = "";

const char *gengetopt_args_info_help[] = {
  "  -h, --help                    Print help and exit",
  "  -V, --version                 Print version and exit",
  "  -i, --input=<folder>          Folder with the files to sort",
  "  -o, --output=<folder>         Folder to put the sorted files",
  "  -a, --serial-algorithm=<algorithm>\n                                Algorithms to use in the sort process  \n                                  (possible values=\"bubble\", \"merge\", \n                                  \"quick\", \"shell\")",
  "\n Mode: Daemon",
  "  -l, --log=<filename>          Filename to log the messages",
  "  -d, --daemon                  Use program as a daemon  (default=off)",
  "\n Mode: UDP time",
  "  -s, --time-server-addr=<ipaddress>\n                                IP address of the UDP time server",
  "  -p, --time-server-port=<portnumber>\n                                The port of the UDP time server",
  "\n Mode: UDP report",
  "      --stats-server=<ipaddress>\n                                IP address of the UDP server to publish the \n                                  results",
  "      --stats-port=<portnumber> The port of the UDP server to publish the \n                                  results",
    0
};

typedef enum {ARG_NO
  , ARG_FLAG
  , ARG_STRING
  , ARG_INT
  , ARG_ENUM
} cmdline_parser_arg_type;

static
void clear_given (struct gengetopt_args_info *args_info);
static
void clear_args (struct gengetopt_args_info *args_info);

static int
cmdline_parser_internal (int argc, char **argv, struct gengetopt_args_info *args_info,
                        struct cmdline_parser_params *params, const char *additional_error);

static int
cmdline_parser_required2 (struct gengetopt_args_info *args_info, const char *prog_name, const char *additional_error);

const char *cmdline_parser_serial_algorithm_values[] = {"bubble", "merge", "quick", "shell", 0}; /*< Possible values for serial-algorithm. */

static char *
gengetopt_strdup (const char *s);

static
void clear_given (struct gengetopt_args_info *args_info)
{
  args_info->help_given = 0 ;
  args_info->version_given = 0 ;
  args_info->input_given = 0 ;
  args_info->output_given = 0 ;
  args_info->serial_algorithm_given = 0 ;
  args_info->log_given = 0 ;
  args_info->daemon_given = 0 ;
  args_info->time_server_addr_given = 0 ;
  args_info->time_server_port_given = 0 ;
  args_info->stats_server_given = 0 ;
  args_info->stats_port_given = 0 ;
  args_info->Daemon_mode_counter = 0 ;
  args_info->UDP_report_mode_counter = 0 ;
  args_info->UDP_time_mode_counter = 0 ;
}

static
void clear_args (struct gengetopt_args_info *args_info)
{
  FIX_UNUSED (args_info);
  args_info->input_arg = NULL;
  args_info->input_orig = NULL;
  args_info->output_arg = NULL;
  args_info->output_orig = NULL;
  args_info->serial_algorithm_arg = NULL;
  args_info->serial_algorithm_orig = NULL;
  args_info->log_arg = NULL;
  args_info->log_orig = NULL;
  args_info->daemon_flag = 0;
  args_info->time_server_addr_arg = NULL;
  args_info->time_server_addr_orig = NULL;
  args_info->time_server_port_orig = NULL;
  args_info->stats_server_arg = NULL;
  args_info->stats_server_orig = NULL;
  args_info->stats_port_orig = NULL;
  
}

static
void init_args_info(struct gengetopt_args_info *args_info)
{


  args_info->help_help = gengetopt_args_info_help[0] ;
  args_info->version_help = gengetopt_args_info_help[1] ;
  args_info->input_help = gengetopt_args_info_help[2] ;
  args_info->output_help = gengetopt_args_info_help[3] ;
  args_info->serial_algorithm_help = gengetopt_args_info_help[4] ;
  args_info->serial_algorithm_min = 1;
  args_info->serial_algorithm_max = 4;
  args_info->log_help = gengetopt_args_info_help[6] ;
  args_info->daemon_help = gengetopt_args_info_help[7] ;
  args_info->time_server_addr_help = gengetopt_args_info_help[9] ;
  args_info->time_server_port_help = gengetopt_args_info_help[10] ;
  args_info->stats_server_help = gengetopt_args_info_help[12] ;
  args_info->stats_port_help = gengetopt_args_info_help[13] ;
  
}

void
cmdline_parser_print_version (void)
{
  printf ("%s %s\n",
     (strlen(CMDLINE_PARSER_PACKAGE_NAME) ? CMDLINE_PARSER_PACKAGE_NAME : CMDLINE_PARSER_PACKAGE),
     CMDLINE_PARSER_VERSION);
}

static void print_help_common(void) {
  cmdline_parser_print_version ();

  if (strlen(gengetopt_args_info_purpose) > 0)
    printf("\n%s\n", gengetopt_args_info_purpose);

  if (strlen(gengetopt_args_info_usage) > 0)
    printf("\n%s\n", gengetopt_args_info_usage);

  printf("\n");

  if (strlen(gengetopt_args_info_description) > 0)
    printf("%s\n\n", gengetopt_args_info_description);
}

void
cmdline_parser_print_help (void)
{
  int i = 0;
  print_help_common();
  while (gengetopt_args_info_help[i])
    printf("%s\n", gengetopt_args_info_help[i++]);
}

void
cmdline_parser_init (struct gengetopt_args_info *args_info)
{
  clear_given (args_info);
  clear_args (args_info);
  init_args_info (args_info);
}

void
cmdline_parser_params_init(struct cmdline_parser_params *params)
{
  if (params)
    { 
      params->override = 0;
      params->initialize = 1;
      params->check_required = 1;
      params->check_ambiguity = 0;
      params->print_errors = 1;
    }
}

struct cmdline_parser_params *
cmdline_parser_params_create(void)
{
  struct cmdline_parser_params *params = 
    (struct cmdline_parser_params *)malloc(sizeof(struct cmdline_parser_params));
  cmdline_parser_params_init(params);  
  return params;
}

static void
free_string_field (char **s)
{
  if (*s)
    {
      free (*s);
      *s = 0;
    }
}

/** @brief generic value variable */
union generic_value {
    int int_arg;
    char *string_arg;
    const char *default_string_arg;
};

/** @brief holds temporary values for multiple options */
struct generic_list
{
  union generic_value arg;
  char *orig;
  struct generic_list *next;
};

/**
 * @brief add a node at the head of the list 
 */
static void add_node(struct generic_list **list) {
  struct generic_list *new_node = (struct generic_list *) malloc (sizeof (struct generic_list));
  new_node->next = *list;
  *list = new_node;
  new_node->arg.string_arg = 0;
  new_node->orig = 0;
}

/**
 * The passed arg parameter is NOT set to 0 from this function
 */
static void
free_multiple_field(unsigned int len, void *arg, char ***orig)
{
  unsigned int i;
  if (arg) {
    for (i = 0; i < len; ++i)
      {
        free_string_field(&((*orig)[i]));
      }

    free (arg);
    free (*orig);
    *orig = 0;
  }
}


static void
cmdline_parser_release (struct gengetopt_args_info *args_info)
{

  free_string_field (&(args_info->input_arg));
  free_string_field (&(args_info->input_orig));
  free_string_field (&(args_info->output_arg));
  free_string_field (&(args_info->output_orig));
  free_multiple_field (args_info->serial_algorithm_given, (void *)(args_info->serial_algorithm_arg), &(args_info->serial_algorithm_orig));
  args_info->serial_algorithm_arg = 0;
  free_string_field (&(args_info->log_arg));
  free_string_field (&(args_info->log_orig));
  free_string_field (&(args_info->time_server_addr_arg));
  free_string_field (&(args_info->time_server_addr_orig));
  free_string_field (&(args_info->time_server_port_orig));
  free_string_field (&(args_info->stats_server_arg));
  free_string_field (&(args_info->stats_server_orig));
  free_string_field (&(args_info->stats_port_orig));
  
  

  clear_given (args_info);
}

/**
 * @param val the value to check
 * @param values the possible values
 * @return the index of the matched value:
 * -1 if no value matched,
 * -2 if more than one value has matched
 */
static int
check_possible_values(const char *val, const char *values[])
{
  int i, found, last;
  size_t len;

  if (!val)   /* otherwise strlen() crashes below */
    return -1; /* -1 means no argument for the option */

  found = last = 0;

  for (i = 0, len = strlen(val); values[i]; ++i)
    {
      if (strncmp(val, values[i], len) == 0)
        {
          ++found;
          last = i;
          if (strlen(values[i]) == len)
            return i; /* exact macth no need to check more */
        }
    }

  if (found == 1) /* one match: OK */
    return last;

  return (found ? -2 : -1); /* return many values or none matched */
}


static void
write_into_file(FILE *outfile, const char *opt, const char *arg, const char *values[])
{
  int found = -1;
  if (arg) {
    if (values) {
      found = check_possible_values(arg, values);      
    }
    if (found >= 0)
      fprintf(outfile, "%s=\"%s\" # %s\n", opt, arg, values[found]);
    else
      fprintf(outfile, "%s=\"%s\"\n", opt, arg);
  } else {
    fprintf(outfile, "%s\n", opt);
  }
}

static void
write_multiple_into_file(FILE *outfile, int len, const char *opt, char **arg, const char *values[])
{
  int i;
  
  for (i = 0; i < len; ++i)
    write_into_file(outfile, opt, (arg ? arg[i] : 0), values);
}

int
cmdline_parser_dump(FILE *outfile, struct gengetopt_args_info *args_info)
{
  int i = 0;

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot dump options to stream\n", CMDLINE_PARSER_PACKAGE);
      return EXIT_FAILURE;
    }

  if (args_info->help_given)
    write_into_file(outfile, "help", 0, 0 );
  if (args_info->version_given)
    write_into_file(outfile, "version", 0, 0 );
  if (args_info->input_given)
    write_into_file(outfile, "input", args_info->input_orig, 0);
  if (args_info->output_given)
    write_into_file(outfile, "output", args_info->output_orig, 0);
  write_multiple_into_file(outfile, args_info->serial_algorithm_given, "serial-algorithm", args_info->serial_algorithm_orig, cmdline_parser_serial_algorithm_values);
  if (args_info->log_given)
    write_into_file(outfile, "log", args_info->log_orig, 0);
  if (args_info->daemon_given)
    write_into_file(outfile, "daemon", 0, 0 );
  if (args_info->time_server_addr_given)
    write_into_file(outfile, "time-server-addr", args_info->time_server_addr_orig, 0);
  if (args_info->time_server_port_given)
    write_into_file(outfile, "time-server-port", args_info->time_server_port_orig, 0);
  if (args_info->stats_server_given)
    write_into_file(outfile, "stats-server", args_info->stats_server_orig, 0);
  if (args_info->stats_port_given)
    write_into_file(outfile, "stats-port", args_info->stats_port_orig, 0);
  

  i = EXIT_SUCCESS;
  return i;
}

int
cmdline_parser_file_save(const char *filename, struct gengetopt_args_info *args_info)
{
  FILE *outfile;
  int i = 0;

  outfile = fopen(filename, "w");

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot open file for writing: %s\n", CMDLINE_PARSER_PACKAGE, filename);
      return EXIT_FAILURE;
    }

  i = cmdline_parser_dump(outfile, args_info);
  fclose (outfile);

  return i;
}

void
cmdline_parser_free (struct gengetopt_args_info *args_info)
{
  cmdline_parser_release (args_info);
}

/** @brief replacement of strdup, which is not standard */
char *
gengetopt_strdup (const char *s)
{
  char *result = 0;
  if (!s)
    return result;

  result = (char*)malloc(strlen(s) + 1);
  if (result == (char*)0)
    return (char*)0;
  strcpy(result, s);
  return result;
}

static char *
get_multiple_arg_token(const char *arg)
{
  const char *tok;
  char *ret;
  size_t len, num_of_escape, i, j;

  if (!arg)
    return 0;

  tok = strchr (arg, ',');
  num_of_escape = 0;

  /* make sure it is not escaped */
  while (tok)
    {
      if (*(tok-1) == '\\')
        {
          /* find the next one */
          tok = strchr (tok+1, ',');
          ++num_of_escape;
        }
      else
        break;
    }

  if (tok)
    len = (size_t)(tok - arg + 1);
  else
    len = strlen (arg) + 1;

  len -= num_of_escape;

  ret = (char *) malloc (len);

  i = 0;
  j = 0;
  while (arg[i] && (j < len-1))
    {
      if (arg[i] == '\\' && 
	  arg[ i + 1 ] && 
	  arg[ i + 1 ] == ',')
        ++i;

      ret[j++] = arg[i++];
    }

  ret[len-1] = '\0';

  return ret;
}

static const char *
get_multiple_arg_token_next(const char *arg)
{
  const char *tok;

  if (!arg)
    return 0;

  tok = strchr (arg, ',');

  /* make sure it is not escaped */
  while (tok)
    {
      if (*(tok-1) == '\\')
        {
          /* find the next one */
          tok = strchr (tok+1, ',');
        }
      else
        break;
    }

  if (! tok || strlen(tok) == 1)
    return 0;

  return tok+1;
}

static int
check_multiple_option_occurrences(const char *prog_name, unsigned int option_given, unsigned int min, unsigned int max, const char *option_desc);

int
check_multiple_option_occurrences(const char *prog_name, unsigned int option_given, unsigned int min, unsigned int max, const char *option_desc)
{
  int error = 0;

  if (option_given && (min > 0 || max > 0))
    {
      if (min > 0 && max > 0)
        {
          if (min == max)
            {
              /* specific occurrences */
              if (option_given != (unsigned int) min)
                {
                  fprintf (stderr, "%s: %s option occurrences must be %d\n",
                    prog_name, option_desc, min);
                  error = 1;
                }
            }
          else if (option_given < (unsigned int) min
                || option_given > (unsigned int) max)
            {
              /* range occurrences */
              fprintf (stderr, "%s: %s option occurrences must be between %d and %d\n",
                prog_name, option_desc, min, max);
              error = 1;
            }
        }
      else if (min > 0)
        {
          /* at least check */
          if (option_given < min)
            {
              fprintf (stderr, "%s: %s option occurrences must be at least %d\n",
                prog_name, option_desc, min);
              error = 1;
            }
        }
      else if (max > 0)
        {
          /* at most check */
          if (option_given > max)
            {
              fprintf (stderr, "%s: %s option occurrences must be at most %d\n",
                prog_name, option_desc, max);
              error = 1;
            }
        }
    }
    
  return error;
}
int
cmdline_parser (int argc, char **argv, struct gengetopt_args_info *args_info)
{
  return cmdline_parser2 (argc, argv, args_info, 0, 1, 1);
}

int
cmdline_parser_ext (int argc, char **argv, struct gengetopt_args_info *args_info,
                   struct cmdline_parser_params *params)
{
  int result;
  result = cmdline_parser_internal (argc, argv, args_info, params, 0);

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser2 (int argc, char **argv, struct gengetopt_args_info *args_info, int override, int initialize, int check_required)
{
  int result;
  struct cmdline_parser_params params;
  
  params.override = override;
  params.initialize = initialize;
  params.check_required = check_required;
  params.check_ambiguity = 0;
  params.print_errors = 1;

  result = cmdline_parser_internal (argc, argv, args_info, &params, 0);

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser_required (struct gengetopt_args_info *args_info, const char *prog_name)
{
  int result = EXIT_SUCCESS;

  if (cmdline_parser_required2(args_info, prog_name, 0) > 0)
    result = EXIT_FAILURE;

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser_required2 (struct gengetopt_args_info *args_info, const char *prog_name, const char *additional_error)
{
  int error = 0;
  FIX_UNUSED (additional_error);

  /* checks for required options */
  if (! args_info->input_given)
    {
      fprintf (stderr, "%s: '--input' ('-i') option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error = 1;
    }
  
  if (! args_info->output_given)
    {
      fprintf (stderr, "%s: '--output' ('-o') option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error = 1;
    }
  
  if (! args_info->serial_algorithm_given)
    {
      fprintf (stderr, "%s: '--serial-algorithm' ('-a') option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error = 1;
    }
  
  if (check_multiple_option_occurrences(prog_name, args_info->serial_algorithm_given, args_info->serial_algorithm_min, args_info->serial_algorithm_max, "'--serial-algorithm' ('-a')"))
     error = 1;
  
  if (args_info->Daemon_mode_counter && ! args_info->log_given)
    {
      fprintf (stderr, "%s: '--log' ('-l') option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error = 1;
    }
  
  if (args_info->UDP_time_mode_counter && ! args_info->time_server_addr_given)
    {
      fprintf (stderr, "%s: '--time-server-addr' ('-s') option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error = 1;
    }
  
  if (args_info->UDP_time_mode_counter && ! args_info->time_server_port_given)
    {
      fprintf (stderr, "%s: '--time-server-port' ('-p') option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error = 1;
    }
  
  if (args_info->UDP_report_mode_counter && ! args_info->stats_server_given)
    {
      fprintf (stderr, "%s: '--stats-server' option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error = 1;
    }
  
  if (args_info->UDP_report_mode_counter && ! args_info->stats_port_given)
    {
      fprintf (stderr, "%s: '--stats-port' option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error = 1;
    }
  
  
  /* checks for dependences among options */

  return error;
}


static char *package_name = 0;

/**
 * @brief updates an option
 * @param field the generic pointer to the field to update
 * @param orig_field the pointer to the orig field
 * @param field_given the pointer to the number of occurrence of this option
 * @param prev_given the pointer to the number of occurrence already seen
 * @param value the argument for this option (if null no arg was specified)
 * @param possible_values the possible values for this option (if specified)
 * @param default_value the default value (in case the option only accepts fixed values)
 * @param arg_type the type of this option
 * @param check_ambiguity @see cmdline_parser_params.check_ambiguity
 * @param override @see cmdline_parser_params.override
 * @param no_free whether to free a possible previous value
 * @param multiple_option whether this is a multiple option
 * @param long_opt the corresponding long option
 * @param short_opt the corresponding short option (or '-' if none)
 * @param additional_error possible further error specification
 */
static
int update_arg(void *field, char **orig_field,
               unsigned int *field_given, unsigned int *prev_given, 
               char *value, const char *possible_values[],
               const char *default_value,
               cmdline_parser_arg_type arg_type,
               int check_ambiguity, int override,
               int no_free, int multiple_option,
               const char *long_opt, char short_opt,
               const char *additional_error)
{
  char *stop_char = 0;
  const char *val = value;
  int found;
  char **string_field;
  FIX_UNUSED (field);

  stop_char = 0;
  found = 0;

  if (!multiple_option && prev_given && (*prev_given || (check_ambiguity && *field_given)))
    {
      if (short_opt != '-')
        fprintf (stderr, "%s: `--%s' (`-%c') option given more than once%s\n", 
               package_name, long_opt, short_opt,
               (additional_error ? additional_error : ""));
      else
        fprintf (stderr, "%s: `--%s' option given more than once%s\n", 
               package_name, long_opt,
               (additional_error ? additional_error : ""));
      return 1; /* failure */
    }

  if (possible_values && (found = check_possible_values((value ? value : default_value), possible_values)) < 0)
    {
      if (short_opt != '-')
        fprintf (stderr, "%s: %s argument, \"%s\", for option `--%s' (`-%c')%s\n", 
          package_name, (found == -2) ? "ambiguous" : "invalid", value, long_opt, short_opt,
          (additional_error ? additional_error : ""));
      else
        fprintf (stderr, "%s: %s argument, \"%s\", for option `--%s'%s\n", 
          package_name, (found == -2) ? "ambiguous" : "invalid", value, long_opt,
          (additional_error ? additional_error : ""));
      return 1; /* failure */
    }
    
  if (field_given && *field_given && ! override)
    return 0;
  if (prev_given)
    (*prev_given)++;
  if (field_given)
    (*field_given)++;
  if (possible_values)
    val = possible_values[found];

  switch(arg_type) {
  case ARG_FLAG:
    *((int *)field) = !*((int *)field);
    break;
  case ARG_INT:
    if (val) *((int *)field) = strtol (val, &stop_char, 0);
    break;
  case ARG_ENUM:
    if (val) *((int *)field) = found;
    break;
  case ARG_STRING:
    if (val) {
      string_field = (char **)field;
      if (!no_free && *string_field)
        free (*string_field); /* free previous string */
      *string_field = gengetopt_strdup (val);
    }
    break;
  default:
    break;
  };

  /* check numeric conversion */
  switch(arg_type) {
  case ARG_INT:
    if (val && !(stop_char && *stop_char == '\0')) {
      fprintf(stderr, "%s: invalid numeric value: %s\n", package_name, val);
      return 1; /* failure */
    }
    break;
  default:
    ;
  };

  /* store the original value */
  switch(arg_type) {
  case ARG_NO:
  case ARG_FLAG:
    break;
  default:
    if (value && orig_field) {
      if (no_free) {
        *orig_field = value;
      } else {
        if (*orig_field)
          free (*orig_field); /* free previous string */
        *orig_field = gengetopt_strdup (value);
      }
    }
  };

  return 0; /* OK */
}

/**
 * @brief store information about a multiple option in a temporary list
 * @param list where to (temporarily) store multiple options
 */
static
int update_multiple_arg_temp(struct generic_list **list,
               unsigned int *prev_given, const char *val,
               const char *possible_values[], const char *default_value,
               cmdline_parser_arg_type arg_type,
               const char *long_opt, char short_opt,
               const char *additional_error)
{
  /* store single arguments */
  char *multi_token;
  const char *multi_next;

  if (arg_type == ARG_NO) {
    (*prev_given)++;
    return 0; /* OK */
  }

  multi_token = get_multiple_arg_token(val);
  multi_next = get_multiple_arg_token_next (val);

  while (1)
    {
      add_node (list);
      if (update_arg((void *)&((*list)->arg), &((*list)->orig), 0,
          prev_given, multi_token, possible_values, default_value, 
          arg_type, 0, 1, 1, 1, long_opt, short_opt, additional_error)) {
        if (multi_token) free(multi_token);
        return 1; /* failure */
      }

      if (multi_next)
        {
          multi_token = get_multiple_arg_token(multi_next);
          multi_next = get_multiple_arg_token_next (multi_next);
        }
      else
        break;
    }

  return 0; /* OK */
}

/**
 * @brief free the passed list (including possible string argument)
 */
static
void free_list(struct generic_list *list, short string_arg)
{
  if (list) {
    struct generic_list *tmp;
    while (list)
      {
        tmp = list;
        if (string_arg && list->arg.string_arg)
          free (list->arg.string_arg);
        if (list->orig)
          free (list->orig);
        list = list->next;
        free (tmp);
      }
  }
}

/**
 * @brief updates a multiple option starting from the passed list
 */
static
void update_multiple_arg(void *field, char ***orig_field,
               unsigned int field_given, unsigned int prev_given, union generic_value *default_value,
               cmdline_parser_arg_type arg_type,
               struct generic_list *list)
{
  int i;
  struct generic_list *tmp;

  if (prev_given && list) {
    *orig_field = (char **) realloc (*orig_field, (field_given + prev_given) * sizeof (char *));

    switch(arg_type) {
    case ARG_INT:
    case ARG_ENUM:
      *((int **)field) = (int *)realloc (*((int **)field), (field_given + prev_given) * sizeof (int)); break;
    case ARG_STRING:
      *((char ***)field) = (char **)realloc (*((char ***)field), (field_given + prev_given) * sizeof (char *)); break;
    default:
      break;
    };
    
    for (i = (prev_given - 1); i >= 0; --i)
      {
        tmp = list;
        
        switch(arg_type) {
        case ARG_INT:
          (*((int **)field))[i + field_given] = tmp->arg.int_arg; break;
        case ARG_ENUM:
          (*((int **)field))[i + field_given] = tmp->arg.int_arg; break;
        case ARG_STRING:
          (*((char ***)field))[i + field_given] = tmp->arg.string_arg; break;
        default:
          break;
        }        
        (*orig_field) [i + field_given] = list->orig;
        list = list->next;
        free (tmp);
      }
  } else { /* set the default value */
    if (default_value && ! field_given) {
      switch(arg_type) {
      case ARG_INT:
      case ARG_ENUM:
        if (! *((int **)field)) {
          *((int **)field) = (int *)malloc (sizeof (int));
          (*((int **)field))[0] = default_value->int_arg; 
        }
        break;
      case ARG_STRING:
        if (! *((char ***)field)) {
          *((char ***)field) = (char **)malloc (sizeof (char *));
          (*((char ***)field))[0] = gengetopt_strdup(default_value->string_arg);
        }
        break;
      default: break;
      }
      if (!(*orig_field)) {
        *orig_field = (char **) malloc (sizeof (char *));
        (*orig_field)[0] = 0;
      }
    }
  }
}

static int check_modes(
  int given1[], const char *options1[],
                       int given2[], const char *options2[])
{
  int i = 0, j = 0, errors = 0;
  
  while (given1[i] >= 0) {
    if (given1[i]) {
      while (given2[j] >= 0) {
        if (given2[j]) {
          ++errors;
          fprintf(stderr, "%s: option %s conflicts with option %s\n",
                  package_name, options1[i], options2[j]);
        }
        ++j;
      }
    }
    ++i;
  }
  
  return errors;
}

int
cmdline_parser_internal (
  int argc, char **argv, struct gengetopt_args_info *args_info,
                        struct cmdline_parser_params *params, const char *additional_error)
{
  int c;	/* Character of the parsed option.  */

  struct generic_list * serial_algorithm_list = NULL;
  int error = 0;
  struct gengetopt_args_info local_args_info;
  
  int override;
  int initialize;
  int check_required;
  int check_ambiguity;
  
  package_name = argv[0];
  
  override = params->override;
  initialize = params->initialize;
  check_required = params->check_required;
  check_ambiguity = params->check_ambiguity;

  if (initialize)
    cmdline_parser_init (args_info);

  cmdline_parser_init (&local_args_info);

  optarg = 0;
  optind = 0;
  opterr = params->print_errors;
  optopt = '?';

  while (1)
    {
      int option_index = 0;

      static struct option long_options[] = {
        { "help",	0, NULL, 'h' },
        { "version",	0, NULL, 'V' },
        { "input",	1, NULL, 'i' },
        { "output",	1, NULL, 'o' },
        { "serial-algorithm",	1, NULL, 'a' },
        { "log",	1, NULL, 'l' },
        { "daemon",	0, NULL, 'd' },
        { "time-server-addr",	1, NULL, 's' },
        { "time-server-port",	1, NULL, 'p' },
        { "stats-server",	1, NULL, 0 },
        { "stats-port",	1, NULL, 0 },
        { 0,  0, 0, 0 }
      };

      c = getopt_long (argc, argv, "hVi:o:a:l:ds:p:", long_options, &option_index);

      if (c == -1) break;	/* Exit from `while (1)' loop.  */

      switch (c)
        {
        case 'h':	/* Print help and exit.  */
          cmdline_parser_print_help ();
          cmdline_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'V':	/* Print version and exit.  */
          cmdline_parser_print_version ();
          cmdline_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'i':	/* Folder with the files to sort.  */
        
        
          if (update_arg( (void *)&(args_info->input_arg), 
               &(args_info->input_orig), &(args_info->input_given),
              &(local_args_info.input_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "input", 'i',
              additional_error))
            goto failure;
        
          break;
        case 'o':	/* Folder to put the sorted files.  */
        
        
          if (update_arg( (void *)&(args_info->output_arg), 
               &(args_info->output_orig), &(args_info->output_given),
              &(local_args_info.output_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "output", 'o',
              additional_error))
            goto failure;
        
          break;
        case 'a':	/* Algorithms to use in the sort process.  */
        
          if (update_multiple_arg_temp(&serial_algorithm_list, 
              &(local_args_info.serial_algorithm_given), optarg, cmdline_parser_serial_algorithm_values, 0, ARG_ENUM,
              "serial-algorithm", 'a',
              additional_error))
            goto failure;
        
          break;
        case 'l':	/* Filename to log the messages.  */
          args_info->Daemon_mode_counter += 1;
        
        
          if (update_arg( (void *)&(args_info->log_arg), 
               &(args_info->log_orig), &(args_info->log_given),
              &(local_args_info.log_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "log", 'l',
              additional_error))
            goto failure;
        
          break;
        case 'd':	/* Use program as a daemon.  */
          args_info->Daemon_mode_counter += 1;
        
        
          if (update_arg((void *)&(args_info->daemon_flag), 0, &(args_info->daemon_given),
              &(local_args_info.daemon_given), optarg, 0, 0, ARG_FLAG,
              check_ambiguity, override, 1, 0, "daemon", 'd',
              additional_error))
            goto failure;
        
          break;
        case 's':	/* IP address of the UDP time server.  */
          args_info->UDP_time_mode_counter += 1;
        
        
          if (update_arg( (void *)&(args_info->time_server_addr_arg), 
               &(args_info->time_server_addr_orig), &(args_info->time_server_addr_given),
              &(local_args_info.time_server_addr_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "time-server-addr", 's',
              additional_error))
            goto failure;
        
          break;
        case 'p':	/* The port of the UDP time server.  */
          args_info->UDP_time_mode_counter += 1;
        
        
          if (update_arg( (void *)&(args_info->time_server_port_arg), 
               &(args_info->time_server_port_orig), &(args_info->time_server_port_given),
              &(local_args_info.time_server_port_given), optarg, 0, 0, ARG_INT,
              check_ambiguity, override, 0, 0,
              "time-server-port", 'p',
              additional_error))
            goto failure;
        
          break;

        case 0:	/* Long option with no short option */
          /* IP address of the UDP server to publish the results.  */
          if (strcmp (long_options[option_index].name, "stats-server") == 0)
          {
            args_info->UDP_report_mode_counter += 1;
          
          
            if (update_arg( (void *)&(args_info->stats_server_arg), 
                 &(args_info->stats_server_orig), &(args_info->stats_server_given),
                &(local_args_info.stats_server_given), optarg, 0, 0, ARG_STRING,
                check_ambiguity, override, 0, 0,
                "stats-server", '-',
                additional_error))
              goto failure;
          
          }
          /* The port of the UDP server to publish the results.  */
          else if (strcmp (long_options[option_index].name, "stats-port") == 0)
          {
            args_info->UDP_report_mode_counter += 1;
          
          
            if (update_arg( (void *)&(args_info->stats_port_arg), 
                 &(args_info->stats_port_orig), &(args_info->stats_port_given),
                &(local_args_info.stats_port_given), optarg, 0, 0, ARG_INT,
                check_ambiguity, override, 0, 0,
                "stats-port", '-',
                additional_error))
              goto failure;
          
          }
          
          break;
        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          goto failure;

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c%s\n", CMDLINE_PARSER_PACKAGE, c, (additional_error ? additional_error : ""));
          abort ();
        } /* switch */
    } /* while */


  update_multiple_arg((void *)&(args_info->serial_algorithm_arg),
    &(args_info->serial_algorithm_orig), args_info->serial_algorithm_given,
    local_args_info.serial_algorithm_given, 0,
    ARG_ENUM, serial_algorithm_list);

  args_info->serial_algorithm_given += local_args_info.serial_algorithm_given;
  local_args_info.serial_algorithm_given = 0;
  
  if (args_info->Daemon_mode_counter && args_info->UDP_report_mode_counter) {
    int Daemon_given[] = {args_info->log_given, args_info->daemon_given,  -1};
    const char *Daemon_desc[] = {"--log", "--daemon",  0};
    int UDP_report_given[] = {args_info->stats_server_given, args_info->stats_port_given,  -1};
    const char *UDP_report_desc[] = {"--stats-server", "--stats-port",  0};
    error += check_modes(Daemon_given, Daemon_desc, UDP_report_given, UDP_report_desc);
  }
  if (args_info->Daemon_mode_counter && args_info->UDP_time_mode_counter) {
    int Daemon_given[] = {args_info->log_given, args_info->daemon_given,  -1};
    const char *Daemon_desc[] = {"--log", "--daemon",  0};
    int UDP_time_given[] = {args_info->time_server_addr_given, args_info->time_server_port_given,  -1};
    const char *UDP_time_desc[] = {"--time-server-addr", "--time-server-port",  0};
    error += check_modes(Daemon_given, Daemon_desc, UDP_time_given, UDP_time_desc);
  }
  if (args_info->UDP_report_mode_counter && args_info->UDP_time_mode_counter) {
    int UDP_report_given[] = {args_info->stats_server_given, args_info->stats_port_given,  -1};
    const char *UDP_report_desc[] = {"--stats-server", "--stats-port",  0};
    int UDP_time_given[] = {args_info->time_server_addr_given, args_info->time_server_port_given,  -1};
    const char *UDP_time_desc[] = {"--time-server-addr", "--time-server-port",  0};
    error += check_modes(UDP_report_given, UDP_report_desc, UDP_time_given, UDP_time_desc);
  }
  
  if (check_required)
    {
      error += cmdline_parser_required2 (args_info, argv[0], additional_error);
    }

  cmdline_parser_release (&local_args_info);

  if ( error )
    return (EXIT_FAILURE);

  return 0;

failure:
  free_list (serial_algorithm_list, 0 );
  
  cmdline_parser_release (&local_args_info);
  return (EXIT_FAILURE);
}