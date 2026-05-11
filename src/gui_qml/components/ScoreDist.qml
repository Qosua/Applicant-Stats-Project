import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtGraphs

Rectangle {
    id: root

    property var distribution: []
    property int firstBucketScore: 100
    property int bucketStep: 10

    readonly property int maxValue: {
        let m = 0
        for (let i = 0; i < distribution.length; ++i)
            if (distribution[i] > m) m = distribution[i]
        return Math.max(m, 1)
    }

    color: "#222326"
    border.width: 1
    border.color: "#333333"
    radius: 12

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 6
        anchors.leftMargin: 0
        spacing: 2

        Label {
            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter
            text: "Распределение баллов поступающих"
            color: "#eeeeee"
            font.pixelSize: 16
            elide: Text.ElideRight
        }
        GraphsView {
            id: graph
            Layout.fillHeight: true
            Layout.fillWidth: true
            theme: GraphsTheme {
                colorScheme: GraphsTheme.ColorScheme.Dark

                backgroundColor: "transparent"
                plotAreaBackgroundColor: "#202022"

                seriesColors: ["#5cb180"]
                borderColors: ["#5cb180"]

                grid.mainColor: "#3a3a3c"
                grid.subColor: "#222223"
                grid.mainWidth: 1
                grid.subWidth: 1

                axisX.mainColor: "transparent"
                axisY.mainColor: "transparent"
                axisX.subColor: "transparent"
                axisY.subColor: "transparent"

                labelTextColor: "#dddddd"
                labelFont.pixelSize: 13
            }

            axisX: BarCategoryAxis {
                categories: {
                    let cats = []
                    for (let i = 0; i < root.distribution.length; ++i)
                        cats.push(String(root.firstBucketScore + i * root.bucketStep))
                    return cats
                }


                titleText:    "Диапозоны баллов"
                titleVisible: true
                titleColor:   "#dddddd"
                titleFont.pixelSize: 14
            }
            axisY: ValueAxis {
                min: 0
                max: Math.ceil(root.maxValue * 1.1 / 2) * 2
                tickInterval: 1
                subTickCount: 1

                labelFormat: "%.0f"
                titleText:    "Количество абитуриентов"
                titleVisible: true
                titleColor:   "#dddddd"
                titleFont.pixelSize: 14
            }

            BarSeries {
                barWidth: 0.9
                labelsMargin: 4

                BarSet {
                    values: root.distribution
                }
            }
        }
    }
}