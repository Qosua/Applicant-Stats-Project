import QtQuick
import QtQuick.Layouts
import QtGraphs
import QtQuick.Controls
import ApplicantStatsProjectModule

Rectangle {
    id: statusBar

    height: 30
    color: "#26282b"

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.leftMargin: 15
    anchors.rightMargin: 15
    anchors.topMargin: 5

    RowLayout {
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.fill: parent
        spacing: 50

        Text {
            property var arr: qmlHelper.currentTablePath.split("/")
            id: pathText

            Layout.preferredWidth: parent.width/5

            text: (arr.length && arr[arr.length - 1] !== ""
                ? "Таблица: " + arr[arr.length - 1]
                : "Файл не выбран")
            font.pointSize: 11
            color: "#cccccc"
            elide: Text.ElideMiddle
        }

        Item {
            Layout.fillWidth: true
        }

        Rectangle {
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width/5
            color: "transparent"


            RowLayout {
                anchors.fill: parent
                spacing: 15
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: parent.height / 3.8
                    Layout.alignment: Qt.AlignVCenter
                    radius: parent.height / 2

                    color: "#3c3c3c"
                    Rectangle {
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom

                        height: parent.height
                        width: parent.width * qmlHelper.currentLoadBarPart

                        radius: parent.height
                        color: "#5073C1"

                    }
                }
                Text {
                    Layout.alignment: Qt.AlignVCenter
                    text: "Процесс"
                    font.pointSize: 11
                    color: "#bbbbbb"
                }
            }
        }

        Item {
            Layout.fillWidth: true
        }

        Text {
            Layout.preferredWidth: parent.width/5

            horizontalAlignment: Text.AlignRight

            text: "Версия: " + qmlHelper.appVersion
            font.pointSize: 11
            color: "#cccccc"

        }
    }

}