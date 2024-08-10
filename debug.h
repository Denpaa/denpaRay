//  debug.hpp
//  Contains exclusively debugging functions
//  Created by 電波

#pragma once

// -----------------------------------------------
// @denpa: Intended to be used to run simple tests.
// -----------------------------------------------
INTERNAL DNOINLINE UNUSED void test(void) {

}

// -----------------------------------------------
// @denpa: Prints a colour.
// -----------------------------------------------
INTERNAL DNOINLINE UNUSED void printColour(colour a) {
	printf("R: %.5f, G: %.5f, B: %.5f\n", (double)a.r, (double)a.g, (double)a.b);
}

// -----------------------------------------------
// @denpa: Prints a tuple.
// -----------------------------------------------
INTERNAL DNOINLINE UNUSED void printTuple(tuple a) {
	printf("X: %.7f, Y: %.7f, Z: %.7f, W: %.7f\n", (double)a.x, (double)a.y, (double)a.z, (double)a.w);
}

// -----------------------------------------------
// @denpa: Prints a 4x4 matrix.
// -----------------------------------------------
INTERNAL DNOINLINE UNUSED void printMatrix4x4(matrix4x4 a) {
	for (u64 i = 0; i < 16; i+=4) {
		printf("%.7f %.7f %.7f %.7f\n", (double)a.v[i], (double)a.v[i+1], (double)a.v[i+2], (double)a.v[i+3]);
	}
}

// -----------------------------------------------
// @denpa: Prints individual intersections.
// -----------------------------------------------
INTERNAL DINLINE UNUSED void printIntersection(intersection i) {
	printf("t-value: %.4f\n", (double)i.t);
}

// -----------------------------------------------
// @denpa: Prints all of the intersections in a list of intersections.
// -----------------------------------------------
INTERNAL DNOINLINE UNUSED void printListOfIntersections(listOfIntersections list) {
	if (list.intersectionCount > 0) {
		for (u64 i = 0; i < list.intersectionCount; i++) {
			printf("ID: %llu, ", i);
			printIntersection(list.intersections[i]);
		}
	}
}
