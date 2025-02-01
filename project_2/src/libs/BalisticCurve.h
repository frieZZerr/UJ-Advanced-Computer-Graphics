#ifndef BALISTICCURVE_H
#define BALISTICCURVE_H

#include <osg/Vec3>
#include <osg/Quat>
#include <cmath>
#include <algorithm>
#include <vector>

static const float _distancePerHold = 7.5f;
static const osg::Vec3 _zAxisVector(0.0f, 0.0f, 1.0f);

inline osg::Vec3 computeVelocity(
    const osg::Quat cannonRotation,
    const float holdDuration)
{
    float distanceFactor = holdDuration * _distancePerHold;

    osg::Vec3 localDirection(_zAxisVector);
    osg::Vec3 worldDirection = cannonRotation * localDirection;

    float finalDistance = _distancePerHold + distanceFactor;
    osg::Vec3 velocity  = worldDirection * finalDistance;

    return velocity;
}

inline float computeTimeToGround(
    const osg::Vec3& start,
    const osg::Vec3& velocity,
    float gravity)
{
    float z0 = start.z();
    float vz = velocity.z();
    float a  = 0.5f * gravity;
    float b  = vz;
    float c  = z0;

    float discriminant = b*b - 4.0f*a*c;
    if (discriminant < 0.0f) {
        // No real solution => never hits ground
        return -1.0f;
    }

    float sqrtDisc = std::sqrt(discriminant);

    float t1 = (-b + sqrtDisc) / (2.0f * a);
    float t2 = (-b - sqrtDisc) / (2.0f * a);

    float tPos = -1.0f;
    // Choose smallest positive root
    if (t1 >= 0.0f && t2 >= 0.0f) {
        tPos = std::min(t1, t2);
    } else if (t1 >= 0.0f) {
        tPos = t1;
    } else if (t2 >= 0.0f) {
        tPos = t2;
    }

    return tPos;
}

inline std::vector<std::pair<float, osg::Vec3>> computeCurvePositionsWithTime(
    int numSteps,
    const osg::Vec3& startPosition,
    const osg::Vec3& velocity,
    float gravity,
    float timeToGroundHit=6.0f)
{
    std::vector<std::pair<float, osg::Vec3>> timePositionPairs;
    timePositionPairs.reserve(numSteps + 1);

    if (timeToGroundHit != 6.0f) {
        timeToGroundHit = computeTimeToGround(startPosition, velocity, gravity);
    }
    if (timeToGroundHit < 0.0f) {
        // No valid ground hit => return empty or partial
        return timePositionPairs;
    }

    for (int i = 0; i <= numSteps; ++i) {
        float t = timeToGroundHit * static_cast<float>(i) / numSteps;

        float x = startPosition.x() + velocity.x() * t;
        float y = startPosition.y() + velocity.y() * t;
        float z = startPosition.z() + velocity.z() * t + 0.5f * gravity * t * t;

        osg::Vec3 position(x, y, z);
        timePositionPairs.emplace_back(t, position);
    }

    return timePositionPairs;
}

#endif // BALISTICCURVE_H
