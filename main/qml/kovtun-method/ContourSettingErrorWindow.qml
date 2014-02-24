import QtQuick 2.1
import QtQuick.Window 2.1
import QtQuick.Controls 1.0

Window {
    id: window
    title: qsTr("Ошибка разбора JSON")
    width: content.width
    height: content.height
    property string errorText: "none"

    Item {
        id: content
        width: childrenRect.width
        height: childrenRect.height + 20

        Text {
            text: errorText
        }

        MouseArea {
            onClicked: {
                window.visible = false;
            }
        }
    }

}
