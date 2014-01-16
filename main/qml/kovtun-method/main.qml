import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.0
import KovtunMethod 1.0

ApplicationWindow {
    id: window
    title: qsTr("Kovtun method")
    width: 640
    height: 480

    Item {
        id: content
        anchors.fill: parent

        Button {
            id: performStepButton
            anchors.verticalCenter: parent.verticalCenter
            text: "Выполнить шаг"
            action: Action {
                onTriggered: {
                    kovtunMethodExecuterQmlInterface.performNextStep();
                }
            }
        }

        Button {
            id: resetButton
            anchors.top: performStepButton.bottom
            text: "Сбросить"
            action: Action {
                onTriggered: {
                    kovtunMethodExecuterQmlInterface.reset();
                }
            }
        }

        KovtunMethodPainter {
            anchors.left: performStepButton.right
            anchors.leftMargin: 10
            height: parent.height
            width: parent.width - x
            objectName: "kovtunMethodPainter"
        }
    }


}
