/* languages.h */

struct language_info
{
  sh_int	*pgsn;
  char		replace[100];
};

extern struct language_info language_table[MAX_LANGUAGE];
