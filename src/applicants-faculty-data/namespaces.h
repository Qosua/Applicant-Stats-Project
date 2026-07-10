#ifndef NAMESPACES_H
#define NAMESPACES_H

enum class ApplicantsFilterFlags {
    All,
    AdmissionsTrue,
    AdmissionsFalse,
    Error = 9999911,
};

enum class StudyType {
    NonBudget,
    Budget,
    SpecialRight,
    Kvot,
    CompanySponsor,
    Error = 9999922,
};

enum class StudyForm {
    Personal,
    PersonalNotPersonal,
    NotPersonal,
    Error = 9999933,
};

#endif // NAMESPACES_H
