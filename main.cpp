//
//  main.cpp
//  Single file C-style denpaRay
//  Created by 電波
//

#include <cstdio>
#include <cstdint>
#include <cmath>
#include "common.h"
#include "tuple.h"
// TODO: A few potential improvements in matrix.hpp
#include "matrix.h"

// -----------------------------------------------
// @denpa: These defines are used for different object types.
// -----------------------------------------------
#define OBJECT_TYPE_NULL 0
#define OBJECT_TYPE_SPHERE 1
#define OBJECT_TYPE_CUBE 2
#define OBJECT_TYPE_PLANE 3
#define OBJECT_TYPE_CYLINDER 4

// -----------------------------------------------
// @denpa:
// -----------------------------------------------
typedef struct intersection {
	void* object = NULL;
	uint32_t objectType = OBJECT_TYPE_NULL;
	float t = 0.f;
} intersection;

// -----------------------------------------------
// @denpa:
// -----------------------------------------------
typedef struct intersectionsList{
	intersection intersections[4] = {};
	uint64_t intersectionCount = 0;
} intersectionsList;

// -----------------------------------------------
// @denpa:
// -----------------------------------------------
typedef struct ray {
	point rayOrigin = createPoint(0.f, 0.f, 0.f);
	vector rayDirection = createVector(0.f, 0.f, 0.f);
} ray;

typedef struct material {
	colour surfaceColour = createColour(0.f, 0.f, 0.f, 0.f);
	float ambient = 0.f;
	float diffuse = 0.f;
	float specular = 0.f;
	float shininess = 0.f;
} material;

DNOINLINE material createMaterial(void) {
	return material {.surfaceColour = createColour(1.f, 1.f, 1.f, 1.f), .ambient = .1f, .diffuse = .9f, .specular = .9f, .shininess = 200.f};
}

// -----------------------------------------------
// @denpa:
// -----------------------------------------------
typedef struct sphere {
	uint32_t sphereID;
	point origin = createPoint(0.f, 0.f, 0.f);
	float radius = 1.f;
	matrix4x4 transformation = identityMatrix4x4();
	material material = createMaterial();
} sphere;

// -----------------------------------------------
// @denpa: Creates a sphere with a radius of 1.f and a default transformation with the origin at 0.f, 0.f, 0.f.
// -----------------------------------------------
DNOINLINE sphere createSphere(void) {// TODO: increment sphereID each time. Find appropriate algorithm.
	static uint32_t sphereID = 0;
	return (sphere) {.sphereID = sphereID++,
					.origin = {{0.f, 0.f, 0.f, 1.f}},
					.radius = 1.f,
					.transformation = identityMatrix4x4(),
					.material = createMaterial()};
}

// -----------------------------------------------
// @denpa:
// -----------------------------------------------
typedef struct pointLight {
	colour intensity = createColour(0.f, 0.f, 0.f, 0.f);
	point position = createPoint(0.f, 0.f, 0.f);
} pointLight;

// -----------------------------------------------
// @denpa:
// -----------------------------------------------
DINLINE point findRayPosition(tuple rayOrigin, tuple rayDirection, float t) {
	return addTuples(rayOrigin, scaleTuple(rayDirection, t));
}

// -----------------------------------------------
// @denpa: Transforms the ray by the provided transformation matrix.
// -----------------------------------------------
DINLINE ray transformRay(struct ray r, struct matrix4x4 transform) {
	return (ray) {.rayOrigin = multiplyMatrix4x4Tuple(transform, r.rayOrigin), .rayDirection = multiplyMatrix4x4Tuple(transform, r.rayDirection)};
}

