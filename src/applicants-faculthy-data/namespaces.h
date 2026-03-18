#ifndef NAMESPACES_H
#define NAMESPACES_H

enum class ApplicantsFilterFlags {
    All              = 1 << 0, //Take all admissions from applicants
    AdmissionsTrue   = 1 << 1, //Take applicants only with accepted admissions
    AdmissionsFalse  = 1 << 2, //Take applicants only with denied admissions
};

enum class StudyType {
    NonBudget      = 1 << 3,
    Budget         = 1 << 4,
    SpecialRight   = 1 << 5,
    Kvot           = 1 << 6,
    CompanySponsor = 1 << 7,
    Error = 1 << 8,
};

enum class StudyForm {
    Personal = 1 << 9,
    PersonalNotPersonal = 1 << 10,
    NotPersonal = 1 << 11,
    Error = 1 << 12,
};

#endif // NAMESPACES_H
