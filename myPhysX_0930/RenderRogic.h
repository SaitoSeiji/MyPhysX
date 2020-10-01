#pragma once
#include <glut.h>
#include "PxPhysicsAPI.h"
using namespace physx;

namespace RenderRaw {
#define MAX_NUM_MESH_VEC3S  1024
	void renderGeometry(const PxGeometry& geom);
	static PX_FORCE_INLINE void renderGeometryHolder(const PxGeometryHolder& h) {
		renderGeometry(h.any());
	}
}
