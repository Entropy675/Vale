#include "nanoflann.hpp"
#include "ofMain.h"
#include <vector>
#include <string>
#include <stdexcept>

// Store both spatial coordinates and associated ID
struct PointID
{
    glm::vec3 position;
    int id;
    PointID(const glm::vec3& pos, int id) : position(pos), id(id) {}
};

template <typename DataSource, typename _DistanceType = float>
struct KDTreeVectorOfPointIDAdaptor
{
    typedef KDTreeVectorOfPointIDAdaptor<DataSource, _DistanceType> self_t;
    typedef nanoflann::KDTreeSingleIndexAdaptor<
        nanoflann::L2_Simple_Adaptor<_DistanceType, self_t>,
        self_t> KDTree;

    DataSource& pts;   // Reference to the vector of PointIDs
    KDTree* index;     // Pointer to the KD-tree

    KDTreeVectorOfPointIDAdaptor(DataSource& _pts, const size_t leaf_size = 10)
        : pts(_pts), index(nullptr)
    {
        index = new KDTree(3, *this, nanoflann::KDTreeSingleIndexAdaptorParams(leaf_size));
    }

    ~KDTreeVectorOfPointIDAdaptor() { delete index; }

    // Number of points in the dataset
    inline size_t kdtree_get_point_count() const { return pts.size(); }

    // Get a specific dimension of a specific point
    inline _DistanceType kdtree_get_pt(const size_t idx, const size_t dim) const
    {
        const glm::vec3& point = pts[idx].position;
        if (dim == 0) return point.x;
        if (dim == 1) return point.y;
        return point.z;
    }

    // Bounding box (not used, return false)
    template <class BBOX>
    bool kdtree_get_bbox(BBOX&) const { return false; }

    // Compute the L2 (Euclidean) distance between a query point and a dataset point
    inline _DistanceType kdtree_distance(const _DistanceType* a, const size_t b_idx, size_t size) const
    {
        const glm::vec3& b = pts[b_idx].position;
        return (a[0] - b.x) * (a[0] - b.x) +
               (a[1] - b.y) * (a[1] - b.y) +
               (a[2] - b.z) * (a[2] - b.z);
    }
};

class ofxKDTree
{
public:
    typedef std::vector<PointID> PointCloud;

    ofxKDTree() : mat_index(nullptr) {}

    ~ofxKDTree() { clear(); }

    void addPoint(const glm::vec3& sample, int id) { samples.emplace_back(sample, id); }
    const std::vector<PointID>& getAllPoints() const { return samples; }

    void clear()
    {
        if (mat_index)
        {
            delete mat_index;
            mat_index = nullptr;
        }
        samples.clear();
    }

    void constructKDTree()
    {
        if (samples.empty())
            throw std::runtime_error("Error: no samples in KDTree");
        mat_index = new KDTreeVectorOfPointIDAdaptor<PointCloud>(samples, 64);
        mat_index->index->buildIndex();
    }

    // idea: check for collision against this guy, if true check for next nearist, etc until one is untrue
    // gives id of the current nearist neighbour
    int nearestNeighbour(const glm::vec3& query_pt)
    {
        if (!mat_index)
            throw std::runtime_error("KDTree not built yet!");

        std::vector<size_t> indexes(1); // Only one nearest neighbor
        std::vector<float> dists(1);    // Distance to the nearest neighbor
        nanoflann::KNNResultSet<float> resultSet(1);
        resultSet.init(&indexes[0], &dists[0]);

        float query[3] = {query_pt.x, query_pt.y, query_pt.z};
        mat_index->index->findNeighbors(resultSet, query, nanoflann::SearchParams(10));

        return samples[indexes[0]].id; // Return the ID of the nearest neighbor
    }

    // gives all info for nearest neighbours
    void getKNN(const glm::vec3& query_pt, int k, std::vector<size_t>& indexes, std::vector<float>& dists)
    {
        if (!mat_index)
            throw std::runtime_error("KDTree not built yet!");

        indexes.resize(k);
        dists.resize(k);
        nanoflann::KNNResultSet<float> resultSet(k);
        resultSet.init(&indexes[0], &dists[0]);

        float query[3] = {query_pt.x, query_pt.y, query_pt.z};
        mat_index->index->findNeighbors(resultSet, query, nanoflann::SearchParams(10));
    }

    // gives all ids of nearest neighbours
    std::vector<int> getKNNIDs(const glm::vec3& query_pt, int k)
    {
        std::vector<size_t> indexes;
        std::vector<float> dists;
        getKNN(query_pt, k, indexes, dists);

        std::vector<int> ids;
        for (size_t i = 0; i < indexes.size(); ++i)
            ids.push_back(samples[indexes[i]].id);
        return ids; // Return the IDs of the nearest neighbors
    }

private:
    KDTreeVectorOfPointIDAdaptor<PointCloud>* mat_index;
    PointCloud samples;
};
