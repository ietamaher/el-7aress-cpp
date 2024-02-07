
#ifndef PLCACTIONDATA_H
#define PLCACTIONDATA_H

#include <QString>
#include <QVariant>
#include <QMap>
#include <QMetaType>

struct PlcActionData {
    QString command_type;  // e.g., "move", "stop", "set_speed", "set_angle"
    QString target;        // e.g., "motor1", "PLC1"
    QMap<QString, QVariant> parameters;  // e.g., {"speed": 100, "angle": 45}

    PlcActionData() noexcept = default;

    // Custom constructor to initialize the members
    PlcActionData(const QString& cmdType, const QString& tgt, const QMap<QString, QVariant>& params) 
        : command_type(cmdType), target(tgt), parameters(params) {}

    ~PlcActionData() noexcept = default;
};


// Make ActionData known to Qt's meta-object system for signal-slot mechanism
Q_DECLARE_METATYPE(PlcActionData)

#endif // PLCACTIONDATA_H
