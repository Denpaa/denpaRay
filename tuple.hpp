//  tuple.hpp
//  Contains all the functions related to tuples, colours, points and vectors
//  Created by 電波

#pragma once

// -----------------------------------------------
// @denpa: A tuple can represent a colour, a point or a vector.
// -----------------------------------------------
typedef union tuple {
	struct { // Represents a colour or a pixel.
		f32 r = 0.f;
		f32 g = 0.f;
		f32 b = 0.f;
		f32 a = 0.f;
	};
	struct { // When w is 1, the tuple represents a point. When it is 0, it repesents a vector.
		f32 x;
		f32 y;
		f32 z;
		f32 w;
	};
} tuple;

typedef tuple colour;
typedef tuple point;
typedef tuple vector;

STATIC_ASSERT(sizeof(tuple) == 4 * sizeof(f32), "Unexpected padding for union tuple.");

// -----------------------------------------------
// @denpa: Creates a colour.
// -----------------------------------------------
INTERNAL DINLINE colour createColour(f32 r, f32 g, f32 b, f32 a) {
	return colour {.r = r, .g = g, .b = b, .a = a};
}

// -----------------------------------------------
// @denpa: Creates a point.
// -----------------------------------------------
INTERNAL DINLINE point createPoint(f32 x, f32 y, f32 z) {
	return point {.x = x, .y = y, .z = z, .w = 1.f};
}

// -----------------------------------------------
// @denpa: Creates a vector.
// -----------------------------------------------
INTERNAL DINLINE vector createVector(f32 x, f32 y, f32 z) {
	return vector {.x = x, .y = y, .z = z, .w = 0.f};
}

// -----------------------------------------------
// @denpa: Adds one tuple to another.
// Never add two points or else w = 2.
// -----------------------------------------------
INTERNAL DINLINE tuple addTuples(tuple a, tuple b) {
	return tuple {.x = a.x+b.x, .y = a.y+b.y, .z = a.z+b.z, .w = a.w+b.w};
}

// -----------------------------------------------
// @denpa: Subtracts one tuple from another.
// Never subtract a point from a vector or else w = -1.
// -----------------------------------------------
INTERNAL DINLINE tuple subtractTuples(tuple a, tuple b) {
	return tuple {.x = a.x-b.x, .y = a.y-b.y, .z = a.z-b.z, .w = a.w-b.w};
}

// -----------------------------------------------
// @denpa: Mutiplies two tuples together.
// -----------------------------------------------
INTERNAL DINLINE tuple multiplyTuples(tuple a, tuple b) {
	return tuple {.x = a.x*b.x, .y = a.y*b.y, .z = a.z*b.z, .w = a.w*b.w};
}

// -----------------------------------------------
// @denpa: Calculate the magnitude of a vector.
// -----------------------------------------------
INTERNAL DINLINE f32 magnitudeOfTuple(tuple a) {
	return sqrtf((a.x*a.x) + (a.y*a.y) + (a.z*a.z) + (a.w*a.w));
}

// -----------------------------------------------
// @denpa: Normalizes a vector into a unit vector.
// -----------------------------------------------
INTERNAL DINLINE tuple normalizeTuple(tuple a) {
	f32 magnitude = magnitudeOfTuple(a);
	return tuple {.x = a.x/magnitude, .y = a.y/magnitude, .z = a.z/magnitude, .w = a.w/magnitude};
}

// -----------------------------------------------
// @denpa: Checks if the provided tuple is a unit vector.
// Not intended to be used with points.
// -----------------------------------------------
INTERNAL DINLINE bool isUnitTuple(tuple a) {
	return areFloatsEqual(magnitudeOfTuple(a), 1.f);
}

// -----------------------------------------------
// @denpa: Scales a tuple with the given scale factor.
// -----------------------------------------------
INTERNAL DINLINE tuple scaleTuple(tuple a, f32 scaleFactor) {
	return tuple {.x = a.x*scaleFactor, .y = a.y*scaleFactor, .z = a.z*scaleFactor, .w = a.w*scaleFactor};
}

// -----------------------------------------------
// @denpa: Negates the given tuple.
// -----------------------------------------------
INTERNAL DINLINE tuple negateTuple(tuple a) {
	return tuple {.x = -a.x, .y = -a.y, .z = -a.z, .w = a.w};
}

// -----------------------------------------------
// @denpa: Calculates the dot product of two vectors.
// -----------------------------------------------
INTERNAL DINLINE f32 dotProduct(vector a, vector b) {
	return (a.x*b.x) + (a.y*b.y) + (a.z*b.z) + (a.w*b.w);
}

// -----------------------------------------------
// @denpa: Calculates the cross product of two vectors.
// -----------------------------------------------
INTERNAL DINLINE vector crossProduct(vector a, vector b) {
	return createVector((a.y*b.z) - (a.z*b.y), (a.z*b.x) - (a.x*b.z), (a.x*b.y) - (a.y*b.x));
}

// -----------------------------------------------
// @denpa: Clamps all colours to lie between 0 and 1, and then expands them from 0 to 255.
// This is done so that it can be used to later write the colours to a PPM file.
// -----------------------------------------------
INTERNAL DNOINLINE void clampAndScaleColours(colour* pixels, u64 canvasX, u64 canvasY) {
	for (u64 y = 0; y < canvasY; y++) {
		for (u64 x = 0; x < canvasX; x++) {
			pixels[y * canvasY + x].r = DENPA_CLAMP(pixels[y*canvasY+x].r, 0.f, 1.f);
			pixels[y * canvasY + x].g = DENPA_CLAMP(pixels[y*canvasY+x].g, 0.f, 1.f);
			pixels[y * canvasY + x].b = DENPA_CLAMP(pixels[y*canvasY+x].b, 0.f, 1.f);
			pixels[y * canvasY + x] = scaleTuple(pixels[y*canvasY+x], 255.f);
		}
	}
}
