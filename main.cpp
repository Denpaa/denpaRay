//  main.cpp
//  Single file C-style denpaRay
//  Created by 電波

#include <cstdio>
#include <cstdint>
#include <cmath>
#include "common.hpp"
#include "tuple.hpp"
#include "matrix.hpp"
#include "tracer.hpp"
#include "miscellaneous.hpp"
#include "debug.hpp"

// -----------------------------------------------
// @denpa: The main function, where the magic happens.
// -----------------------------------------------
int main(UNUSED int argc, UNUSED const char** argv) {
	f32 wallZ = 10.f;
	f32 wallSize = 7.f;
	f32 half = wallSize/2.f;
	u32 canvasX = 1000;
	u32 canvasY = canvasX;
	colour* pixels = (colour*)safeMalloc(sizeof(colour) * canvasX * canvasY);
	f32 pixelSize = wallSize/canvasX;
	
	pointLight pointLight = {.intensity = createColour(1.f, 1.f, 1.f, 1.f), .position = createPoint(-10.f, 10.f, -10.f)};
	
	ray ray = {createPoint(0.f, 0.f, -5.f), createVector(0.f, 0.f, 1.f)};
	sphere sphere = createSphere();
	sphere.material.surfaceColour = createColour(1.f, .2f, 1.f, 1.f);
	
	for (u32 y = 0; y < canvasY; y++) {
		f32 worldY = half - (pixelSize*y);
		for (u32 x = 0; x < canvasX; x++) {
			f32 worldX = -half + (pixelSize * x);
			point position = createPoint(worldX, worldY, wallZ);
			ray.rayDirection = normalizeTuple(subtractTuples(position, ray.rayOrigin));
			intersection result = findRayHits(findSphereRayIntersections(&sphere, ray));
			
			if (result.object != NULL) {
				point intersectionPoint = findRayPosition(ray.rayOrigin, ray.rayDirection, result.t);
				vector normal = findNormalAt(&sphere, intersectionPoint);
				vector eye = negateTuple(ray.rayDirection);
				pixels[y * canvasY + x] = phongLighting(sphere.material, pointLight, intersectionPoint, eye, normal);
			}
		}
	}
	
	clampAndScaleColours(pixels, canvasX, canvasY);
	createPPMFile("denpa.ppm", canvasX, canvasY, pixels);
	free(pixels);
	
	return EXIT_SUCCESS;
}
