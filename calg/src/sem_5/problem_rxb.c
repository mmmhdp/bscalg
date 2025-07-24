#include <assert.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

#define DB
#define EMAIL_BUFF_SIZE 1000
#define EMAIL_PATTERN                                                         \
  "^[[:alnum:]._][[:alnum:]._]*@[[:alpha:]][[:alpha:]]*\\.[[:alpha:]][[:"     \
  "alpha:].]*[[:alpha:]]*$"

static char *
init_email_buff (char **email)
{
  free (*email);
  *email = malloc (EMAIL_BUFF_SIZE + 1);
  (*email)[EMAIL_BUFF_SIZE] = '\0';

  return *email;
}

int
is_valid_email (char *emailbuff)
{
  int res;
  regex_t regex;
  regcomp (&regex, EMAIL_PATTERN, 0);

  res = regexec (&regex, emailbuff, 0, NULL, 0);

  if (res == 0)
    {
#ifdef DB
      printf ("MATCH\n");
#endif
      regfree (&regex);
      return 1;
    }
  if (res == REG_NOMATCH)
    {
#ifdef DB
      printf ("NO MATCH\n");
#endif
      regfree (&regex);
      return 0;
    }

  switch (res)
    {
    case 0:
      regfree (&regex);
      return 1;
    case REG_NOMATCH:
      regfree (&regex);
      return 0;
    default:
      regfree (&regex);
      return 0;
    }

  fprintf (stderr, "Unreachable code reached!\n");
  abort ();
}

int
main (void)
{
  char *emailbuff;

  emailbuff = init_email_buff (&emailbuff);

  scanf ("%s", emailbuff);
#ifdef DB
  printf ("%s\n", emailbuff);
#endif
  if (is_valid_email (emailbuff))
    printf ("1");
  else
    printf ("0");

  free (emailbuff);
}
