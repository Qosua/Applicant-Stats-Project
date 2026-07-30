import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: page

    property var payload: null
    readonly property string facultyName: payload ? (payload.facultyName ?? "") : ""

    readonly property var stats: facultyName !== ""
        ? statsPageModel.facultyStats(facultyName)
        : null

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12
        visible: stats !== null

        Label { text: facultyName; color: "#eee"; font.pixelSize: 24 }



        Item { Layout.fillWidth: true; Layout.fillHeight: true }
    }
}