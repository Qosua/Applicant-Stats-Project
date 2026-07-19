import QtQuick
import QtQuick.Layouts
import QtGraphs
import QtQuick.Controls
import ApplicantStatsProjectModule

Window {
    id: window

    property int currentPageIndex: 0

    width: 1280; minimumWidth: 720
    height: 720; minimumHeight: 480

    visible: true
    color: "#26282b"

    LeftTopGradient {
        anchors.fill: parent
    }

    PagesMenu {
        id: pagesMenu
    }

    StackLayout {
        id: pagesStack

        anchors.top: parent.top
        anchors.bottom: statusBar.top
        anchors.right: parent.right
        anchors.left: pagesMenu.right

        anchors.margins: 8
        anchors.topMargin: 10
        anchors.bottomMargin: 0

        currentIndex: window.currentPageIndex

        StatsPage {
            id: splitView
        }

        Rectangle {
            color: "#232b40"
        }

        Rectangle {
            color: "#7160a8"

        }
    }

    StatusBar {
        id: statusBar
    }

    Rectangle {
        id: dimOverlay
        anchors.fill: parent
        color: "#000000"
        opacity: 0.45
        visible: false
        Behavior on opacity {
            NumberAnimation { duration: 150; easing.type: Easing.OutQuad }
        }
    }

}



