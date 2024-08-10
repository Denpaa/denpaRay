//  miscellaneous.hpp
//  Contains miscellaneous functions
//  Created by 電波

#pragma once

// -----------------------------------------------
// @denpa: A safer version of malloc.
// -----------------------------------------------
INTERNAL DNOINLINE void* safeMalloc(size_t structSize) {
	void* data = malloc(structSize);
	if (!data) {
		printf("safeMalloc() failed with structSize: %lu", structSize);
		exit(EXIT_FAILURE);
	}
	return data;
}

// -----------------------------------------------
// @denpa: Uses the provided colour data to create a .ppm file.
// This function does not fully follow the ppm specification.
// -----------------------------------------------
INTERNAL DNOINLINE void createPPMFile(const char* fileName, u32 x, u32 y, colour* pixels) {
	FILE* output = fopen(fileName, "wb");
	if (!output) {perror("fopen() in createPPMFile() failed."); return;}
	fprintf(output, "P3\n%d %d\n255\n", x, y);
	for (u32 i = 0; i < (x*y); i++) {
		fprintf(output, "%d %d %d\n", (int)((pixels+i)->r), (int)((pixels+i)->g), (int)((pixels+i)->b));
	}
	fclose(output);
}
