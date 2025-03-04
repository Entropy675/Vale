#ifndef SPHEREPACKINGEXPERIMENT_H__
#define SPHEREPACKINGEXPERIMENT_H__

#include "ofMain.h"
#include <vector>
#include <queue>
#include <unordered_set>

struct Sphere {
    glm::vec3 center;
    float radius;
    int entityId;
};

class SpherePacker {
public:
        // Main function to generate spheres inside a mesh
        std::vector<Sphere> generateSpheres(const ofMesh& mesh, int entityId, float minRadius,
                                        float maxRadius, float densityFactor, int maxSpheres) {
        // Output storage
        std::vector<Sphere> spheres;

        // 1. Create a voxel grid to represent the mesh interior
        // First, find the bounding box of the mesh
        glm::vec3 minBounds, maxBounds;
        getBoundingBox(mesh, minBounds, maxBounds);

        // Voxelize the mesh to determine interior points
        float voxelSize = minRadius * 0.5f;
        VoxelGrid grid = voxelizeMesh(mesh, minBounds, maxBounds, voxelSize);

        // 2. Set up priority queue for sphere generation
        std::priority_queue<CandidatePoint> candidates;

        // Seed the queue with interior points from the voxel grid
        seedCandidatePoints(grid, candidates, densityFactor);

        // 3. Generate spheres using greedy algorithm
        std::unordered_set<glm::ivec3, IVec3Hash> occupiedVoxels;

        while (!candidates.empty() && spheres.size() < maxSpheres) {
            // Get highest priority candidate
            CandidatePoint candidate = candidates.top();
            candidates.pop();

            // Skip if this voxel is already occupied
            glm::ivec3 voxelPos = pointToVoxel(candidate.position, minBounds, voxelSize);
            if (occupiedVoxels.find(voxelPos) != occupiedVoxels.end()) {
                continue;
            }

            // Calculate maximum possible radius for this point
            float maxPossibleRadius = calculateMaxRadius(candidate.position, mesh, grid,
                                                        spheres, minBounds, maxBounds, voxelSize);

            // Apply density-based scaling
            float adjustedRadius = adjustRadiusByDensity(maxPossibleRadius, candidate.density,
                                                       minRadius, maxRadius);

            // Only add if radius is sufficient
            if (adjustedRadius >= minRadius) {
                Sphere newSphere;
                newSphere.center = candidate.position;
                newSphere.radius = adjustedRadius;
                newSphere.entityId = entityId;
                spheres.push_back(newSphere);

                // Mark occupied voxels
                markOccupiedVoxels(newSphere, minBounds, voxelSize, occupiedVoxels);
            }
        }

        return spheres;
    }

private:
    // Voxel grid representation
    struct VoxelGrid {
        std::vector<bool> data;
        glm::ivec3 dimensions;
        glm::vec3 origin;
        float voxelSize;

        bool isInterior(int x, int y, int z) const {
            if (x < 0 || y < 0 || z < 0 ||
                x >= dimensions.x || y >= dimensions.y || z >= dimensions.z) {
                return false;
            }
            int index = x + y * dimensions.x + z * dimensions.x * dimensions.y;
            return data[index];
        }
    };

    struct CandidatePoint {
        glm::vec3 position;
        float density;
        float priority;

        bool operator<(const CandidatePoint& other) const {
            return priority < other.priority;
        }
    };

    struct IVec3Hash {
        size_t operator()(const glm::ivec3& v) const {
            return std::hash<int>()(v.x) ^
                   (std::hash<int>()(v.y) << 1) ^
                   (std::hash<int>()(v.z) << 2);
        }
    };

    // Find mesh bounding box
    void getBoundingBox(const ofMesh& mesh, glm::vec3& minBounds, glm::vec3& maxBounds) {
        const std::vector<glm::vec3>& vertices = mesh.getVertices();
        if (vertices.empty()) return;

        minBounds = maxBounds = vertices[0];
        for (const auto& v : vertices) {
            minBounds = glm::min(minBounds, v);
            maxBounds = glm::max(maxBounds, v);
        }

        // Add some padding
        glm::vec3 padding = (maxBounds - minBounds) * 0.01f;
        minBounds -= padding;
        maxBounds += padding;
    }

    // Convert point to voxel coordinates
    glm::ivec3 pointToVoxel(const glm::vec3& point, const glm::vec3& origin, float voxelSize) {
        glm::vec3 relative = point - origin;
        return glm::ivec3(relative / voxelSize);
    }

