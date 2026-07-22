import QtQuick
import QtQuick.Layouts
import QtGraphs
import QtQuick.Controls
import ApplicantStatsProjectModule

Rectangle {

    id: tablesList
    property bool collapsed: false
    property bool dropped: dropArea.containsDrag
    readonly property int collapsedHeight: buttonsBar.height + 1

    DropArea {
        id: dropArea
        anchors.fill: parent

        keys: ["text/uri-list"]

        onEntered: (drag) => {
            drag.accept(Qt.CopyAction)
        }

        onDropped: (drop) => {
            if (drop.hasUrls) {
                for (let i = 0; i < drop.urls.length; i++) {
                    let path = drop.urls[i].toString()
                    path = path.replace(/^file:\/\/\//, "")

                    qmlHelper.addNewTable(path)
                }
            }
        }
    }

    Rectangle {
        id: buttonsBar
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left

        height: 38
        color: "transparent"

        property bool sortFlag: false

        RowLayout {
            anchors.fill: parent
            anchors.margins: 3
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            spacing: 7

            PageButton {
                Layout.preferredHeight: parent.height
                Layout.preferredWidth: parent.height + 7

                btnIconSource: !tablesList.collapsed ? "qrc:/resources/icons/angle-down.png" : "qrc:/resources/icons/angle-up.png"
                btnIconColor: "#aeaeae"
                btnToolTipName: tablesList.collapsed ? " Развернуть " : " Свернуть "
                btnToolTipDelay: 700
                iconSize: 20

                onClicked: {
                    tablesList.collapsed = !tablesList.collapsed
                }

            }
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {
                        tablesList.collapsed = !tablesList.collapsed
                    }
                }
            }
            PageButton {
                Layout.preferredHeight: parent.height
                Layout.preferredWidth: parent.height + 7

                btnIconSource: "qrc:/resources/icons/plus.png"
                btnIconColor: "#aeaeae"
                btnToolTipName: " Добавить таблицу "
                btnToolTipDelay: 700
                iconSize: 20

                onClicked: {
                    qmlHelper.addNewTableFromExploler();
                }

            }
            PageButton {
                Layout.preferredHeight: parent.height
                Layout.preferredWidth: parent.height + 7

                btnIconSource: "qrc:/resources/icons/folder.png"
                btnIconColor: "#aeaeae"
                btnToolTipName: " Открыть папку хранения таблиц "
                btnToolTipDelay: 700
                iconSize: 20

                onClicked: {
                    qmlHelper.openAppCacheFolder();
                }

            }
            PageButton {

                property bool sortFlag: true

                Layout.preferredHeight: parent.height
                Layout.preferredWidth: parent.height + 7

                btnIconSource: "qrc:/resources/icons/sort-alpha-down.png"
                btnIconColor: "#aeaeae"
                btnToolTipName: " Сортировать по имени "
                btnToolTipDelay: 700
                iconSize: 20

                onClicked: {
                    if (sortFlag) {
                        sortFilterProxyModel.sort(0, Qt.DescendingOrder)
                        sortFlag = false
                    } else {
                        sortFilterProxyModel.sort(0, Qt.AscendingOrder)
                        sortFlag = true
                    }
                }

            }

        }
    }

    Rectangle {
        id: someKindOfBorder
        visible: !tablesList.collapsed
        anchors.top: buttonsBar.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: 1
        anchors.rightMargin: 1
        color: "#2e2e2e"
        height: 1
    }

    Rectangle {

        id: viewRect
        visible: !tablesList.collapsed
        anchors.top: someKindOfBorder.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.margins: 10
        color: "transparent"

        ListView {

            id: listView
            model: sortFilterProxyModel
            spacing: 5
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            anchors.fill: parent
            anchors.margins: 4

            delegate: Rectangle {

                id: delegat

                readonly property bool selected: qmlHelper.currentTablePath === tableFullPath

                implicitWidth: viewRect.width - 10
                implicitHeight: 60
                radius: 10

                color: {
                    if (selected)
                        return "#2d374e"
                    if (delegatHover.hovered)
                        return "#3f4247"
                    return "#2c2e32"
                }
                Behavior on color {
                    ColorAnimation {
                        duration: 100
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        qmlHelper.currentTablePath = tableFullPath
                        qmlHelper.sendSignalToProceedTable(tableName, customComboBox.currentValue)
                    }
                    HoverHandler {
                        id: delegatHover
                        cursorShape: Qt.PointingHandCursor
                    }
                }

                Image {
                    id: icon
                    source: model.tableIconPath
                    height: 20
                    width: 20
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.margins: 5
                    anchors.leftMargin: 8
                }
                Text {
                    id: tableLastChangeDateText
                    text: "(" + model.tableLastChangeDate + ")"
                    elide: Text.ElideRight
                    font.pointSize: 11
                    color: "#999999"
                    anchors.top: parent.top
                    anchors.left: icon.right
                    anchors.margins: 5
                }
                Text {
                    id: tableNameText
                    text: model.tableName
                    elide: Text.ElideRight
                    font.pointSize: 11
                    color: "#eeeeee"
                    anchors.top: parent.top
                    anchors.left: tableLastChangeDateText.right
                    anchors.right: updateButton.left
                    anchors.margins: 5
                }
                CustomComboBox {

                    id: customComboBox

                    implicitHeight: 27
                    comboBoxModel: entryCommisionsListModel

                    Component.onCompleted: {
                        currentIndex = (entryCommisionId ? (entryCommisionId != -1 ? entryCommisionId : 0) : 0)
                    }

                    onCurrentIndexChanged: {
                        tablesListModel.setNewCurrentIndex(indexInArray, currentIndex)
                    }

                    anchors.left: parent.left
                    anchors.right: updateButton.left
                    anchors.top: icon.bottom
                    anchors.margins: 5
                }
                PageButton {

                    id: updateButton

                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.margins: 10

                    implicitWidth: parent.height - 20

                    btnIconSource: "qrc:/resources/icons/update-60.png"
                    btnIconColor: "#aeaeae"
                    btnToolTipName: "обновить"
                    btnToolTipDelay: 700
                    iconSize: 24

                    onClicked: {
                        qmlHelper.currentTablePath = tableFullPath
                        qmlHelper.sendSignalToProceedTable(tableName, customComboBox.currentValue)
                    }

                }


            }
        }
    }

    Item {
        id: emptyPlaceholder
        anchors.fill: parent
        visible: listView.count === 0

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 10

            PageButton {
                id: placeHolderBtn
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 38
                Layout.preferredWidth: 38

                btnIconSource: "qrc:/resources/icons/plus.png"
                btnIconColor: "#aeaeae"
                btnToolTipDelay: 700
                iconSize: 38

                onClicked: {
                    qmlHelper.addNewTableFromExploler();
                }
                opacity: tablesList.dropped ? 1.0 : 0.6

                Behavior on opacity {
                    NumberAnimation {
                        duration: 120
                    }
                }
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
                text: tablesList.dropped
                    ? "Отпустите, чтобы добавить"
                    : "Перетащите таблицу сюда\nили нажмите +"
                color: tablesList.dropped ? "#aeaeae" : "#6c6e72"
                font.pointSize: 14
                lineHeight: 1.2
            }
        }
    }

}