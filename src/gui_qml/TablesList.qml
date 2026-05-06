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

        // Фильтр — принимаем только файлы
        keys: ["text/uri-list"]

        onEntered: (drag) => {
            drag.accept(Qt.CopyAction)
        }

        onDropped: (drop) => {
            if (drop.hasUrls) {
                for (let i = 0; i < drop.urls.length; i++) {
                    let path = drop.urls[i].toString()
                    path = path.replace(/^file:\/\/\//, "")

                    qmlHelper.copyFileToAppdata(path)
                }
            }
        }
    }

    Rectangle {
        id: buttonsBar
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left

        height: 36
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
                Layout.preferredWidth: parent.height

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
                Layout.preferredWidth: parent.height

                btnIconSource: "qrc:/resources/icons/plus.png"
                btnIconColor: "#aeaeae"
                btnToolTipName: " Добавить таблицу "
                btnToolTipDelay: 700
                iconSize: 20

                onClicked: {
                    qmlHelper.addTableFromExploler();
                }

            }
            PageButton {
                Layout.preferredHeight: parent.height
                Layout.preferredWidth: parent.height

                btnIconSource: "qrc:/resources/icons/folder.png"
                btnIconColor: "#aeaeae"
                btnToolTipName: " Открыть папку хранения таблиц "
                btnToolTipDelay: 700
                iconSize: 20

                onClicked: {
                    qmlHelper.openAppDataFolder();
                }

            }
            PageButton {

                property bool sortFlag: true

                Layout.preferredHeight: parent.height
                Layout.preferredWidth: parent.height

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
            spacing: 2
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            anchors.fill: parent
            anchors.margins: 4

            delegate: Rectangle {

                readonly property bool selected: qmlHelper.currentTablePath === tableFullPath

                width: viewRect.width - 10
                height: 30
                radius: 8

                color: {
                    if (selected)
                        return "#3a4660"
                    if (delegatHover.hovered)
                        return "#3f4247"
                    return "#2c2e32"
                }
                Behavior on color {
                    ColorAnimation { duration: 100 }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        qmlHelper.currentTablePath = tableFullPath
                        qmlHelper.sendSignalToProceedTable(tableName)
                    }
                }
                HoverHandler {
                    id: delegatHover
                    cursorShape: Qt.PointingHandCursor
                }
                Text {
                    id: tableLastChangeDateText
                    text: "(" + tableLastChangeDate + ")"
                    font.pointSize: 9
                    color: "#999999"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.margins: 4
                }
                Image {
                    id: icon
                    source: tableIconPath
                    height: parent.height - 10
                    width: parent.height - 10
                    //fillMode: Image.PreserveAspectFit
                    anchors.left: tableLastChangeDateText.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 6
                }
                Text {
                    id: tableNameText
                    text: tableName
                    font.pointSize: 10
                    color: "#eeeeee"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: icon.right
                    anchors.leftMargin: 4
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
                    qmlHelper.addTableFromExploler();
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