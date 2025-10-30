#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[])
{
	FILE *input, *output;

	int c;
	char s[3];

	if (argc != 3)
	{
		fprintf(stderr, "usage: hex input output\n");
		exit(EXIT_FAILURE);
	}

	input = fopen(argv[1], "r");
	if (input == NULL)
	{
		fprintf(stderr,"Error opening file \"%s\": %s\n", argv[1],
			strerror(errno));
		exit(EXIT_FAILURE);
	}

	output = fopen(argv[2], "wb");
	if (output == NULL)
	{
		fprintf(stderr, "Error opening file \"%s\": %s\n", argv[2],
			strerror(errno));
		exit(EXIT_FAILURE);
	}

	s[2] = 0;
	for (;;)
	{
		c = fgetc(input);

		if (c == EOF)
			break;
		
		if (isspace(c))
			continue;
		
		if (c == ';')
		{
			for (;;)
			{
				c = fgetc(input);

				if (c == '\n' || c == EOF)
					break;
			}

			continue;
		}

		if (c == '"')
		{
			for (;;)
			{
				c = fgetc(input);

				if (c == EOF)
				{
					fprintf(stderr, "Unexpected EOF\n");
					exit(EXIT_FAILURE);
				}

				if (c == '"')
					break;

				fputc(c, output);
			}

			continue;
		}

		if (c == '0')
		{
			c = fgetc(input);

			if (c == 'x' || c == 'X')
			{
				long int pos, target;

				pos = ftell(output);
				if (pos == -1)
				{
					fprintf(stderr,
						"Error reading position\n");
					exit(EXIT_FAILURE);
				}
				
				if (fscanf(input, "%lx", &target) != 1)
				{
					fprintf(stderr,
						"Error scanning position\n");
					exit(EXIT_FAILURE);
				}

				if (pos > target)
				{
					fprintf(stderr,
						"Invalid position: 0x%lx\n",
						target);
					exit(EXIT_FAILURE);
				}

				if (fseek(output, target, SEEK_SET) != 0)
				{
					fprintf(stderr,
						"Error setting position\n");
					exit(EXIT_FAILURE);
				}
				continue;
			}

			ungetc(c, input);
			c = '0';
		}

		if (!isxdigit(c))
		{
			fprintf(stderr,
				"Unexpected character: %c\n",
				c);
			exit(EXIT_FAILURE);
		}
		s[0] = c;

		c = fgetc(input);
		if (!isxdigit(c))
		{
			fprintf(stderr,
				"Unexpected character: %c\n",
				c);
			exit(EXIT_FAILURE);
		}
		s[1] = c;

		fputc(strtol(s, 0, 16), output);
	}

	fclose(input);
	fclose(output);

	return EXIT_SUCCESS;
}