// -----------------------------------------------
// @denpa:
// -----------------------------------------------
DINLINE intersectionsList findSphereRayIntersections(sphere* sphere, ray ray) {
	intersectionsList result {};
	ray = transformRay(ray, inverseMatrix4x4(sphere->transformation));
	tuple sphereToRay = subtractTuples(ray.rayOrigin, sphere->origin);
	float a = dotProduct(ray.rayDirection, ray.rayDirection);
	float b = 2.f * dotProduct(ray.rayDirection, sphereToRay);
	float discriminant = (b*b) - (4 * a * (dotProduct(sphereToRay, sphereToRay) - 1.f));
	
	if (discriminant < 0.f) {
		return result;
	}
	
	result.intersections[0].t = (-b - sqrtf(discriminant)) / (2*a);
	result.intersections[1].t = (-b + sqrtf(discriminant)) / (2*a);
	
	if (areFloatsEqual(result.intersections[0].t, result.intersections[1].t)) {
		result.intersectionCount += 1;
		result.intersections[1].t = 0.f;
	} else {
		result.intersectionCount += 2;
		if (result.intersections[0].t > result.intersections[1].t) { // Implement a faster sorting algorithm.
			float temporary = result.intersections[1].t;
			result.intersections[1].t = result.intersections[0].t;
			result.intersections[0].t = temporary;
		}
	}
	
	for (uint32_t i = 0; i < result.intersectionCount; i++) { //TODO: This may have to be fixed at some point. It blindly labels everything as a sphere.
		result.intersections[i].object = sphere;
		result.intersections[i].objectType = OBJECT_TYPE_SPHERE;
	}
	
	return result;
}

// -----------------------------------------------
// @denpa: Goes through the list of intersections and finds the intersection with the lowest positive t value.
// It is assumed that the list of intersections is sorted in ascending order.
// If it fails to find any positive t values, it will return a struct intersection with the values nulled out.
// -----------------------------------------------
DINLINE struct intersection findRayHits(intersectionsList intersectionList) {
	for (uint32_t i = 0; i < intersectionList.intersectionCount; i++) {
		if (intersectionList.intersections[i].t > 0.f) {
			return intersectionList.intersections[i];
		}
	}
	return (intersection) {.object = NULL, .objectType = OBJECT_TYPE_NULL, .t = 0.f};
}

// -----------------------------------------------
// @denpa: The normal on the sphere is calculated.
// -----------------------------------------------
DINLINE vector findNormalAt(sphere* sphere, point worldPoint) {
	point objectPoint = multiplyMatrix4x4Tuple(inverseMatrix4x4(sphere->transformation), worldPoint);
	vector objectNormal = subtractTuples(objectPoint, sphere->origin);
	vector worldNormal = multiplyMatrix4x4Tuple(transposeMatrix4x4(inverseMatrix4x4(sphere->transformation)), objectNormal);
	worldNormal.w = 0.f;
	return normalizeTuple(worldNormal);
}

// -----------------------------------------------
// @denpa: The reflected vector on a plane is calculated and returned.
// Formula: inVector - normalVector * 2 * dotProductOf(inVector, normalVector)
// -----------------------------------------------
DINLINE vector reflectVector(vector in, vector normal) {
	return subtractTuples(in, scaleTuple(normal, 2.f * dotProduct(in, normal)));
}

// -----------------------------------------------
// @denpa: For every interesction, the appropriate colour for the pixel is determined.
// -----------------------------------------------
DINLINE colour lighting(material material, pointLight pointLight, point point, vector eyeVector, vector normalVector) {
	colour effectiveColour = multiplyTuples(material.surfaceColour, pointLight.intensity);
	vector lightVector = normalizeTuple(subtractTuples(pointLight.position, point));
	colour ambient = scaleTuple(effectiveColour, material.ambient);
	
	colour diffuse = {};
	colour specular = {};
	
	float lightDotNormal = dotProduct(lightVector, normalVector);
	if (lightDotNormal < 0.f) {
		diffuse = createColour(0.f, 0.f, 0.f, 1.f);
		specular = createColour(0.f, 0.f, 0.f, 1.f);
	} else {
		diffuse = scaleTuple(effectiveColour, material.diffuse * lightDotNormal);
		vector reflectV = reflectVector(negateTuple(lightVector), normalVector);
		float reflectDotEye = dotProduct(reflectV, eyeVector);
		if (reflectDotEye <= 0.f) {
			specular = createColour(0.f, 0.f, 0.f, 1.f);
		} else {
			float factor = powf(reflectDotEye, material.shininess);
			specular = scaleTuple(pointLight.intensity, material.specular * factor);
		}
	}
	return addTuples(addTuples(ambient, diffuse), specular);
}

