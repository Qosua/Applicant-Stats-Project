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
    Q_INVOKABLE void setLastChosenDirectionInfo(const QString & lastChosenCode, const QString & lastChosenDirectionName,
                                                const QString & lastChosenStudyType, const QString & lastChosenStudyForm);
    Q_INVOKABLE void setLastChosenFacultyInfo(const QString & lastChosenFacultyName);

    std::shared_ptr<ApplicantsListModel> getApplicantsListModel() const;

signals:
    void loadDirectionPage(const int& index);
    void loadFacultyPage(const QString& facultyName);
    void nothingToLoad();

private:
    void rebuildDirectionStats();
    void rebuildFacultyStats();
    void gettingReadyPrevPage();

    std::shared_ptr<QList<FacultyDirection>> m_facultiesList;
    std::shared_ptr<ApplicantsListModel> m_applicantsListModel;

    QList<DirectionStats> m_directionsStats;
    QList<FacultyStats> m_facultiesStats;

    QString m_lastChosenCode;
    QString m_lastChosenDirectionName;
    QString m_lastChosenStudyType;
    QString m_lastChosenStudyForm;
    QString m_lastChosenFacultyName;
};

#endif  // APPLICANTSTATSPROJECT_STATSPAGEMODEL_H
