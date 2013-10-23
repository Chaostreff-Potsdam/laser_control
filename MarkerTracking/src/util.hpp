#pragma once

// Boost
#include <boost/utility.hpp>

// STL
#include <limits>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif


namespace {
    inline void clampAnglePos(double& a) {
        while (a < 0) a += 360;
        while (a >= 360) a -= 360;
    }

    inline void clampAngle(double& a) {
        while (a <= -180) a += 360;
        while (a > 180) a -= 360;
    }

    inline double clampSlope(double a) {
        while (a > 90) a -= 180;
        while (a <= -90) a += 180;
        return a;
    }

    inline double flipAngle(double a) {
        return a > 0 ? a - 180 : a + 180;
    }

    inline double angle(const double& x, const double& y, const double& l) {
        double a = std::asin(y / l) * 180 / M_PI;
        a = (x > 0 ? 360 + a : 180 - a);
        clampAnglePos(a);
        return a;
    }

    inline double angle(const double& x, const double& y) {
        return angle(x, y, std::sqrt(x*x + y*y));
    }

    inline double angleAbs(const double& a, const double& b) {
        double d = a - b;
        return (d > 180 ? d - 360 :
               (d < -180 ? d + 360 :
               d));
    }

    inline double difference(const double& a, const double& b) {
        return a > b ? a - b: -(b - a);
    }

    inline double median(std::vector<double> values) {
        std::sort(values.begin(), values.end());
        return values[values.size() / 2];
    }

    inline double absdev(const std::vector<double>& values, const double& tendency) {
        double sum = 0;
        for (std::vector<double>::const_iterator i = values.begin(); i < values.end(); i++) {
            sum += std::abs(*i - tendency);
        }
        return sum / values.size();
    }

    double mean(const std::vector<double>& values) {
        if (values.size() == 0) return 0;
        double result = 0;
        for (std::vector<double>::const_iterator i = values.begin(); i != values.end(); ++i) {
            result += *i;
        }
        return result / values.size();
    }

    inline double clampProbability(const double& p) {
        return std::max(0.0, std::min(1.0, p));
    }

    inline double gauss(const double& x, const double& c) {
        return std::exp(std::pow(x / c, 2) / -2.0);
    }

    inline bool isBottomLeft(double angle) {
        return angle < 45 || angle >= 315;
    }

    inline bool isTopLeft(double angle) {
        return angle < 135 && angle >= 45;
    }

    inline bool isTopRight(double angle) {
        return angle < 225 && angle >= 135;
    }

    inline bool isBottomRight(double angle) {
        return angle < 315 && angle >= 225;
    }

    inline double sigmoid(const double& x, const double &a) {
        return 0.5 * (1 + std::atan(a * (x - 0.5)) / std::atan(a / 2));
    }
}
