#ifndef APPLICANTSTATSPROJECT_STATSPAGEMODEL_H
#define APPLICANTSTATSPROJECT_STATSPAGEMODEL_H

#include <QAbstractListModel>
#include <QVariantMap>
#include <memory>

#include "../applicants-faculty-data/faculty-direction.h"
#include "../models/applicants-list-model.h"

namespace StatsConfig {
constexpr int kMinScoreBucket = 110;
constexpr int kMaxScoreBucket = 310;
constexpr int kBucketStep = 10;
constexpr int kBucketCount = (kMaxScoreBucket - kMinScoreBucket) / kBucketStep + 1;
}  // namespace StatsConfig

struct DirectionStats {

    QString name;
    QString code;
    QString facultyName;
    StudyForm form = StudyForm::Error;
    StudyType type = StudyType::Error;

    int size = 0;
    int capacity = 0;

    std::array<int, StatsConfig::kBucketCount> scoreDistribution{};

    std::optional<int> maxSumScore;
    std::optional<int> minSumScore;
    std::optional<double> meanSumScore;
    std::optional<int> midSumScore;
    int sourceIndex = -1;
};

struct FacultyStats {

    QString facultyName;

    int totalSize = 0;
    int totalCapacity = 0;

    std::optional<int> maxSumScore;
    std::optional<int> minSumScore;
    std::optional<double> meanSumScore;
    QList<int> directionIndices;
};

class StatsPageModel : public QObject {
    Q_OBJECT
public:
    StatsPageModel();

    void setFaculties(std::shared_ptr<QList<FacultyDirection>> data);
    std::shared_ptr<ApplicantsListModel> getApplicantsListModel();

    Q_INVOKABLE QMap<QString, QVariant> directionStatsAt(int index) const;
    Q_INVOKABLE QMap<QString, QVariant> facultyStats(const QString &name) const;

    std::shared_ptr<ApplicantsListModel> getApplicantsListModel() const;

private:
    void rebuildDirectionStats();
    void rebuildFacultyStats();

    std::shared_ptr<QList<FacultyDirection>> m_source;
    QList<DirectionStats> m_directions;
    QList<FacultyStats> m_faculties;
    std::shared_ptr<ApplicantsListModel> m_applicantsListModel;
};

#endif  // APPLICANTSTATSPROJECT_STATSPAGEMODEL_H
