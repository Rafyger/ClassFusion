#ifndef EXPERTTEACHER_H
#define EXPERTTEACHER_H

#include "teacherprofile.h"

// Multi-level inheritance
class ExpertTeacher : public TeacherProfile
{
    double bonus;

public:
    ExpertTeacher(QString n, QString c, QString t,
                  double lec, double inter, double w, double ex,
                  double b)
        : TeacherProfile(n, c, t, lec, inter, w, ex), bonus(b) {}

    double computeScore() const override {
        return TeacherProfile::computeScore() + bonus;
    }
};

#endif // EXPERTTEACHER_H
