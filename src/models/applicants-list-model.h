#ifndef APPLICANTSTATSPROJECT_APPLICANTSLISTMODEL_H
#define APPLICANTSTATSPROJECT_APPLICANTSLISTMODEL_H

#include <QAbstractListModel>
#include <QDir>
#include <QFileSystemWatcher>
#include <QList>
#include <QString>
#include <memory>

#include "../applicants-faculty-data/applicant.h"
#include "../utils/support-system.h"

class ApplicantsListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum ApplicantsListRoles {
        applicantNameRole = Qt::UserRole + 1,
        idRole,
	    scoreRole,
	    phoneRole,
	    emailRole,
    };

    explicit ApplicantsListModel(QObject *parent = nullptr);
    ~ApplicantsListModel() override;

    void setApplicantsList(const QList<QPair<PriorityInfo, Applicant>> &data);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<QPair<PriorityInfo, Applicant>>  applicantsList;

};

#endif  // APPLICANTSTATSPROJECT_APPLICANTSLISTMODEL_H
