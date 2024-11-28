#include "nanoflann.hpp"
#include "ofMain.h"
#include <vector>
#include <string>
#include <stdexcept>

template <typename DataSource, typename _DistanceType = float>
struct KDTreeVectorOfVec3Adaptor
{
    typedef KDTreeVectorOfVec3Adaptor<DataSource, _DistanceType> self_t;
    typedef nanoflann::KDTreeSingleIndexAdaptor<
        nanoflann::L2_Simple_Adaptor<_DistanceType, self_t>,
        self_t> KDTree;

    DataSource& pts;   // Reference to the vector of glm::vec3
    KDTree* index;     // Pointer to the KD-tree

    KDTreeVectorOfVec3Adaptor(DataSource& _pts, const size_t leaf_size = 10)
        : pts(_pts), index(nullptr)
    {
        index = new KDTree(3, *this, nanoflann::KDTreeSingleIndexAdaptorParams(leaf_size));
    }

    ~KDTreeVectorOfVec3Adaptor() { delete index; }

    // Number of points in the dataset
    inline size_t kdtree_get_point_count() const { return pts.size(); }

    // Get a specific dimension of a specific point
    inline _DistanceType kdtree_get_pt(const size_t idx, const size_t dim) const
    {
        if (dim == 0) return pts[idx].x;
        if (dim == 1) return pts[idx].y;
        return pts[idx].z;
    }

    // Bounding box (not used, return false)
    template <class BBOX>
    bool kdtree_get_bbox(BBOX&) const { return false; }

    // Compute the L2 (Euclidean) distance between a query point and a dataset point
    inline _DistanceType kdtree_distance(const _DistanceType* a, const size_t b_idx, size_t size) const
    {
        const glm::vec3& b = pts[b_idx];
        return (a[0] - b.x) * (a[0] - b.x) +
               (a[1] - b.y) * (a[1] - b.y) +
               (a[2] - b.z) * (a[2] - b.z);
    }
};

class ofxKDTree
{
public:
    typedef std::vector<glm::vec3> PointCloud;

    ofxKDTree() : mat_index(nullptr) {}

    ~ofxKDTree() { clear(); }

    void addPoint(const glm::vec3& sample) { samples.push_back(sample); }

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
        mat_index = new KDTreeVectorOfVec3Adaptor<PointCloud>(samples, 64);
        mat_index->index->buildIndex();
    }

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

private:
    KDTreeVectorOfVec3Adaptor<PointCloud>* mat_index;
    PointCloud samples;
};
