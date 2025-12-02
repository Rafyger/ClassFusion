#ifndef TEACHERPROFILE_H
#define TEACHERPROFILE_H

#include "teacherbase.h"

class TeacherProfile : public TeacherBase
{
protected:
    QString name;
    QString course;
    QString tags;

    double lecture;
    double interactive;
    double writing;
    double exams;

public:
    TeacherProfile() :
        lecture(0), interactive(0), writing(0), exams(0) {}

    TeacherProfile(QString n, QString c, QString t,
                   double lec, double inter, double w, double ex)
        : name(n), course(c), tags(t),
        lecture(lec), interactive(inter), writing(w), exams(ex) {}

    QString getName() const override { return name; }
    QString getCourse() const override { return course; }
    QString getTags() const override { return tags; }

    // Virtual scoring
    double computeScore() const override {
        return (interactive * 0.4 +
                lecture * 0.3 +
                writing * 0.2 +
                exams * 0.1);
    }

    // Compile-time polymorphism — overload
    double computeScore(double weightAdjust) const {
        return computeScore() * weightAdjust;
    }

    double getLecture()     const { return lecture; }
    double getInteractive() const { return interactive; }
    double getWriting()     const { return writing; }
    double getExams()       const { return exams; }
};

#endif // TEACHERPROFILE_H
