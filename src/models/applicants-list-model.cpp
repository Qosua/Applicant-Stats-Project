#include "applicants-list-model.h"

ApplicantsListModel::ApplicantsListModel(QObject* parent) : QAbstractListModel(parent) {}

ApplicantsListModel::~ApplicantsListModel() {}

void ApplicantsListModel::setApplicantsList(const QList<QPair<PriorityInfo, Applicant>> &data) {

    if (!this->applicantsList.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0,  this->applicantsList.size() - 1);
         this->applicantsList.clear();
        endRemoveRows();
    }

    beginResetModel();
    this->applicantsList = data;
    endResetModel();
}

int ApplicantsListModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent)
    return this->applicantsList.size();
}

QVariant ApplicantsListModel::data(const QModelIndex& index, int role) const {

    if (index.row() < 0 or index.row() >= this->applicantsList.count())
        return QVariant();

    const Applicant &applicant = this->applicantsList[index.row()].second;
    const PriorityInfo &priorityInfo = this->applicantsList[index.row()].first;

    switch (role) {
        case ApplicantsListRoles::applicantNameRole:
            return applicant.FIO();
            break;
        case ApplicantsListRoles::idRole:
            return applicant.id();
            break;
        case ApplicantsListRoles::emailRole:
            return applicant.email();
            break;
        case ApplicantsListRoles::phoneRole:
            return applicant.phoneNumber();
            break;
        case ApplicantsListRoles::scoreRole:
            return priorityInfo.egeScore();
            break;
        default:
            return QVariant();
            break;
    }
}

QHash<int, QByteArray> ApplicantsListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ApplicantsListRoles::applicantNameRole] = "applicantName";
    roles[ApplicantsListRoles::idRole] = "applicantId";
    roles[ApplicantsListRoles::emailRole] = "applicantEmail";
    roles[ApplicantsListRoles::phoneRole] = "applicantPhone";
    roles[ApplicantsListRoles::scoreRole] = "applicantScore";
    return roles;
}


