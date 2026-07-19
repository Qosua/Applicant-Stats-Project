#include "stats-page-model.h"

StatsPageModel::StatsPageModel() {
    m_applicantsListModel = std::make_shared<ApplicantsListModel>(this);
}

void StatsPageModel::setFaculties(std::shared_ptr<QList<FacultyDirection>> data) {

    m_facultiesList = std::move(data);
    rebuildDirectionStats();
    rebuildFacultyStats();
    gettingReadyPrevPage();
}

std::shared_ptr<ApplicantsListModel> StatsPageModel::getApplicantsListModel() {
    return m_applicantsListModel;
}

void StatsPageModel::setLastChosenDirectionInfo(const QString & lastChosenCode, const QString & lastChosenDirectionName,
                                       const QString & lastChosenStudyType, const QString & lastChosenStudyForm) {

    m_lastChosenCode.clear();
    m_lastChosenDirectionName.clear();
    m_lastChosenStudyType.clear();
    m_lastChosenStudyForm.clear();
    m_lastChosenFacultyName.clear();

    m_lastChosenCode = lastChosenCode;
    m_lastChosenDirectionName = lastChosenDirectionName;
    m_lastChosenStudyType = lastChosenStudyType;
    m_lastChosenStudyForm = lastChosenStudyForm;
}

void StatsPageModel::setLastChosenFacultyInfo(const QString & lastChosenFacultyName) {

    m_lastChosenCode.clear();
    m_lastChosenDirectionName.clear();
    m_lastChosenStudyType.clear();
    m_lastChosenStudyForm.clear();
    m_lastChosenFacultyName.clear();

    m_lastChosenFacultyName = lastChosenFacultyName;
}

void StatsPageModel::rebuildDirectionStats() {

    m_directionsStats.clear();
    if (!m_facultiesList)
	return;

    m_directionsStats.reserve(m_facultiesList->size());

    for (int i = 0; i < m_facultiesList->size(); ++i) {
	const FacultyDirection& dir = m_facultiesList->at(i);
	const auto& pool = dir.pool();

	DirectionStats d;
	d.name = dir.name();
	d.code = dir.code();
	d.facultyName = dir.division();
	d.form = dir.studyForm();
	d.type = dir.studyType();
	d.capacity = dir.capacity();
	d.size = pool.size();
	d.sourceIndex = i;

	if (!pool.isEmpty()) {
	    int maxV = std::numeric_limits<int>::min();
	    int minV = std::numeric_limits<int>::max();
	    long sum = 0;

	    for (const auto& p : pool) {
		const int score = p.first.egeScore();
		maxV = std::max(maxV, score);
		minV = std::min(minV, score);
		sum += score;

		const int bucket
		    = (score - StatsConfig::kMinScoreBucket) / StatsConfig::kBucketStep;
		if (bucket >= 0 && bucket < StatsConfig::kBucketCount)
		    ++d.scoreDistribution[bucket];
	    }

	    d.maxSumScore = maxV;
	    d.minSumScore = minV;
	    d.midSumScore = pool[pool.size()/2].first.egeScore();
	    d.meanSumScore = static_cast<double>(sum) / pool.size();
	}

	m_directionsStats.append(std::move(d));
    }
}

void StatsPageModel::rebuildFacultyStats() {

    m_facultiesStats.clear();
    if (m_directionsStats.isEmpty())
	return;

    QHash<QString, int> facultyIndex;

    for (int i = 0; i < m_directionsStats.size(); ++i) {
	const DirectionStats& d = m_directionsStats[i];

	auto it = facultyIndex.find(d.facultyName);
	if (it == facultyIndex.end()) {
	    FacultyStats f;
	    f.facultyName = d.facultyName;
	    m_facultiesStats.append(std::move(f));
	    it = facultyIndex.insert(d.facultyName, m_facultiesStats.size() - 1);
	}

	FacultyStats& f = m_facultiesStats[*it];
	f.directionIndices.append(i);
	f.totalSize += d.size;
	f.totalCapacity += d.capacity;

	if (d.maxSumScore)
	    f.maxSumScore
	        = f.maxSumScore ? std::max(*f.maxSumScore, *d.maxSumScore) : d.maxSumScore;
	if (d.minSumScore)
	    f.minSumScore
	        = f.minSumScore ? std::min(*f.minSumScore, *d.minSumScore) : d.minSumScore;
    }

    for (auto& f : m_facultiesStats) {
	long sum = 0;
	int total = 0;
	for (int idx : std::as_const(f.directionIndices)) {
	    const auto& d = m_directionsStats[idx];
	    if (d.meanSumScore) {
		sum += static_cast<long>(*d.meanSumScore * d.size);
		total += d.size;
	    }
	}
	if (total > 0)
	    f.meanSumScore = static_cast<double>(sum) / total;
    }
}

