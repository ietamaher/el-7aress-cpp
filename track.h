#ifndef TRACK_H
#define TRACK_H

#include "objectinfo.h"
#include <opencv2/opencv.hpp>  // Assuming you're using OpenCV's Kalman Filter
#include <cmath> 

class Track {
public:
    Track(const ObjectInfo& initialInfo);

    void update(const ObjectInfo& newInfo);
    ObjectInfo getCurrentInfo() const;
    unsigned long getUniqueId() const;
    unsigned int getClassId() const;
    float getVisibility() const;
    float distanceTo(const ObjectInfo& object) const;
    void incrementMissingAge(); 
    int getMissingAge() const;
    void resetMissingAge();
    void incrementAge();

private:
    ObjectInfo currentInfo;
    int age = 0;             // Age of the track
    int missing_age = 0;  
    
};

#endif // TRACK_H