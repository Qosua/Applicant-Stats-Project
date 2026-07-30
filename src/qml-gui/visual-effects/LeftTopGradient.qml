import QtQuick
import QtQuick.Layouts
import QtGraphs
import QtQuick.Controls
import ApplicantStatsProjectModule

Canvas {
    anchors.fill: parent
    onPaint: {
        var ctx = getContext("2d")
        ctx.clearRect(0, 0, parent.width, parent.height)
        var cx = width * 0.17
        var cy = -height * 0.1
        var grad = ctx.createRadialGradient(cx, cy, 0, cx, cy, width * 0.3)
        grad.addColorStop(0.0, "rgba(80,115,193,0.6)")
        grad.addColorStop(1.0, "rgba(79,117,204,0)")
        ctx.fillStyle = grad
        ctx.fillRect(0, 0, width, height)
    }
}