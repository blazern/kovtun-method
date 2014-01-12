import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.0
import KovtunMethod 1.0

ApplicationWindow {
    title: qsTr("Kovtun method")
    width: 640
    height: 480

    KovtunMethodPainter {
        anchors.fill: parent
        objectName: "kovtunMethodPainter"
    }
}
