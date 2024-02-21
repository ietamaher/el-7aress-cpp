#include "track.h"


Track::Track(const ObjectInfo& initialInfo) : currentInfo(initialInfo) {}

void Track::update(const ObjectInfo& newInfo) {
    // Update the currentInfo with new data from newInfo
    currentInfo = newInfo;
}

ObjectInfo Track::getCurrentInfo() const {
    return currentInfo;
}

unsigned long Track::getUniqueId() const {
    return currentInfo.uniqueId;
}

unsigned int Track::getClassId() const {
    return currentInfo.classId;
}

float Track::getVisibility() const {
    return currentInfo.visibility;
}

float Track::distanceTo(const ObjectInfo& object) const {
    float center_x = (currentInfo.bboxLeft + currentInfo.bboxRight) / 2;
    float center_y = (currentInfo.bboxTop + currentInfo.bboxBottom) / 2;

    float object_center_x = (object.bboxLeft + object.bboxRight) / 2;
    float object_center_y = (object.bboxTop + object.bboxBottom) / 2;

    float dx = center_x - object_center_x;
    float dy = center_y - object_center_y;
    return std::sqrt(dx * dx + dy * dy);
}

void Track::incrementMissingAge() {
    missing_age++;
}

int Track::getMissingAge() const {
    return missing_age;
}

void Track::resetMissingAge() {
    missing_age = 0;
}

void Track::incrementAge() {
    age++;
}