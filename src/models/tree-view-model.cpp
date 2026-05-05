#include "tree-view-model.h"

TreeViewModel::TreeViewModel(QObject *parent)
    : QAbstractItemModel(parent), m_rootNode(new TreeNode) {}

TreeViewModel::~TreeViewModel() { delete m_rootNode; }

void TreeViewModel::setFaculties(std::shared_ptr<QList<FacultyDirection>> faculties) {

    beginResetModel();
    m_faculties = faculties;
    buildTree();
    endResetModel();
}

void TreeViewModel::buildTree() {

    delete m_rootNode;
    m_rootNode = new TreeNode;

    if (!m_faculties)
	return;

    QList<QString> divisionOrder;
    QHash<QString, QList<int>> divisionMap;

    for (int i = 0; i < m_faculties->size(); ++i) {

	const QString &div = m_faculties->at(i).division();

	if (!divisionMap.contains(div))
	    divisionOrder.append(div);

	divisionMap[div].append(i);
    }

    for (const QString &div : std::as_const(divisionOrder)) {

	auto *divNode = new TreeNode;
	divNode->display = div;
	divNode->isDivision = true;
	divNode->parent = m_rootNode;
	m_rootNode->children.append(divNode);

	for (int srcIdx : std::as_const(divisionMap[div])) {

	    const FacultyDirection &fd = m_faculties->at(srcIdx);

	    auto *dirNode = new TreeNode;
	    dirNode->display = fd.name();
	    dirNode->isDivision = false;
	    dirNode->sourceIndex = srcIdx;
	    dirNode->parent = divNode;
	    divNode->children.append(dirNode);
	}
    }
}

TreeViewModel::TreeNode *TreeViewModel::nodeFromIndex(const QModelIndex &index) const {

    if (!index.isValid())
	return m_rootNode;

    return static_cast<TreeNode *>(index.internalPointer());
}

QModelIndex TreeViewModel::index(int row, int column, const QModelIndex &parent) const {

    if (column != 0)
	return {};

    TreeNode *parentNode = nodeFromIndex(parent);

    if (!parentNode || row < 0 || row >= parentNode->children.size())
	return {};

    return createIndex(row, 0, parentNode->children.at(row));
}

QModelIndex TreeViewModel::parent(const QModelIndex &child) const {

    TreeNode *node = nodeFromIndex(child);

    if (!node || node == m_rootNode)
	return {};

    TreeNode *parentNode = node->parent;

    if (!parentNode || parentNode == m_rootNode)
	return {};

    TreeNode *grandParent = parentNode->parent;
    int row = grandParent->children.indexOf(parentNode);

    return createIndex(row, 0, parentNode);
}

int TreeViewModel::rowCount(const QModelIndex &parent) const {

    TreeNode *node = nodeFromIndex(parent);
    return node ? node->children.size() : 0;
}

int TreeViewModel::columnCount(const QModelIndex &parent) const { return 1; }

QVariant TreeViewModel::data(const QModelIndex &index, int role) const {

    TreeNode *node = nodeFromIndex(index);

    if (!node || node == m_rootNode)
	return {};

    switch (role) {

	case Qt::DisplayRole:
	case NameRole:
	    return node->display;

	case IsDivisionRole:
	    return node->isDivision;

	case DepthRole:
	    return node->isDivision ? 0 : 1;

	case CodeRole:
	    if (node->isDivision || !m_faculties)
		return {};
	    return m_faculties->at(node->sourceIndex).code();

	case CapacityRole:
	    if (node->isDivision || !m_faculties)
		return {};
	    if (m_faculties->at(node->sourceIndex).studyType() == StudyType::Budget)
		return m_faculties->at(node->sourceIndex).capacity();
	    else
		return m_faculties->at(node->sourceIndex).pool().size();

	case PoolSizeRole:
	    if (node->isDivision || !m_faculties)
		return {};
	    return m_faculties->at(node->sourceIndex).pool().size();

	case StudyFormRole:
	    if (node->isDivision || !m_faculties)
		return {};
	    switch (m_faculties->at(node->sourceIndex).studyForm()) {
		case StudyForm::Personal:
		    return QStringLiteral("");
		case StudyForm::NotPersonal:
		    return QStringLiteral("Заочная");
		case StudyForm::PersonalNotPersonal:
		    return QStringLiteral("Очно-заочная");
		default:
		    return QStringLiteral("ОШИБКА ФОРМЫ ОБУЧЕНИЯ");
	    }

	case StudyTypeRole:
	    if (node->isDivision || !m_faculties)
		return {};
	    switch (m_faculties->at(node->sourceIndex).studyType()) {
		case StudyType::Budget:
		    return QStringLiteral("Бюджет");
		case StudyType::NonBudget:
		    return QStringLiteral("Внебюджет");
		case StudyType::Kvot:
		    return QStringLiteral("Отдельная квота");
		case StudyType::CompanySponsor:
		    return QStringLiteral("Целевое");
		case StudyType::SpecialRight:
		    return QStringLiteral("Особое право");
		default:
		    return QStringLiteral("ОШИБКА ТИПА ОБУЧЕНИЯ");
	    }
	case SourceIndexRole:
	    if (node->isDivision)
		return -1;
	    return node->sourceIndex;
	default:
	    return {};
    }
}

QHash<int, QByteArray> TreeViewModel::roleNames() const {

    return {
        {Qt::DisplayRole, "display"}, {NameRole, "name"},
        {CodeRole, "code"},           {CapacityRole, "capacity"},
        {PoolSizeRole, "poolSize"},   {StudyFormRole, "studyForm"},
        {StudyTypeRole, "studyType"}, {IsDivisionRole, "isDivision"},
        {DepthRole, "depth"},         {SourceIndexRole, "sourceIndex"},
    };
}
