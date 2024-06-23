//
//  debug.h
//  Contains exclusively debugging functions
//  Created by 電波
//

#pragma once

static void test(void) {

}

// -----------------------------------------------
// @denpa:
// -----------------------------------------------
DNOINLINE UNUSED void printColour(colour a) {
	printf("R: %.5f, G: %.5f, B: %.5f\n", (double)a.r, (double)a.g, (double)a.b);
}

// -----------------------------------------------
// @denpa:
// -----------------------------------------------
DNOINLINE UNUSED void printTuple(tuple a) {
	printf("X: %.7f, Y: %.7f, Z: %.7f, W: %.7f\n", (double)a.x, (double)a.y, (double)a.z, (double)a.w);
}

// -----------------------------------------------
// @denpa: Intended for debugging. Prints a 4x4 matrix.
// -----------------------------------------------
DNOINLINE UNUSED void printMatrix4x4(matrix4x4 a) {
	for (uint32_t i = 0; i < 16; i+=4) {
		printf("%.7f %.7f %.7f %.7f\n", (double)a.v[i], (double)a.v[i+1], (double)a.v[i+2], (double)a.v[i+3]);
	}
}

// -----------------------------------------------
// @denpa: Intended for debugging. Prints individual intersections.
// -----------------------------------------------
DINLINE UNUSED void printIntersection(intersection intersection) {
	printf("%d, %.4f\n", intersection.objectType, (double)intersection.t); //TODO: Make it print the OBJECT_TYPE string instead of the raw integers.
}

// -----------------------------------------------
// @denpa: Intended for debugging. Prints all of the intersections in a list of intersections.
// -----------------------------------------------
DNOINLINE UNUSED void printIntersectionsList(intersectionsList intersectionsList) {
	if (intersectionsList.intersectionCount > 0) {
		for (uint32_t i = 0; i < intersectionsList.intersectionCount; i++) {
			printIntersection(intersectionsList.intersections[i]);
		}
	}
}
