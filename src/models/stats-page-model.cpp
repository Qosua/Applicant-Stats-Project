#include "stats-page-model.h"

StatsPageModel::StatsPageModel() {
    m_applicantsListModel = std::make_shared<ApplicantsListModel>(this);
}

void StatsPageModel::setFaculties(std::shared_ptr<QList<FacultyDirection>> data) {

    m_source = std::move(data);
    rebuildDirectionStats();
    rebuildFacultyStats();
}

std::shared_ptr<ApplicantsListModel> StatsPageModel::getApplicantsListModel() {
    return m_applicantsListModel;
}

void StatsPageModel::rebuildDirectionStats() {

    m_directions.clear();
    if (!m_source)
	return;

    m_directions.reserve(m_source->size());

    for (int i = 0; i < m_source->size(); ++i) {
	const FacultyDirection& dir = m_source->at(i);
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

	m_directions.append(std::move(d));
    }
}

void StatsPageModel::rebuildFacultyStats() {

    m_faculties.clear();
    if (m_directions.isEmpty())
	return;

    QHash<QString, int> facultyIndex;

    for (int i = 0; i < m_directions.size(); ++i) {
	const DirectionStats& d = m_directions[i];

	auto it = facultyIndex.find(d.facultyName);
	if (it == facultyIndex.end()) {
	    FacultyStats f;
	    f.facultyName = d.facultyName;
	    m_faculties.append(std::move(f));
	    it = facultyIndex.insert(d.facultyName, m_faculties.size() - 1);
	}

	FacultyStats& f = m_faculties[*it];
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

    // Средний по факультету — взвешенно по числу зачисленных
    for (auto& f : m_faculties) {
	long sum = 0;
	int total = 0;
	for (int idx : std::as_const(f.directionIndices)) {
	    const auto& d = m_directions[idx];
	    if (d.meanSumScore) {
		sum += static_cast<long>(*d.meanSumScore * d.size);
		total += d.size;
	    }
	}
	if (total > 0)
	    f.meanSumScore = static_cast<double>(sum) / total;
    }
}

QMap<QString, QVariant> StatsPageModel::directionStatsAt(int index) const {

    if (index < 0 || index >= m_directions.size())
        return {};

    const auto& d = m_directions[index];

    m_applicantsListModel->setApplicantsList((*m_source)[d.sourceIndex].pool());

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

    for (const auto& f : m_faculties) {

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