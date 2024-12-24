#include "offsets.h"
#include <iostream>
#include <fstream>
#include <filesystem>

void validate_arguments(int argc)
{
	if (argc > 2) {
		printf("Too many arguments!\n");
		system("pause");
		exit(-1);
	}
	if (argc < 2) {
		printf("Drag your LR2body/LRHbody onto this file!\n");
		system("pause");
		exit(-1);
	}
}

int main(int argc, char* argv[])
{
	validate_arguments(argc);

	int width, height;

	printf("Enter Width: ");
	scanf("%i", &width);
	printf("Enter Height: ");
	scanf("%i", &height);

	std::uintmax_t filesize = std::filesystem::file_size(argv[1]);

	uint8_t* buf = new uint8_t[filesize];
	std::ifstream fin(argv[1], std::ios::binary);
	fin.read((char*)(buf), filesize);
	if (!fin) {
		printf("Couldn't read file!\n");
		return -1;
	}

	fin.close();

	/* patch integer offsets */
	for (auto offset : offsets.width_int) {
		printf("%i -> %i\n", *(int*)(buf + offset), width);
		*(int*)(buf + offset) = width;
	}

	for (auto offset : offsets.height_int) {
		printf("%i -> %i\n", *(int*)(buf + offset), height);
		*(int*)(buf + offset) = height;
	}

	/* patch float offsets*/
	printf("%f -> %f\n", *(double*)(buf + offsets.width_float), (double)width);
	*(double*)(buf + offsets.width_float) = (double)width;
	printf("%f -> %f\n", *(double*)(buf + offsets.height_float), (double)height);
	*(double*)(buf + offsets.height_float) = (double)height;

	printf("Patched!\n");
	std::ofstream fout("LR2body Patched.exe", std::ios::binary);
	for (int i = 0; i < filesize; ++i) {
		fout << buf[i];
	}

	fout.close();
	system("pause");

	return 0;
}
