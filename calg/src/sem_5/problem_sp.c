#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *
replace (char *str, char const *from, char const *to)
{
  int offset;
  char *tbg, *tend, isbufused = 0, *buff = NULL;

  if (strcmp (from, to) == 0)
    {
      buff = malloc (strlen (str) + 1);
      strcpy (buff, str);
      return buff;
    }

  for (offset = 0;;)
    {
      tbg = strstr (str + offset, from);
      if (tbg == NULL)
        break;

      tend = tbg + strlen (from);
      *tbg = '\0';

      buff = malloc (strlen (str) + strlen (to) + strlen (tend) + 1);
      buff[0] = '\0';

      strcat (buff, str);
      strcat (buff, to);
      strcat (buff, tend);

      offset = strlen (str) + strlen (to);
      if (isbufused)
        free (str);

      str = buff;
      isbufused = 1;
    }

  return str;
}

int
main (void)
{
  char
      //*tstr = "Hello, %username, how are you, %username",
      //*tfrom = "%username",
      //*tto = "Eric, the Blood Axe",

      *tstr
      = "h E EMRTxExEx iAxExExbTxExEx xExExG Tkp xExExcTDxExEx q xCjUfxExEx "
        "dahfS",
      *tfrom = "xExEx", *tto = "xExEx",

      *str, *from, *to, *rstr;

  str = malloc (strlen (tstr) + 1);
  strcpy (str, tstr);

  from = malloc (strlen (tfrom) + 1);
  strcpy (from, tfrom);

  to = malloc (strlen (tto) + 1);
  strcpy (to, tto);

  printf ("str : ||%s||\n", str);
  printf ("from: ||%s||\n", from);
  printf ("to  : ||%s||\n", to);

  rstr = replace (str, from, to);

  printf ("||%s||\n", rstr);

  if (strcmp (to, to) == 0)
    printf ("%s is equal to %s", to, to);

  free (rstr);
  free (to);
  free (from);
  free (str);
}
