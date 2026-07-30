import QtQuick
import QtQuick.Layouts
import QtGraphs
import QtQuick.Controls
import ApplicantStatsProjectModule

Rectangle {
    id: placeHolder
    color: "transparent"

    property var payload: null

    Rectangle {
        anchors.centerIn: parent
        width: 340; height: 340
        radius: width / 2
        color: "transparent"
        border.color: "#6e8cd1"
        border.width: 1
        opacity: 0.16

        SequentialAnimation on scale {
            loops: Animation.Infinite
            NumberAnimation {
                from: 0.94;
                to: 1.06; duration: 2600; easing.type: Easing.InOutSine
            }
            NumberAnimation {
                from: 1.06;
                to: 0.94; duration: 2600; easing.type: Easing.InOutSine
            }
        }
    }

    Rectangle {
        anchors.centerIn: parent
        width: 230; height: 230
        radius: width / 2
        color: "transparent"
        border.color: "#6e8cd1"
        border.width: 1
        opacity: 0.32

        SequentialAnimation on scale {
            loops: Animation.Infinite
            NumberAnimation {
                from: 1.06;
                to: 0.94; duration: 2200; easing.type: Easing.InOutSine
            }
            NumberAnimation {
                from: 0.94;
                to: 1.06; duration: 2200; easing.type: Easing.InOutSine
            }
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 14

        Rectangle {
            Layout.preferredWidth: 100
            Layout.preferredHeight: 100
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 10
            radius: 22
            color: "#1f2125"
            border.color: "#3a4660"
            border.width: 1

            Item {
                width: 58; height: 50
                anchors.centerIn: parent

                Rectangle {
                    x: 0; width: 10; height: 20; radius: 2
                    color: "#3f5587"
                    anchors.bottom: parent.bottom
                }
                Rectangle {
                    x: 16; width: 10; height: 34; radius: 2
                    color: "#556da3"
                    anchors.bottom: parent.bottom
                }
                Rectangle {
                    x: 32; width: 10; height: 26; radius: 2
                    color: "#6781b8"
                    anchors.bottom: parent.bottom
                }
                Rectangle {
                    x: 48; width: 10; height: 44; radius: 2
                    color: "#8a9fcc"
                    anchors.bottom: parent.bottom
                }
            }
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Выберите таблицу и направление/факультет"
            color: "#d4d8e0"
            font.pixelSize: 22
            font.weight: Font.Medium
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "для начала работы со статистикой"
            color: "#7a8090"
            font.pixelSize: 19
        }

        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 90
            Layout.preferredHeight: 1
            Layout.topMargin: 10
            color: "transparent"
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop {
                    position: 0.0; color: "transparent"
                }
                GradientStop {
                    position: 0.5; color: "#556da3"
                }
                GradientStop {
                    position: 1.0; color: "transparent"
                }
            }
        }
        Text {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 4
            text: "Список доступных таблиц - слева"
            color: "#555a64"
            font.pixelSize: 16
        }
    }
}