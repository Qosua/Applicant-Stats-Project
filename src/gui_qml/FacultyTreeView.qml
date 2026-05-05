import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

TreeView {
    id: treeView
    model: treeViewModel
    clip: true

    signal directionSelected(int sourceIndex)

    signal facultySelected(string facultyName)

    signal selectionCleared()

    selectionModel: ItemSelectionModel {
    }

    rowSpacing: 3
    boundsBehavior: Flickable.StopAtBounds

    delegate: TreeViewDelegate {
        id: treeDelegate

        onClicked: {
            if (model.isDivision) {
                console.log("Faculty - " + model.display)
                treeView.facultySelected(model.display)
            } else {
                console.log("Direction - " + model.display)
                treeView.directionSelected(model.sourceIndex)
            }
        }

        implicitWidth: treeView.width

        topPadding: 5
        bottomPadding: 5
        leftPadding: 15 + depth * 20
        implicitHeight: 25
        indentation: 20

        indicator: Item {
            width: 16;
            height: 16

            Label {
                anchors.centerIn: parent
                text: treeDelegate.expanded ? "-" : "+"
                font.bold: true
                color: "#607ab5"
                font.pixelSize: 18
            }
        }

        contentItem: RowLayout {
            spacing: 10

            Label {
                visible: !model.isDivision
                text: model.code ?? "ОШИБКА КОДА НАПРАВЛЕНИЯ"
                font.pixelSize: 14
                color: "#888888"
                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
            }
            Label {
                text: model.display
                color: "#CCCCCC"
                font.pixelSize: model.isDivision ? 16 : 14
                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
            }
            Label {
                visible: (!model.isDivision && model.studyForm !== "")
                text: model.studyForm ?? "ОШИБКА ФОРМЫ ОБУЧЕНИЯ"
                font.pixelSize: 14
                color: "#6c758a"
                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
            }
            Label {
                visible: !model.isDivision
                text: model.studyType ?? "ОШИБКА ВИДА ОБУЧЕНИЯ"
                font.pixelSize: 14
                color: "#87a586"
                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
            }
            // Label {
            //     visible: !model.isDivision
            //     text: (model.poolSize ?? 0) + " / " + (model.capacity ?? 0)
            //     font.pixelSize: 14
            //     color: {
            //         if (model.isDivision)
            //             return "#888888"
            //         if (model.capacity === 0)
            //             return "#27ae60"
            //         let ratio = (model.poolSize ?? 0) / Math.max(model.capacity ?? 1, 1)
            //         return ratio >= 1.0 ? "#27ae60" : ratio >= 0.7 ? "#f39c12" : "#e74c3c"
            //     }
            //     verticalAlignment: Text.AlignVCenter
            //     Layout.fillHeight: true
            // }
            Item {
                Layout.fillWidth: true
            }
        }

        background: Rectangle {
            radius: 8
            implicitWidth: treeView.width
            color: treeDelegate.current ? "#3a4660"
                : treeDelegate.hovered ? "#393b40"
                    : "transparent"
        }
    }
}

