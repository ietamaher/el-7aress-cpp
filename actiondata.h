// actiondata.h
#ifndef ACTIONDATA_H
#define ACTIONDATA_H

#include <QString>
#include <QVariant>
#include <QMap>
#include <QMetaType>

struct ActionData {
    QString command_type;  // e.g., "move", "stop", "set_speed", "set_angle"
    QString target;        // e.g., "motor1", "PLC1"
    QMap<QString, QVariant> parameters;  // e.g., {"speed": 100, "angle": 45}

    ActionData() noexcept = default;

    // Custom constructor to initialize the members
    ActionData(const QString& cmdType, const QString& tgt, const QMap<QString, QVariant>& params) 
        : command_type(cmdType), target(tgt), parameters(params) {}

    ~ActionData() noexcept = default;
};


// Make ActionData known to Qt's meta-object system for signal-slot mechanism
Q_DECLARE_METATYPE(ActionData)

#endif // ACTIONDATA_H
