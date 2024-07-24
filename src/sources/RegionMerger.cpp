#include "../headers/ImageProcessing/RegionMerger.hpp"
#include "../Settings.hpp"
#include <unordered_set>
#include <iostream>
#include <thread>

RegionMerger::RegionMerger(std::vector<Region*>* _listRegion): ImageProcess(_listRegion){
    this->listRegion = _listRegion;
    for (Region* region : *listRegion){
        identifiersForRegions[region->getID()] = region;
    }
    regionsToRemove = std::vector<Region*>();
}

void RegionMerger::execute(){
    int actualRegionNumber = 0;

    while (actualRegionNumber != listRegion->size()){
        actualRegionNumber = listRegion->size();
        computeRegionsNeighbors();
        findAllMerges();
        applyMerges();
    }

}

void RegionMerger::mergeNeighbor(Region* region1, Region* region2){
    region1->fuseRegion(region2);
    region1->updateBorder();

    mutex.lock();
    regionsToRemove.push_back(region2);
    mutex.unlock();
}

void RegionMerger::computeRegionsNeighbors(){
    std::vector<std::thread> listThread;
    for (Region* region : *listRegion){
        listThread.push_back(std::thread(&Region::findNeighbors, region));
    }
    for (int i = 0; i < listThread.size();i++){
        listThread[i].join();
    }
}

void RegionMerger::findAllMerges(){
    std::vector<std::thread> listThread;
    for (Region* region : *listRegion){
        listThread.push_back(std::thread(&RegionMerger::findMergesForRegion, this, region));
    }
    for (int i = 0; i < listThread.size();i++){
        listThread[i].join();
    }
    listThread.clear();
    removeMergeConflict();
}

void RegionMerger::findMergesForRegion(Region* region){
    for (short regionId : region->getNeighbors()){
        Region* candidate = identifiersForRegions[regionId];
        if (region->verifyColor(candidate->getColor(), REGION_COLOR_SIMILARITY_THRESHOLD)) {
            mutex.lock();
            pendingMerge.push(std::pair<Region*, Region*>(region, candidate));
            mutex.unlock();
            break;
        }
    }
}

void RegionMerger::removeMergeConflict(){
    std::unordered_set<Region*> mergedRegions;
    std::queue<std::pair<Region*, Region*>> newPendingMergeList;
    while (!pendingMerge.empty()){
        std::pair<Region*, Region*> merge = pendingMerge.front();

        if (mergedRegions.find(merge.first) == mergedRegions.end() && 
                mergedRegions.find(merge.second) == mergedRegions.end()){
            newPendingMergeList.push(merge);
        }
        mergedRegions.insert(merge.first);
        mergedRegions.insert(merge.second);
        pendingMerge.pop();
    }
    pendingMerge = newPendingMergeList;
}

void RegionMerger::applyMerges(){
    std::vector<std::thread> listThread;
    while(!pendingMerge.empty()){
        std::pair<Region*, Region*> merge = pendingMerge.front();
        listThread.push_back(std::thread(&RegionMerger::mergeNeighbor,this, merge.first, merge.second));
        pendingMerge.pop();
    }
    for (int i = 0; i < listThread.size();i++){
        listThread[i].join();
    }

    for (int i = 0; i < regionsToRemove.size();i++){
        identifiersForRegions.erase(regionsToRemove[i]->getID());
        listRegion->erase(std::remove(listRegion->begin(), 
                listRegion->end(), regionsToRemove[i]), listRegion->end());
        delete regionsToRemove[i]; 
    }
    regionsToRemove.clear();
}