#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef MAME2003PLUS
#define FONT_WIDTH 8
#define FONT_HEIGHT 8
#else
#define FONT_WIDTH 6
#define FONT_HEIGHT 10
#endif

#define BUFFERSIZE ((FONT_WIDTH * FONT_HEIGHT + 7) / 8)
static unsigned char fontimage[BUFFERSIZE];

char *removeFileExt(char *myStr)
{
	char *retStr, *lastExt;

	if (myStr == NULL)
		return NULL;

	if ((retStr = malloc(strlen(myStr) + 1)) == NULL)
		return NULL;

	strcpy(retStr, myStr);
	lastExt = strrchr(retStr, '.');

	if (lastExt != NULL)
		*lastExt = '\0';

	return retStr;
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage:\n");
		printf("%s [input file]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	SDL_Init(SDL_INIT_VIDEO);
	SDL_ShowCursor(SDL_DISABLE);

	char *inputName_str = argv[1];
	char *inputNameNoExt_str = removeFileExt(inputName_str);

	SDL_Surface *image = IMG_Load(inputName_str);
	printf("w = %d, h = %d\n", image->w, image->h);
	printf("bits = %d\n", image->format->BitsPerPixel);
	printf("pixels = %p\n", image->pixels);

	SDL_Surface *screen = SDL_CreateRGBSurface(SDL_SWSURFACE, image->w, image->h, 32, 0, 0, 0, 0);
	SDL_BlitSurface(image, NULL, screen, NULL);

	printf("w = %d, h = %d\n", screen->w, screen->h);
	printf("bits = %d\n", screen->format->BitsPerPixel);
	printf("pixels = %p\n", screen->pixels);

#ifdef MAME2003PLUS
	int num_of_chars = (screen->w / 8) * (screen->h / 8);
#else
	int num_of_chars = (screen->w / 16) * (screen->h / 16);
#endif
	printf("num of chars = %d\n", num_of_chars);

	uint32_t *src = (uint32_t *)screen->pixels;

	int offset_x, offset_y;
	int i = 33;

	char outputCName_str[FILENAME_MAX];
	char outputBinName_str[FILENAME_MAX];

	sprintf(outputCName_str, "%s.h", inputNameNoExt_str);
	sprintf(outputBinName_str, "%s.bin", inputNameNoExt_str);

	FILE *pFile_OutputBin = fopen(outputBinName_str, "wb");

	FILE *pFile_Output = fopen(outputCName_str, "wb");
	fprintf(pFile_Output, "/* FontName = %s */ \n", inputName_str);
	fprintf(pFile_Output, "/* generated with png2c by trngaje */ \n\n");

	fprintf(pFile_Output, "#define FONT_WIDTH %d\n", FONT_WIDTH);
	fprintf(pFile_Output, "#define FONT_HEIGHT %d\n", FONT_HEIGHT);

	fprintf(pFile_Output, "\nstatic const unsigned char bitmap_bin[%d] = {\n", num_of_chars * BUFFERSIZE);

	for (i = 0; i < num_of_chars; i++)
	{
		memset(fontimage, 0, BUFFERSIZE);
		printf("code = 0x%x\n", i);

#ifdef MAME2003PLUS
		offset_x = (i % 114) * 8;
		offset_y = (i / 114) * 8;
#else
		offset_x = (i % 16) * 16;
		offset_y = (i / 16) * 16;
#endif
		for (int y = 0; y < FONT_HEIGHT; y++)
		{
			for (int x = 0; x < FONT_WIDTH; x++)
			{
#ifdef MAME2003PLUS
				unsigned char rem = 1 << (7 - ((x + y * FONT_WIDTH) & 7));
#else
				unsigned char rem = 1 << ((x + y * FONT_WIDTH) & 7);
#endif
				unsigned char offset = (x + y * FONT_WIDTH) >> 3;
				if (*(src + (y + offset_y) * screen->w + x + offset_x) == 0xffffff)
				{
					fontimage[offset] |= rem;
				}
			}
		}

		for (int y = 0; y < BUFFERSIZE; y++)
		{
			// const unsigned char buffer = fontimage[y];
			fprintf(pFile_Output, "0x%02X%s", fontimage[y], ((i == num_of_chars - 1) && (y == BUFFERSIZE - 1)) ? "\0" : ", ");
			fwrite(&fontimage[y], sizeof(fontimage[y]), 1, pFile_OutputBin);
		}

		fprintf(pFile_Output, " \n");

		// display font
		printf("\n");
		for (int jj = 0; jj < FONT_HEIGHT; jj++)
		{
			for (int ii = 0; ii < FONT_WIDTH; ii++)
			{
#ifdef MAME2003PLUS
				unsigned char rem = 1 << (7 - ((ii + jj * FONT_WIDTH) & 7));
#else
				unsigned char rem = 1 << ((ii + jj * FONT_WIDTH) & 7);
#endif
				unsigned char offset = (ii + jj * FONT_WIDTH) >> 3;
				if ((fontimage[offset] & rem) > 0)
					printf("#");
				else
					printf("_");
			}
			printf("\n");
		}
		printf("\n");
	}

	fprintf(pFile_Output, "};\n");

	if (pFile_Output != NULL)
		fclose(pFile_Output);

	if (pFile_OutputBin != NULL)
		fclose(pFile_OutputBin);

	SDL_Quit();

	return EXIT_SUCCESS;
}

#ifdef WIN32

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   char *command_line, int show_command)
{
	int argc;
	char **argv;

	char *arg;
	int index;
	int result;

	// count the arguments

	argc = 1;
	arg = command_line;

	while (arg[0] != 0)
	{

		while (arg[0] != 0 && arg[0] == ' ')
		{
			arg++;
		}

		if (arg[0] != 0)
		{

			argc++;

			while (arg[0] != 0 && arg[0] != ' ')
			{
				arg++;
			}
		}
	}

	// tokenize the arguments

	argv = (char **)malloc(argc * sizeof(char *));

	arg = command_line;
	index = 1;

	while (arg[0] != 0)
	{

		while (arg[0] != 0 && arg[0] == ' ')
		{
			arg++;
		}

		if (arg[0] != 0)
		{

			argv[index] = arg;
			index++;

			while (arg[0] != 0 && arg[0] != ' ')
			{
				arg++;
			}

			if (arg[0] != 0)
			{
				arg[0] = 0;
				arg++;
			}
		}
	}

	// put the program name into argv[0]

	char filename[_MAX_PATH];

	GetModuleFileName(NULL, filename, _MAX_PATH);
	argv[0] = filename;

	// call the user specified main function

	result = main(argc, argv);

	free(argv);
	return result;
}

#endif
