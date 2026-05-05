#ifndef APPLICANTSTATSPROJECT_STATSPAGEMODEL_H
#define APPLICANTSTATSPROJECT_STATSPAGEMODEL_H

#include <QAbstractListModel>
#include <QVariantMap>
#include <memory>

#include "../applicants-faculty-data/faculty-direction.h"

namespace StatsConfig {
constexpr int kMinScoreBucket = 110;
constexpr int kMaxScoreBucket = 310;
constexpr int kBucketStep = 10;
constexpr int kBucketCount = (kMaxScoreBucket - kMinScoreBucket) / kBucketStep + 1;
}  // namespace StatsConfig

struct DirectionStats {

    QString name;
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
    QList<int> directionIndices;  // индексы в общем плоском списке DirectionStats
};

class StatsPageModel : public QObject {
    Q_OBJECT
public:
    StatsPageModel();

    void setFaculties(std::shared_ptr<QList<FacultyDirection>> data);

    Q_INVOKABLE QMap<QString, QVariant> directionStatsAt(int index) const;
    Q_INVOKABLE QMap<QString, QVariant> facultyStats(const QString &name) const;

private:
    void rebuildDirectionStats();
    void rebuildFacultyStats();  // агрегат по m_directions

    std::shared_ptr<QList<FacultyDirection>> m_source;
    QList<DirectionStats> m_directions;
    QList<FacultyStats> m_faculties;
};

#endif  // APPLICANTSTATSPROJECT_STATSPAGEMODEL_H
