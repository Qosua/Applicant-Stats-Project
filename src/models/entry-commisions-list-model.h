#ifndef APPLICANTSTATSPROJECT_KCPMODELLIST_H
#define APPLICANTSTATSPROJECT_KCPMODELLIST_H

#include <QAbstractListModel>
#include <QList>
#include <QVector>

#include "../utils/database-manager.h"

class EntryCommisionsListModel : public QAbstractListModel {
    Q_OBJECT
public:
   explicit EntryCommisionsListModel(QObject *parent = nullptr);

    enum EntryCommisionsListModelRoles {
        entryCommisionNameRole = Qt::UserRole + 1,
        entryCommisionYearRole,
        entryCommisionIsBachelorRole,
        entryCommisionNameYearIsBachelorRole,

    };

    struct EntryCommisionData {
        QString name;
        bool isBachelor;
        int year;
    };


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<EntryCommisionData> m_data;

};

#endif  // APPLICANTSTATSPROJECT_KCPMODELLIST_H
