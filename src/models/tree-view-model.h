#ifndef APPLICANTSTATSPROJECT_TREEVIEWMODEL_H
#define APPLICANTSTATSPROJECT_TREEVIEWMODEL_H

#include <QAbstractItemModel>
#include <memory>

#include "../applicants-faculty-data/faculty-direction.h"

class TreeViewModel : public QAbstractItemModel {
    Q_OBJECT

public:
    enum Roles {
	DisplayRole = Qt::DisplayRole,
	NameRole = Qt::UserRole + 1,
	CodeRole,
	CapacityRole,
	PoolSizeRole,
	StudyFormRole,
	StudyTypeRole,
	IsDivisionRole,
	DepthRole,
	SourceIndexRole,
    };
    Q_ENUM(Roles)

    explicit TreeViewModel(QObject *parent = nullptr);
    ~TreeViewModel() override;

    void setFaculties(std::shared_ptr<QList<FacultyDirection>> faculties);

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    struct TreeNode {
	QString display;
	bool isDivision = false;
	int sourceIndex = -1;
	TreeNode *parent = nullptr;
	QList<TreeNode *> children;

	~TreeNode() { qDeleteAll(children); }
    };

    void buildTree();

    TreeNode *nodeFromIndex(const QModelIndex &index) const;

    TreeNode *m_rootNode = nullptr;
    std::shared_ptr<QList<FacultyDirection>> m_faculties;
};

#endif  // APPLICANTSTATSPROJECT_TREEVIEWMODEL_H
