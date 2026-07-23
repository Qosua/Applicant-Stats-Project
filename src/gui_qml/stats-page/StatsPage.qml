import QtQuick
import QtQuick.Layouts
import QtGraphs
import QtQuick.Controls
import ApplicantStatsProjectModule

SplitView {

    function getFileName(mode) {
        if (mode === "faculty") {
            return "FacultyStatsPage.qml"
        } else if (mode === "direction") {
            return "DirectionStatsPage.qml"
        } else {
            return "EmptyStatsPlaceholder.qml"
        }
    }

    orientation: Qt.Horizontal
    handle: Rectangle {

        implicitWidth: 4

        color: SplitHandle.hovered ? "#556da3" : "transparent"

        Behavior on color {
            ColorAnimation {
                duration: 100
            }
        }
    }

    Rectangle {

        SplitView.preferredWidth: parent.width / 4
        SplitView.minimumWidth: parent.width / 10

        color: "transparent"
        topLeftRadius: 10
        bottomLeftRadius: 10

        SplitView {

            anchors.fill: parent
            orientation: Qt.Vertical
            handle: Rectangle {

                implicitHeight: 5

                color: SplitHandle.hovered ? "#556da3" : "transparent"

                Behavior on color {
                    ColorAnimation {
                        duration: 100
                    }
                }
            }

            Rectangle {
                color: "#191a1c"
                border.width: 1
                border.color: "#323232"
                topLeftRadius: 10
                SplitView.fillHeight: tablesList.collapsed
                SplitView.preferredHeight: parent.height / 2
                SplitView.minimumHeight: parent.height / 10

                FacultyTreeView {
                    anchors.fill: parent
                    anchors.margins: 8

                    onDirectionSelected: (idx) => {
                        detailLoader.mode = "direction"
                        detailLoader.payload = {sourceIndex: idx}
                    }
                    onFacultySelected: (name) => {
                        detailLoader.mode = "faculty"
                        detailLoader.payload = {facultyName: name}
                    }
                    onSelectionCleared: {
                        detailLoader.mode = "none"
                        detailLoader.payload = null
                    }
                }
            }
            TablesList {
                id: tablesList
                color: "#191a1c"
                border.width: 1
                border.color: tablesList.dropped ? "#556da3" : "#323232"
                bottomLeftRadius: 10

                SplitView.preferredHeight: collapsed ? collapsedHeight : parent.height / 2
                SplitView.minimumHeight: collapsed ? collapsedHeight : parent.height / 10
                SplitView.maximumHeight: collapsed ? collapsedHeight : Number.POSITIVE_INFINITY

            }
        }
    }

    Rectangle {

        SplitView.preferredWidth: parent.width / (3 / 4)
        SplitView.minimumWidth: parent.width / 10
        color: "#191a1c"
        border.color: "#323232"; border.width: 1
        topRightRadius: 10
        bottomRightRadius: 10
        clip: true

        Rectangle {
            id: searchBar
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 5

            height: 40
            color: "red"

        }

        Loader {
            id: detailLoader
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: searchBar.bottom
            anchors.margins: 1

            property string mode: "none"
            property var payload: null

            source: getFileName(mode)

            onLoaded: if (item) item.payload = payload
            onPayloadChanged: if (item) item.payload = payload

            Connections {
                target: statsPageModel

                function onLoadDirectionPage(index) {
                    detailLoader.mode = "direction"
                    detailLoader.payload = {sourceIndex: index}
                }

                function onLoadFacultyPage(name) {
                    detailLoader.mode = "faculty"
                    detailLoader.payload = {facultyName: name}
                }

                function onNothingToLoad() {
                    detailLoader.mode = "none"
                    detailLoader.payload = null
                }
            }
        }
    }
}