    // Voxelize the mesh to determine interior points
    VoxelGrid voxelizeMesh(const ofMesh& mesh, const glm::vec3& minBounds,
                          const glm::vec3& maxBounds, float voxelSize) {
        // Calculate grid dimensions
        glm::vec3 dimensions = maxBounds - minBounds;
        glm::ivec3 gridDim = glm::ivec3(ceil(dimensions / voxelSize)) + glm::ivec3(1);

        // Initialize grid
        VoxelGrid grid;
        grid.dimensions = gridDim;
        grid.origin = minBounds;
        grid.voxelSize = voxelSize;
        grid.data.resize(gridDim.x * gridDim.y * gridDim.z, false);

        // First pass - mark all triangles
        markTrianglesInGrid(mesh, grid);

        // Second pass - flood fill from outside to identify interior
        floodFillFromExterior(grid);

        return grid;
    }

    // Mark all triangles in the voxel grid
    void markTrianglesInGrid(const ofMesh& mesh, VoxelGrid& grid) {
        const std::vector<glm::vec3>& vertices = mesh.getVertices();
        const std::vector<ofIndexType>& indices = mesh.getIndices();

        for (size_t i = 0; i < indices.size(); i += 3) {
            glm::vec3 v0 = vertices[indices[i]];
            glm::vec3 v1 = vertices[indices[i+1]];
            glm::vec3 v2 = vertices[indices[i+2]];

            // Voxelize this triangle using a 3D version of the rasterization algorithm
            voxelizeTriangle(v0, v1, v2, grid);
        }
    }

    // Voxelize a triangle
    void voxelizeTriangle(const glm::vec3& v0, const glm::vec3& v1,
                         const glm::vec3& v2, VoxelGrid& grid) {
        // Simplified implementation - in practice, you'd use a proper triangle voxelization algorithm
        // This is just a placeholder

        // Get triangle bounding box
        glm::vec3 minTri = glm::min(glm::min(v0, v1), v2);
        glm::vec3 maxTri = glm::max(glm::max(v0, v1), v2);

        // Convert to voxel coordinates
        glm::ivec3 minVoxel = pointToVoxel(minTri, grid.origin, grid.voxelSize);
        glm::ivec3 maxVoxel = pointToVoxel(maxTri, grid.origin, grid.voxelSize);

        // Clamp to grid bounds
        minVoxel = glm::max(minVoxel, glm::ivec3(0));
        maxVoxel = glm::min(maxVoxel, grid.dimensions - glm::ivec3(1));

        // Mark all voxels that intersect the triangle
        for (int x = minVoxel.x; x <= maxVoxel.x; x++) {
            for (int y = minVoxel.y; y <= maxVoxel.y; y++) {
                for (int z = minVoxel.z; z <= maxVoxel.z; z++) {
                    // Get voxel center
                    glm::vec3 voxelCenter = grid.origin + glm::vec3(x, y, z) * grid.voxelSize +
                                           grid.voxelSize * 0.5f;

                    // Check if voxel center is inside or close to triangle
                    if (pointTriangleDistance(voxelCenter, v0, v1, v2) < grid.voxelSize * 0.5f) {
                        int index = x + y * grid.dimensions.x + z * grid.dimensions.x * grid.dimensions.y;
                        grid.data[index] = true;
                    }
                }
            }
        }
    }

    // Distance from point to triangle
    float pointTriangleDistance(const glm::vec3& p, const glm::vec3& a,
                               const glm::vec3& b, const glm::vec3& c) {
        // Simplified implementation - in practice, use a proper point-triangle distance function
        // This is just a placeholder

        // Project point onto triangle plane
        glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
        float distance = glm::abs(glm::dot(normal, p - a));

        // Check if projection is inside triangle
        // (simplified - in practice, use barycentric coordinates)

        return distance;
    }

