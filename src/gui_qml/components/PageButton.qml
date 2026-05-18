import QtQuick
import QtQuick.Layouts
import QtGraphs
import QtQuick.Controls
import QtQuick.Effects
import ApplicantStatsProjectModule

Button {
    id: root

    function defineColor(hovered, chose) {
        if (chose)
            return btnChoseBackgroundColor;

        if (hovered)
            return btnHoverBackgroundColor;

        return btnBackgroundColor;
    }

    property bool isBtnChecked: false
    property string btnIconSource: ""
    property string btnToolTipName: ""
    property int btnToolTipDelay: 1
    property int iconSize: 22
    property color btnIconColor: "#ced0d6"
    property color btnBackgroundColor: "transparent"
    property color btnChoseBackgroundColor: "#556da3"
    property color btnHoverBackgroundColor: "#393b40"

    flat: true
    icon.source: btnIconSource
    icon.width: iconSize
    icon.height: iconSize
    icon.color: btnIconColor
    antialiasing: true
    hoverEnabled: true


    Rectangle {
        anchors.fill: parent
        color: { return defineColor(parent.hovered, parent.isBtnChecked)}
        radius: 8

        Behavior on color {
            ColorAnimation {
                duration: 100
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////

    HoverHandler {
        id: hover
        cursorShape: Qt.PointingHandCursor
    }
    ToolTip {
        visible: {return hover.hovered && root.btnToolTipName !== ""}
        delay: btnToolTipDelay

        x: parent.width + 6
        y: (parent.height - height) / 2

        contentItem: Text {
            text: btnToolTipName
            color: "#ffffff"
            font.pixelSize: 15
        }

        background: Rectangle {
            id: rect
            color: "#2b2b2b"
            radius: 5
            border.color: "#40444a"

            Behavior on color {
                ColorAnimation {
                    duration: 100
                }
            }
        }
    }

}