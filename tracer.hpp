//  tracer.hpp
//  Contains functions, definitions and structures related to the actual tracing part of the program
//  Created by 電波

#pragma once

// -----------------------------------------------
// @denpa: Intersections between rays and objects in one structure.
// -----------------------------------------------
typedef struct intersection {
	void* object = NULL;
	u32 UNUSED_ALIGNMENT_PADDING;
	f32 t = 0.f;
} intersection;

#define MAX_INTERSECTION_COUNT 2

// -----------------------------------------------
// @denpa: A list of intersections
// -----------------------------------------------
typedef struct listOfIntersections{
	intersection intersections[MAX_INTERSECTION_COUNT] = {};
	u64 intersectionCount = 0;
} listOfIntersections;

// -----------------------------------------------
// @denpa: Ray data
// -----------------------------------------------
typedef struct ray {
	point rayOrigin = createPoint(0.f, 0.f, 0.f);
	vector rayDirection = createVector(0.f, 0.f, 0.f);
} ray;

// -----------------------------------------------
// @denpa: Material data
// -----------------------------------------------
typedef struct material {
	colour surfaceColour = createColour(0.f, 0.f, 0.f, 0.f);
	f32 ambient = 0.f;
	f32 diffuse = 0.f;
	f32 specular = 0.f;
	f32 shininess = 0.f;
} material;

// -----------------------------------------------
// @denpa: Creates the default material type.
// -----------------------------------------------
INTERNAL DNOINLINE material createMaterial(void) {
	return material {.surfaceColour = createColour(1.f, 1.f, 1.f, 1.f), .ambient = .1f, .diffuse = .9f, .specular = .9f, .shininess = 200.f};
}

// -----------------------------------------------
// @denpa: Sphere data
// -----------------------------------------------
typedef struct sphere {
	point origin = createPoint(0.f, 0.f, 0.f);
	f32 radius = 1.f;
	matrix4x4 transformation = identityMatrix4x4();
	material material = createMaterial();
} sphere;

// -----------------------------------------------
// @denpa: Creates a sphere with a radius of 1.f and a default transformation with the origin at 0.f, 0.f, 0.f.
// -----------------------------------------------
INTERNAL DNOINLINE sphere createSphere(void) {// TODO: increment sphereID each time. Find appropriate algorithm.
	static u32 sphereID;
	return (sphere) {.origin = {{0.f, 0.f, 0.f, 1.f}},
					.radius = 1.f,
					.transformation = identityMatrix4x4(),
					.material = createMaterial()};
}

// -----------------------------------------------
// @denpa: Defines a point light for the scene.
// -----------------------------------------------
typedef struct pointLight {
	colour intensity = createColour(0.f, 0.f, 0.f, 0.f);
	point position = createPoint(0.f, 0.f, 0.f);
} pointLight;

// -----------------------------------------------
// @denpa: Finds the position of the ray given its origin and direction.
// -----------------------------------------------
INTERNAL DINLINE point findRayPosition(tuple rayOrigin, tuple rayDirection, f32 t) {
	return addTuples(rayOrigin, scaleTuple(rayDirection, t));
}

// -----------------------------------------------
// @denpa: Transforms the ray by the provided transformation matrix.
// -----------------------------------------------
INTERNAL DINLINE ray transformRay(ray r, matrix4x4 transform) {
	return (ray) {.rayOrigin = multiplyMatrix4x4Tuple(transform, r.rayOrigin), .rayDirection = multiplyMatrix4x4Tuple(transform, r.rayDirection)};
}

// -----------------------------------------------
// @denpa: Finds the points at which the sphere and the ray intersects at.
// -----------------------------------------------
INTERNAL DINLINE listOfIntersections findSphereRayIntersections(sphere* sphere, ray ray) {
	listOfIntersections result {};
	ray = transformRay(ray, inverseMatrix4x4(sphere->transformation));
	tuple sphereToRay = subtractTuples(ray.rayOrigin, sphere->origin);
	f32 a = dotProduct(ray.rayDirection, ray.rayDirection);
	f32 b = 2.f * dotProduct(ray.rayDirection, sphereToRay);
	f32 discriminant = (b*b) - (4 * a * (dotProduct(sphereToRay, sphereToRay) - 1.f));
	
	if (discriminant < 0.f) {return result;}
	
	result.intersections[0].t = (-b - sqrtf(discriminant)) / (2*a);
	result.intersections[1].t = (-b + sqrtf(discriminant)) / (2*a);
	
	if (areFloatsEqual(result.intersections[0].t, result.intersections[1].t)) {
		result.intersectionCount += 1;
		result.intersections[1].t = 0.f;
	} else {
		result.intersectionCount += 2;
	}
	
	for (u32 i = 0; i < result.intersectionCount; i++) {result.intersections[i].object = sphere;}
	
	return result;
}

// -----------------------------------------------
// @denpa: Goes through the list of intersections and finds the intersection with the lowest positive t value.
// It is assumed that the list of intersections is sorted in ascending order.
// If it fails to find any positive t values, it will return a struct intersection with the values nulled out.
// -----------------------------------------------
INTERNAL DINLINE struct intersection findRayHits(listOfIntersections list) {
	for (u8 i = 0; i < list.intersectionCount; i++) {
		if (list.intersections[i].t > 0.f) {
			return list.intersections[i];
		}
	}
	return (intersection) {.object = NULL, .t = 0.f};
}

// -----------------------------------------------
// @denpa: The normal on the sphere is calculated.
// -----------------------------------------------
INTERNAL DINLINE vector findNormalAt(sphere* sphere, point worldPoint) {
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
INTERNAL DINLINE vector reflectVector(vector in, vector normal) {
	return subtractTuples(in, scaleTuple(normal, 2.f * dotProduct(in, normal)));
}

// -----------------------------------------------
// @denpa: For every intersection, the appropriate colour for the pixel is determined.
// -----------------------------------------------
INTERNAL DINLINE colour phongLighting(material material, pointLight pointLight, point point, vector eyeVector, vector normalVector) {
	colour effectiveColour = multiplyTuples(material.surfaceColour, pointLight.intensity);
	vector lightVector = normalizeTuple(subtractTuples(pointLight.position, point));
	colour ambient = scaleTuple(effectiveColour, material.ambient);
	
	colour diffuse = {};
	colour specular = {};
	
	f32 lightDotNormal = dotProduct(lightVector, normalVector);
	if (lightDotNormal < 0.f) {
		diffuse = createColour(0.f, 0.f, 0.f, 1.f);
		specular = createColour(0.f, 0.f, 0.f, 1.f);
	} else {
		diffuse = scaleTuple(effectiveColour, material.diffuse * lightDotNormal);
		vector reflectV = reflectVector(negateTuple(lightVector), normalVector);
		f32 reflectDotEye = dotProduct(reflectV, eyeVector);
		if (reflectDotEye <= 0.f) {
			specular = createColour(0.f, 0.f, 0.f, 1.f);
		} else {
			f32 factor = powf(reflectDotEye, material.shininess);
			specular = scaleTuple(pointLight.intensity, material.specular * factor);
		}
	}
	return addTuples(addTuples(ambient, diffuse), specular);
}