    // Flood fill from exterior to identify interior voxels
    void floodFillFromExterior(VoxelGrid& grid) {
        // Create a temporary grid to mark filled voxels
        std::vector<bool> exteriorMark(grid.data.size(), false);

        // Queue for BFS
        std::queue<glm::ivec3> queue;

        // Start from the corners and edges of the grid
        for (int x = 0; x < grid.dimensions.x; x++) {
            for (int y = 0; y < grid.dimensions.y; y++) {
                for (int z = 0; z < grid.dimensions.z; z++) {
                    if (x == 0 || y == 0 || z == 0 ||
                        x == grid.dimensions.x - 1 ||
                        y == grid.dimensions.y - 1 ||
                        z == grid.dimensions.z - 1) {

                        // Add to queue if not already marked as triangle
                        int index = x + y * grid.dimensions.x + z * grid.dimensions.x * grid.dimensions.y;
                        if (!grid.data[index]) {
                            queue.push(glm::ivec3(x, y, z));
                            exteriorMark[index] = true;
                        }
                    }
                }
            }
        }

        // BFS to mark all exterior voxels
        const int dirs[6][3] = {
            {1, 0, 0}, {-1, 0, 0},
            {0, 1, 0}, {0, -1, 0},
            {0, 0, 1}, {0, 0, -1}
        };

        while (!queue.empty()) {
            glm::ivec3 current = queue.front();
            queue.pop();

            // Check all 6 neighbors
            for (int d = 0; d < 6; d++) {
                glm::ivec3 next = current + glm::ivec3(dirs[d][0], dirs[d][1], dirs[d][2]);

                // Check bounds
                if (next.x < 0 || next.y < 0 || next.z < 0 ||
                    next.x >= grid.dimensions.x ||
                    next.y >= grid.dimensions.y ||
                    next.z >= grid.dimensions.z) {
                    continue;
                }

                // Check if already visited or is a triangle
                int nextIndex = next.x + next.y * grid.dimensions.x +
                               next.z * grid.dimensions.x * grid.dimensions.y;
                if (exteriorMark[nextIndex] || grid.data[nextIndex]) {
                    continue;
                }

                // Mark as exterior and add to queue
                exteriorMark[nextIndex] = true;
                queue.push(next);
            }
        }

        // Invert the exterior marks to get interior
        for (size_t i = 0; i < grid.data.size(); i++) {
            // Interior = Not exterior AND not triangle boundary
            if (!exteriorMark[i]) {
                grid.data[i] = true;  // Mark as interior
            } else {
                grid.data[i] = false; // Mark as exterior
            }
        }
    }

    // Seed candidate points for sphere placement
    void seedCandidatePoints(const VoxelGrid& grid, std::priority_queue<CandidatePoint>& candidates,
                            float densityFactor) {
        // Sample interior points from the voxel grid
        for (int x = 0; x < grid.dimensions.x; x++) {
            for (int y = 0; y < grid.dimensions.y; y++) {
                for (int z = 0; z < grid.dimensions.z; z++) {
                    if (grid.isInterior(x, y, z)) {
                        // Calculate point density based on distance to boundary
                        float density = calculatePointDensity(x, y, z, grid, densityFactor);

                        // Create candidate point
                        CandidatePoint candidate;
                        candidate.position = grid.origin + glm::vec3(x, y, z) * grid.voxelSize +
                                            grid.voxelSize * 0.5f;
                        candidate.density = density;

                        // Priority based on distance from boundary and density
                        // Higher density = higher priority
                        candidate.priority = density;

                        candidates.push(candidate);
                    }
                }
            }
        }
    }

    // Calculate density of a point based on distance to boundary
    float calculatePointDensity(int x, int y, int z, const VoxelGrid& grid, float densityFactor) {
        // Simplified implementation - distance to nearest non-interior voxel
        int maxDist = 0;
        int searchRadius = 10;  // Limit search radius for performance

        for (int dx = 1; dx <= searchRadius; dx++) {
            bool foundBoundary = false;

            // Check cube shell at distance dx
            for (int i = -dx; i <= dx; i++) {
                for (int j = -dx; j <= dx; j++) {
                    // Top and bottom faces
                    if (!grid.isInterior(x + i, y + j, z + dx) ||
                        !grid.isInterior(x + i, y + j, z - dx)) {
                        foundBoundary = true;
                        break;
                    }

                    // Left and right faces
                    if (!grid.isInterior(x + dx, y + i, z + j) ||
                        !grid.isInterior(x - dx, y + i, z + j)) {
                        foundBoundary = true;
                        break;
                    }

                    // Front and back faces
                    if (!grid.isInterior(x + i, y + dx, z + j) ||
                        !grid.isInterior(x + i, y - dx, z + j)) {
                        foundBoundary = true;
                        break;
                    }
                }
                if (foundBoundary) break;
            }

            if (foundBoundary) {
                maxDist = dx;
                break;
            }

            // If we reached the search radius limit without finding a boundary
            if (dx == searchRadius) {
                maxDist = searchRadius;
            }
        }

        // Normalize and apply density factor
        float density = glm::clamp(float(maxDist) / float(searchRadius), 0.0f, 1.0f);
        return pow(density, densityFactor);  // Higher densityFactor = more contrast in density
    }

