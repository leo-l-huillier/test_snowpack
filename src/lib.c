/*
** , 2022
** test_snowpack
** File description:
** lib.c
*/
#include "include.h"

void str_overwrite_stdout()
{
  printf("%s", "> ");
  fflush(stdout);
}

int my_strstr(char *str , char const *to_find)
{
    int j = 0;

    for (int i = 0; str[i] != '\0'; i = i + 1) {
        if (to_find[j] != str[i])
            j = 0;
        if (to_find[j] == str[i])
            j++;
        if (to_find[j] == '\0')
            return 0;
    }
    return 1;
}

int is_secret(char *secret)
{
	if (strlen(secret) == 3) {
		for (int i = 0; secret[i] != '\0'; i++) {
			if (secret[i] < '0' || secret[i] > '9')
				return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

void str_trim_lf (char* arr, int length)
{
    int i;
    for (i = 0; i < length; i++) { // trim \n
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}