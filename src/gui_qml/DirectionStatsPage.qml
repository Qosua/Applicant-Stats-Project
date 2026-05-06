import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import ApplicantStatsProjectModule

Item {
    id: page

    property var payload: null
    readonly property int sourceIndex: payload ? (payload.sourceIndex ?? -1) : -1
    readonly property real letterSpacing: 1.05

    readonly property var stats: sourceIndex >= 0
        ? cppStats.directionStatsAt(sourceIndex)
        : null

    component StatCard: Rectangle {
        property string label: ""
        property var value: "—"
        property color accentColor: "#888888"

        Layout.fillWidth: true
        Layout.preferredHeight: 78
        radius: 8
        color: "#222326"
        border.color: "#222326"
        border.width: 1

        // Цветной акцент слева
        Rectangle {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 1
            width: 3
            radius: 2
            color: accentColor
            opacity: 0.85
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 14
            anchors.leftMargin: 18
            spacing: 4

            Label {
                text: label
                color: "#dddddd"
                font.pixelSize: 11
                font.letterSpacing: page.letterSpacing
                font.weight: Font.Medium
            }
            Item {
                Layout.fillHeight: true
            }
            Label {
                text: value
                color: accentColor
                font.pixelSize: 26
                font.weight: Font.DemiBold
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Label {
            text: stats ? stats.name : "STATS ARE NULL"
            color: "#eeeeee"
            font.pixelSize: 24
            font.letterSpacing: page.letterSpacing
            font.weight: Font.Medium
        }
        Label {
            text: stats ? "Факультет: " + stats.facultyName : "STATS ARE NULL"
            color: "#aaaaaa";
            font.pixelSize: 16
            font.letterSpacing: page.letterSpacing
            font.weight: Font.Medium
        }

        RowLayout {
            spacing: 12
            Label {
                text: "Вид обучения:";
                color: "#dddddd"; font.pixelSize: 15
                font.letterSpacing: page.letterSpacing
                font.weight: Font.Medium
            }
            Label {
                text: (stats ? stats.studyType : "ОШИБКА ВИДА ОБУЧЕНИЯ (QML)");
                color: "#aad3a8"; font.pixelSize: 15
                font.letterSpacing: page.letterSpacing
                font.weight: Font.Medium
            }
            Item {
            }
            Label {
                text: "Форма обучения:";
                color: "#dddddd"; font.pixelSize: 15
                font.letterSpacing: page.letterSpacing
                font.weight: Font.Medium
            }
            Label {
                text: (stats ? stats.studyForm : "ОШИБКА ФОРМЫ ОБУЧЕНИЯ (QML)");
                color: "#9ca9c8"; font.pixelSize: 15
                font.letterSpacing: page.letterSpacing
                font.weight: Font.Medium
            }
        }

        Rectangle {
            Layout.fillWidth: true;
            height: 1
            color: "#404040"
        }

        SplitView {

            anchors.margins: 2
            spacing: 4

            Layout.preferredWidth: parent.width
            Layout.preferredHeight: (parent.height / 2) + 120

            ScoreDist {
                SplitView.preferredHeight: parent.height
                SplitView.preferredWidth: parent.width / 1.8
                SplitView.minimumWidth: parent.width / 2.4

                distribution: stats ? stats.scoreDistribution : []
                firstBucketScore: stats ? stats.scoreBucketStart : 100
                bucketStep: stats ? stats.scoreBucketStep : 10
            }
            ListView {
                clip: true
                SplitView.minimumWidth: parent.width / 4

                model: 5
                spacing: 1

                delegate: Rectangle {
                    width: parent.width
                    height: 25

                    color: "red"
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true;
            height: 1
            color: "#404040"
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 10

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 78
                radius: 8
                color: "#222326"
                border.color: "#2d2d35"
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 14
                    spacing: 10

                    RowLayout {
                        spacing: 10
                        Label {
                            text: "ЗАЧИСЛЕНО"
                            color: "#dddddd"
                            font.pixelSize: 11
                            font.letterSpacing: page.letterSpacing
                            font.weight: Font.Medium
                            Layout.fillWidth: true
                        }
                        Label {
                            text: (stats ? stats.size : 0) + " / " + (stats ? stats.capacity : 0)
                            color: "#e8e8ec"
                            font.pixelSize: 18
                            font.weight: Font.DemiBold
                        }
                        Label {
                            text: stats && stats.capacity > 0
                                ? Math.round((stats.size / stats.capacity) * 100) + "%"
                                : "0%"
                            color: "#6fa86f"
                            font.pixelSize: 13
                            font.weight: Font.Medium
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 6
                        radius: 3
                        color: "#2a2a32"

                        Rectangle {
                            width: parent.width * (stats && stats.capacity > 0
                                ? Math.min(stats.size / stats.capacity, 1)
                                : 0)
                            height: parent.height
                            radius: parent.radius
                            gradient: Gradient {
                                orientation: Gradient.Horizontal
                                GradientStop {
                                    position: 0.0; color: "#4a8a4a"
                                }
                                GradientStop {
                                    position: 1.0; color: "#7ab87a"
                                }
                            }
                            Behavior on width {
                                NumberAnimation {
                                    duration: 350; easing.type: Easing.OutCubic
                                }
                            }
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                StatCard {
                    label: "ПРОХОДНОЙ"
                    value: stats && stats.size === stats.capacity ? stats.minScore : "—"
                    accentColor: "#d4a373"
                }
                StatCard {
                    label: "МАКСИМУМ"
                    value: stats && stats.maxScore !== undefined ? stats.maxScore : "—"
                    accentColor: "#7fb37f"
                }
                StatCard {
                    label: "МИНИМУМ"
                    value: stats && stats.minScore !== undefined ? stats.minScore : "—"
                    accentColor: "#c47878"
                }
                StatCard {
                    label: "СРЕДНИЙ"
                    value: stats && stats.meanScore !== undefined ? stats.meanScore.toFixed(1) : "—"
                    accentColor: "#7f9cc4"
                }
                StatCard {
                    label: "МЕДИАНА"
                    value: stats && stats.midScore !== undefined ? stats.midScore.toFixed(1) : "—"
                    accentColor: "#a37fc4"
                }
            }
        }

        Item {
            Layout.fillWidth: true; Layout.fillHeight: true
        }
    }
}