    // Calculate maximum possible radius for a sphere at this position
    float calculateMaxRadius(const glm::vec3& position, const ofMesh& mesh,
                            const VoxelGrid& grid, const std::vector<Sphere>& existingSpheres,
                            const glm::vec3& minBounds, const glm::vec3& maxBounds,
                            float voxelSize) {
        // 1. Distance to mesh boundary (approximated using voxel grid)
        float distToBoundary = approximateDistanceToBoundary(position, grid, voxelSize);

        // 2. Distance to existing spheres
        float distToSpheres = calculateDistanceToSpheres(position, existingSpheres);

        // Return minimum of the two constraints
        return glm::min(distToBoundary, distToSpheres);
    }

    // Approximate distance to mesh boundary using voxel grid
    float approximateDistanceToBoundary(const glm::vec3& position, const VoxelGrid& grid,
                                      float voxelSize) {
        glm::ivec3 voxel = pointToVoxel(position, grid.origin, voxelSize);

        // Use distance field approach
        int maxSearch = 20;  // Limit search radius

        for (int r = 0; r <= maxSearch; r++) {
            bool foundBoundary = false;

            // Check all voxels at distance r
            for (int dx = -r; dx <= r; dx++) {
                for (int dy = -r; dy <= r; dy++) {
                    for (int dz = -r; dz <= r; dz++) {
                        // Only check voxels at exact distance r (Manhattan distance)
                        if (abs(dx) + abs(dy) + abs(dz) != r) continue;

                        glm::ivec3 checkVoxel = voxel + glm::ivec3(dx, dy, dz);

                        // Check if this voxel is outside or at the boundary
                        if (!grid.isInterior(checkVoxel.x, checkVoxel.y, checkVoxel.z)) {
                            foundBoundary = true;
                            break;
                        }
                    }
                    if (foundBoundary) break;
                }
                if (foundBoundary) break;
            }

            if (foundBoundary) {
                return r * voxelSize;
            }
        }

        // If we reached search limit without finding boundary
        return maxSearch * voxelSize;
    }

    // Calculate minimum distance to existing spheres
    float calculateDistanceToSpheres(const glm::vec3& position,
                                    const std::vector<Sphere>& existingSpheres) {
        float minDist = std::numeric_limits<float>::max();

        for (const auto& sphere : existingSpheres) {
            float dist = glm::distance(position, sphere.center) - sphere.radius;
            minDist = glm::min(minDist, dist);
        }

        return minDist;
    }

    // Adjust radius based on density
    float adjustRadiusByDensity(float maxRadius, float density, float minRadius, float maxAllowedRadius) {
        // Scale radius based on density
        // Higher density = larger radius
        float radius = minRadius + (maxAllowedRadius - minRadius) * density;

        // Ensure radius doesn't exceed maximum possible
        return glm::min(radius, maxRadius);
    }

    // Mark voxels occupied by a sphere
    void markOccupiedVoxels(const Sphere& sphere, const glm::vec3& minBounds,
                           float voxelSize, std::unordered_set<glm::ivec3, IVec3Hash>& occupiedVoxels) {
        // Get sphere bounds in voxel coordinates
        glm::ivec3 minVoxel = pointToVoxel(sphere.center - glm::vec3(sphere.radius),
                                          minBounds, voxelSize);
        glm::ivec3 maxVoxel = pointToVoxel(sphere.center + glm::vec3(sphere.radius),
                                          minBounds, voxelSize);

        // Mark all voxels within the sphere
        for (int x = minVoxel.x; x <= maxVoxel.x; x++) {
            for (int y = minVoxel.y; y <= maxVoxel.y; y++) {
                for (int z = minVoxel.z; z <= maxVoxel.z; z++) {
                    glm::vec3 voxelCenter = minBounds + glm::vec3(x, y, z) * voxelSize +
                                           voxelSize * 0.5f;

                    // Check if voxel center is inside sphere
                    if (glm::distance(voxelCenter, sphere.center) <= sphere.radius) {
                        occupiedVoxels.insert(glm::ivec3(x, y, z));
                    }
                }
            }
        }
    }
};

/*
// Simple usage example
void testSphereGeneration(ofMesh& mesh, int entityId) {
    SpherePacker packer;
    std::vector<Sphere> spheres = packer.generateSpheres(
        mesh,           // The mesh to fill
        entityId,       // Entity ID to assign to spheres
        0.1f,           // Minimum radius
        2.0f,           // Maximum radius
        2.0f,           // Density factor (higher = more contrast)
        1000            // Maximum number of spheres
    );

    // Now use the spheres for physics simulation, alchemy, etc.
    std::cout << "Generated " << spheres.size() << " spheres" << std::endl;
}
*/


#endif