typedef struct world {
	void* objects = NULL;
	uint64_t objectCount = 0;
	pointLight* pointLights = NULL;
	uint64_t pointLightCount = 0;
} world;

DINLINE intersectionsList findSphereRayIntersectionsWorld(world w, ray r) {
	for (uint32_t i = 0; i < w.objectCount; i++) {
		
	}
	return intersectionsList {};
}

// -----------------------------------------------
// @denpa: A safer version of malloc.
// -----------------------------------------------
DNOINLINE void* safeMalloc(size_t structSize) {
	void* data = malloc(structSize);
	if (!data) {
		printf("safeMalloc() failed with structSize: %lu", structSize);
		exit(EXIT_FAILURE);
	}
	return data;
}

// -----------------------------------------------
// @denpa: Uses the provided colour data to create a .ppm file.
// -----------------------------------------------
DNOINLINE void createPPMFile(const char* fileName, uint32_t x, uint32_t y, colour* pixels) {
	FILE* output = fopen(fileName, "wb");
	if (!output) {perror("fopen() in createPPMFile() failed."); return;}
	fprintf(output, "P3\n%d %d\n255\n", x, y);
	for (uint32_t i = 0; i < (x*y); i++) {
		fprintf(output, "%d %d %d\n", (int)((pixels+i)->r), (int)((pixels+i)->g), (int)((pixels+i)->b));
	}
	fclose(output);
}

#include "debug.h"

// -----------------------------------------------
// @denpa: The main function, where the magic happens.
// -----------------------------------------------
int main(UNUSED int argc, UNUSED const char** argv) {
	float wallZ = 10.f;
	float wallSize = 7.f;
	float half = wallSize/2.f;
	uint32_t canvasX = 1000, canvasY = canvasX;
	colour* pixels = (colour*)safeMalloc(sizeof(colour)*canvasX*canvasY);
	float pixelSize = wallSize/canvasX;
	
	pointLight pointLight = {.intensity = createColour(1.f, 1.f, 1.f, 1.f), .position = createPoint(-10.f, 10.f, -10.f)};
	
	sphere sphere1 = createSphere();
	sphere1.material.surfaceColour = createColour(.8f, 1.f, .6f, 1.f);
	sphere1.material.diffuse = .7f;
	sphere1.material.specular = .2f;
	
	sphere sphere2 = createSphere();
	sphere2.transformation = createScaleMatrix(.5f, .5f, .5f);
	
	sphere spheres[] = {sphere1, sphere2};
	
	world world;
	world.pointLightCount = 1;
	world.pointLights = &pointLight;
	world.objects = spheres;
	world.objectCount = 2;
	
	ray ray = {createPoint(0.f, 0.f, -5.f), createVector(0.f, 0.f, 1.f)};
	sphere sphere = createSphere();
	sphere.material.surfaceColour = createColour(1.f, .2f, 1.f, 1.f);
	
	test();
	
	uint32_t totalRaysHit = 0;
	
	for (uint32_t y = 0; y < canvasY; y++) {
		float worldY = half - (pixelSize*y);
		for (uint32_t x = 0; x < canvasX; x++) {
			float worldX = -half + (pixelSize * x);
			point position = createPoint(worldX, worldY, wallZ);
			ray.rayDirection = normalizeTuple(subtractTuples(position, ray.rayOrigin));
			
			intersection result = findRayHits(findSphereRayIntersections(&sphere, ray));
			
			if (result.object != NULL) {
				totalRaysHit++;
				point intersectionPoint = findRayPosition(ray.rayOrigin, ray.rayDirection, result.t);
				vector normal = findNormalAt(&sphere, intersectionPoint);
				vector eye = negateTuple(ray.rayDirection);
				pixels[y*canvasY+x] = lighting(sphere.material, world.pointLights[0], intersectionPoint, eye, normal);
			}
		}
	}
	
	printf("Total rays hit: %d\n", totalRaysHit);
	clampAndScaleColours(pixels, canvasX, canvasY);
	createPPMFile("gold.ppm", canvasX, canvasY, pixels);
	free(pixels);
	return EXIT_SUCCESS;
}
