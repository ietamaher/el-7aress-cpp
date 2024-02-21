
#include "trackingmanager.h"
#include <iostream>
#include <QDebug>

TrackingManager::TrackingManager(QObject *parent) : QObject(parent), trackingEnabled(true) {


}

void TrackingManager::processObjectMetadata(const QList<ObjectInfo>& objects) {
    if (!trackingEnabled) return;

    // Temporary list to keep track of updated tracks in this cycle
    QList<unsigned long> updatedTrackIds;

    for (const auto& detectedObject : objects) {
        bool matched = false;

        //qDebug() << "Processing detected object with unique ID:" << detectedObject.uniqueId;

        // Iterate over existing tracks to check if the detected object's ID is already being tracked
        for (const auto& track : tracks) {
            if (track.getUniqueId() == detectedObject.uniqueId) {
                matched = true;
                break; // Break the loop as we found a matching track
            }
        }

        // If no existing track matches the detected object's ID, create a new track
        if (!matched) {
            Track newTrack(detectedObject);
            tracks.append(newTrack);
            updatedTrackIds.append(newTrack.getUniqueId());
            
            //qDebug() << "emit trackUpdated ID:" << newTrack.getUniqueId();

            // Emit signal for UI update (new button creation)
            emit trackUpdated(newTrack.getCurrentInfo());
            //qDebug() << "trackUpdated update detected   ID:" << detectedObject.uniqueId;
        } else {
           // qDebug() << "Track already exists for ID:" << detectedObject.uniqueId << ", skipping update.";
        }
    }

    /*// Handle tracks that were not updated in this cycle (potentially lost or occluded objects)
    auto it = tracks.begin();
    while (it != tracks.end()) {
        if (!updatedTrackIds.contains(it->getUniqueId())) {
            it->incrementMissingAge();

            if (it->getMissingAge() > MAX_MISSING_AGE) {
                // If a track has been missing for too long, remove it
                it = tracks.erase(it);
                continue; // Skip the rest of the loop and proceed with the next iteration
            }
        } else {
            // Reset the missing age for tracks that were updated in this cycle
            it->resetMissingAge();
        }

        // Increment the age of all tracks
        it->incrementAge();
        ++it;
    }*/

    // Optionally, select a primary object for focusing, tracking, etc.
    //selectPrimaryObject();
}



void TrackingManager::toggleTracking(bool enabled) {
    trackingEnabled = enabled;
    if (!enabled) {
        tracks.clear();
    }
}


void TrackingManager::selectPrimaryObject() {
    if (tracks.isEmpty()) return;

    // Example: Select the track with the minimum missing age or another criterion
    auto primaryTrackIt = std::min_element(tracks.begin(), tracks.end(), [](const Track& a, const Track& b) {
        return a.getMissingAge() < b.getMissingAge();
    });

    if (primaryTrackIt != tracks.end()) {
        // Emit signal or perform actions with the primary track
        // For example: emit primaryObjectUpdated(primaryTrackIt->getCurrentInfo());
       // emit primaryObjectUpdated(primaryTrackIt->getCurrentInfo());
    }
}


void TrackingManager::testProcessObjectMetadata() {
    QList<ObjectInfo> mockObjects;

    qDebug() << "Meta Data received ..."; 
    // Create a few mock ObjectInfo instances
    ObjectInfo obj1;
    obj1.bboxLeft = 100; obj1.bboxRight = 100; obj1.bboxTop = 30; obj1.bboxBottom = 50; obj1.uniqueId = 1; 
    mockObjects.append(obj1);

    ObjectInfo obj2;  // Simulate another object, potentially the primary one
    obj2.bboxLeft = 150; obj2.bboxRight = 150; obj2.bboxTop = 50; obj2.bboxBottom = 60; obj2.uniqueId = 2; 
    mockObjects.append(obj2);

    // Process the mock data as if it was received normally
    processObjectMetadata(mockObjects);
    processObjectMetadata(mockObjects);
    processObjectMetadata(mockObjects);
    processObjectMetadata(mockObjects);
    processObjectMetadata(mockObjects);

    // Normally, you would have logic in processObjectMetadata to select the primary object
    // For this test, we'll just emit the second object as the primary one for simplicity
    //emit primaryObjectUpdated(obj2);
    //emit primaryObjectUpdated(obj1);

}

void TrackingManager::testUpdateWithMockData() {
    QList<ObjectInfo> mockObjects;

    // Create a few mock ObjectInfo instances
    ObjectInfo obj1{100, 150, 200, 250, 1};  // Example object
    ObjectInfo obj2{300, 350, 100, 150, 2};  // Another example object
    ObjectInfo obj3{80, 170, 100, 150, 2};  // Another example object

    mockObjects.append(obj1);
    mockObjects.append(obj2);
    mockObjects.append(obj3);

    // Process these mock objects as if they were detected objects
    processObjectMetadata(mockObjects);

    // Normally, selectPrimaryObject() is called within processObjectMetadata
    // to select and emit the primary object. However, if your logic doesn't
    // automatically select a primary object in processObjectMetadata, you
    // might need to call selectPrimaryObject() explicitly here.
}
