#ifndef TRACKINGMANAGER_H
#define TRACKINGMANAGER_H

#include <QObject>
#include <QList>
#include "objectinfo.h"
#include "track.h"
#include <limits> 

 
class TrackingManager : public QObject {
    Q_OBJECT

public:
    //static const int MAX_MISSING_AGE = 30;
    const float DISTANCE_THRESHOLD = 250.0f;  // Example threshold value
    const int MAX_MISSING_AGE = 100;  
    TrackingManager(QObject *parent = nullptr);
    void testProcessObjectMetadata();  // Test method for simulating object metadata processing
    void testUpdateWithMockData();  // Test method for simulating object metadata update
    


public slots:
    void processObjectMetadata(const QList<ObjectInfo>& objects);
    void toggleTracking(bool enabled);

signals:
    void primaryObjectUpdated(const ObjectInfo& primaryObject);
    void trackUpdated(const ObjectInfo& trackInfo); // Signal to update or create a button for a track

private:
    QList<Track> tracks;
    bool trackingEnabled;
    void selectPrimaryObject();
};

#endif // TRACKINGMANAGER_H
