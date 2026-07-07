import QtQuick
import QtQuick.Layouts
import QtGraphs
import QtQuick.Controls
import ApplicantStatsProjectModule

Rectangle {
    id: pagesMenu
    width: 48
    radius: 12

    anchors.top: parent.top
    anchors.bottom: statusBar.top
    anchors.left: parent.left
    anchors.margins: 10
    anchors.bottomMargin: 0

    border.color: "#323232"
    border.width: 1

    color: "#191a1c"

    ColumnLayout {

        property int innerPadding: 6

        spacing: 8

        anchors.fill: parent
        anchors.topMargin: innerPadding
        anchors.bottomMargin: innerPadding

        PageButton {
            Layout.preferredWidth: parent.width - (parent.innerPadding * 2)
            Layout.preferredHeight: parent.width - (parent.innerPadding * 2)
            Layout.alignment: Qt.AlignHCenter

            btnIconSource: "qrc:/resources/icons/stats.png"
            btnToolTipName: " Статистика "

            isBtnChecked: (window.currentPageIndex === 0)
            onClicked: {
                window.currentPageIndex = 0
            }

        }
        PageButton {
            Layout.preferredWidth: parent.width - (parent.innerPadding * 2)
            Layout.preferredHeight: parent.width - (parent.innerPadding * 2)
            Layout.alignment: Qt.AlignHCenter

            btnIconSource: "qrc:/resources/icons/menu-burger.png"
            btnToolTipName: " Найстройка правил приёма "

            isBtnChecked: (window.currentPageIndex === 1)

            onClicked: {
                window.currentPageIndex = 1
            }

        }
        Item {
            Layout.fillHeight: true
        }
        PageButton {
            Layout.preferredWidth: parent.width - (parent.innerPadding * 2)
            Layout.preferredHeight: parent.width - (parent.innerPadding * 2)
            Layout.alignment: Qt.AlignHCenter

            btnIconSource: "qrc:/resources/icons/settings.png"
            btnToolTipName: " Настройки "

            isBtnChecked: (window.currentPageIndex === 2)
            onClicked: {
                window.currentPageIndex = 2
            }

        }

    }
}