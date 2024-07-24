#include <vector>
#include <queue>
#include <mutex>
#include "../Region.hpp"
#include "ImageProcess.hpp"

class RegionMerger : public ImageProcess{
    private :
        std::unordered_map<short, Region*> identifiersForRegions;
        std::queue<std::pair<Region*, Region*>> pendingMerge;
        std::vector<Region*> regionsToRemove;
        std::mutex mutex;

        void mergeNeighbor(Region* region1, Region* region2);
        void findMergesForRegion(Region*);
        void findAllMerges();
        void computeRegionsNeighbors();
        void removeMergeConflict();
        void applyMerges();
    public:
        RegionMerger(std::vector<Region*>* listRegion);
        void execute();
};