void StatsPageModel::gettingReadyPrevPage() {

    if (!m_lastChosenFacultyName.isEmpty()) {
        emit loadFacultyPage(m_lastChosenFacultyName);
        return;
    }

    if (m_lastChosenCode.isEmpty() or m_lastChosenDirectionName.isEmpty() or
        m_lastChosenStudyType.isEmpty() or m_lastChosenStudyForm.isEmpty()) {
        emit nothingToLoad();
        return;
    }

    StudyForm form = StudyForm::Error;
    StudyType type = StudyType::Error;

    if (m_lastChosenStudyForm.contains("Очное") or m_lastChosenStudyForm.contains("Очная")) {
        form = StudyForm::Personal;
    }
    else if (m_lastChosenStudyForm.contains("Заочная") or m_lastChosenStudyForm.contains("Заочное")) {
        form = StudyForm::NotPersonal;
    }
    else if (m_lastChosenStudyForm.contains("Очно-заочная") or m_lastChosenStudyForm.contains("Очно-заочное")) {
        form = StudyForm::PersonalNotPersonal;
    }

    if (m_lastChosenStudyType.contains("Бюджет")) {
        type = StudyType::Budget;
    }
    else if (m_lastChosenStudyType.contains("Отдельная квота") or m_lastChosenStudyType.contains("Отдельное квота")) {
        type = StudyType::Kvot;
    }
    else if (m_lastChosenStudyType.contains("Особое право") or m_lastChosenStudyType.contains("Особая право")) {
        type = StudyType::SpecialRight;
    }
    else if (m_lastChosenStudyType.contains("Внебюджет")) {
        type = StudyType::NonBudget;
    }
    else if (m_lastChosenStudyType.contains("Целевое") or m_lastChosenStudyType.contains("Целевая")) {
        type = StudyType::CompanySponsor;
    }

    for (int i = 0; i < m_directionsStats.size(); ++i) {
        if (m_directionsStats[i].name == m_lastChosenDirectionName and
            m_directionsStats[i].code == m_lastChosenCode and
            m_directionsStats[i].type == type and
            m_directionsStats[i].form == form) {
            emit loadDirectionPage(i);
            return;
        }
    }

    emit nothingToLoad();
}

QMap<QString, QVariant> StatsPageModel::directionStatsAt(int index) const {

    if (index < 0 || index >= m_directionsStats.size())
        return {};

    const auto& d = m_directionsStats[index];

    m_applicantsListModel->setApplicantsList((*m_facultiesList)[d.sourceIndex].pool());

    QMap<QString, QVariant> m;
    m["name"]        = d.name;
    m["code"]        = d.code;
    m["facultyName"] = d.facultyName;
    m["size"]        = d.size;
    m["capacity"]    = d.capacity;
    if (d.maxSumScore)  m["maxScore"]  = *d.maxSumScore;
    if (d.minSumScore)  m["minScore"]  = *d.minSumScore;
    if (d.meanSumScore) m["meanScore"] = *d.meanSumScore;
    if (d.meanSumScore) m["midScore"] = *d.midSumScore;

    switch (d.form) {
        case StudyForm::Personal:
            m["studyForm"] = "Очная";
            break;
        case StudyForm::NotPersonal:
            m["studyForm"] = "Заочная";
            break;
        case StudyForm::PersonalNotPersonal:
            m["studyForm"] = "Очно-заочная";
            break;
        case StudyForm::Error:
            m["studyForm"] = "ОШИБКА ФОРМЫ ОБУЧЕНИЯ";
            break;
    }

    switch (d.type) {
        case StudyType::Budget:
            m["studyType"] = "Бюджет";
            break;
        case StudyType::CompanySponsor:
            m["studyType"] = "Целевое";
            break;
        case StudyType::Kvot:
            m["studyType"] = "Отдельная квота";
            break;
        case StudyType::SpecialRight:
            m["studyType"] = "Особое право";
            break;
        case StudyType::NonBudget:
            m["studyType"] = "ОШИБКА ТИПА ОБУЧЕНИЯ (ЗДЕСЬ ПЛАТНОГО БЫТЬ НЕ ДОЛЖНО)";
            break;
        case StudyType::Error:
            m["studyType"] = "ОШИБКА ТИПА ОБУЧЕНИЯ";
            break;
    }

    QList<QVariant> dist;
    for (int v : d.scoreDistribution)
        dist.append(v);

    m["scoreDistribution"] = dist;
    m["scoreBucketStart"]   = StatsConfig::kMinScoreBucket;
    m["scoreBucketStep"]    = StatsConfig::kBucketStep;
    return m;

}

QMap<QString, QVariant> StatsPageModel::facultyStats(const QString& name) const {

    for (const auto& f : m_facultiesStats) {

        if (f.facultyName != name)
            continue;

        QMap<QString, QVariant> m;
        m["facultyName"]    = f.facultyName;
        m["totalSize"]      = f.totalSize;
        m["totalCapacity"]  = f.totalCapacity;
        m["directionCount"] = f.directionIndices.size();
        if (f.maxSumScore)  m["maxScore"]  = *f.maxSumScore;
        if (f.minSumScore)  m["minScore"]  = *f.minSumScore;
        if (f.meanSumScore) m["meanScore"] = *f.meanSumScore;
        return m;
    }

    return {};
}

std::shared_ptr<ApplicantsListModel> StatsPageModel::getApplicantsListModel() const {
    return m_applicantsListModel;
}