import QtQuick 2.0
import QtQuick.Layouts 1.0

Row {
    spacing: 10
    property string dataDescription: ""
    property string dataValue: ""

    Text {
        id: dataDecsription
        text: parent.dataDescription
    }

    Text {
        text: parent.dataValue
        anchors.leftMargin: 10
    }
}
