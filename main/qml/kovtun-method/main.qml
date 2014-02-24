import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.1
import QtQuick.Layouts 1.0
import KovtunMethod 1.0

ApplicationWindow {
    id: window
    title: qsTr("Kovtun method")
    width: 640
    height: 480

    ExecutionDataWindow {
        id: executionDataWindow
        visible: false
    }

    Item {
        id: content
        anchors.fill: parent
        anchors.topMargin: 5
        anchors.rightMargin: 5
        anchors.bottomMargin: 5
        anchors.leftMargin: 5

        Item {
            id: controls
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            height: stepIndexText.y + stepIndexText.height

            Button {
                id: performStepButton
                anchors.left: parent.left
                text: "Шаг вперед"
                action: Action {
                    onTriggered: {
                        loadingText.visible = true;
                        executionControls.invalidate();
                        kovtunMethodExecuter.setUnitsDimension(executionControls.unitsDimension);
                        kovtunMethodExecuter.performNextStep();
                    }
                }
            }

            Button {
                id: resetButton
                anchors.left: performStepButton.right
                anchors.leftMargin: 10
                text: "Сбросить"
                action: Action {
                    onTriggered: {
                        kovtunMethodExecuter.reset();
                    }
                }
            }

            Button {
                id: chooseContour
                anchors.left: resetButton.right
                anchors.leftMargin: 10
                text: "Выбрать контур"
                action: Action {
                    onTriggered: {
                        fileDialog.visible = true;
                    }
                }
            }

            ExecutionAdditionalControls {
                id: executionControls
                anchors.left: chooseContour.right
                anchors.leftMargin: 30

                onFilledRectanglesLinesVisibleChanged: {
                    kovtunMethodPainter.setFilledRectanglesLinesVisibility(filledRectanglesLinesVisible);
                    kovtunMethodPainter.update();
                }

                onGravityCentersVisibleChanged: {
                    kovtunMethodPainter.setGravityCentersVisibility(gravityCentersVisible);
                    kovtunMethodPainter.update();
                }

                onUnitsVisibleChanged: {
                    kovtunMethodPainter.setUnitsVisibility(unitsVisible);
                    kovtunMethodPainter.update();
                }

                onNeighborMethodExecutionChanged: {
                    if (neighborMethodExecution) {
                        kovtunMethodExecuter.setNeighborMethodExecution();
                    } else {
                        kovtunMethodExecuter.setFractalMethodEXecution();
                    }
                }
            }

            Text {
                id: stepIndexText
                text: "номер шага:"
                anchors.top: performStepButton.bottom
                anchors.topMargin: 10
            }

            Text {
                id: stepIndexValue
                text: stepIndex
                anchors.left: stepIndexText.right
                anchors.leftMargin: 10
                anchors.verticalCenter: stepIndexText.verticalCenter
                property int stepIndex: 0
            }

            Button {
                id: showExecutionDataButton
                anchors.left: stepIndexValue.right
                anchors.leftMargin: 10
                anchors.verticalCenter: stepIndexValue.verticalCenter
                text: "Данные"
                action: Action {
                    onTriggered: {
                        executionDataWindow.visible = !executionDataWindow.visible;
                    }
                }
            }
        }

        Item {
            id: result
            anchors.top: controls.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: progressBar.top

            KovtunMethodPainter {
                id: kovtunMethodPainter
                anchors.fill: parent
                anchors.topMargin: 5
                anchors.rightMargin: 5
                anchors.bottomMargin: 5
                anchors.leftMargin: 5
                objectName: "kovtunMethodPainter"
            }

            Text {
                id: loadingText
                text: "Подождите, выполняются расчёты..."
                color: "red"
                font.pointSize: 0.035 * kovtunMethodPainter.width
                anchors.centerIn: kovtunMethodPainter
                visible: false
            }
        }

        ProgressBar {
            id: progressBar
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            value: executerProgressWatcher.progress
        }
    }

    KovtunMethodExecuterQmlInterface {
        id: kovtunMethodExecuter
        objectName: "kovtunMethodExecuter"

        onExecutionReset: {
            kovtunMethodPainter.update();
            stepIndexValue.stepIndex = 0;
            executionDataWindow.reset();
        }
        onStepStartedPerforming: {
            executionControls.onStepStartedPerforming();
        }

        onStepPerformed: {
            kovtunMethodPainter.update();
            loadingText.visible = false;
            executionDataWindow.error = getCurrentError();
            executionDataWindow.activeRectanglesCount = getActiveRectanglesCount();
            executionDataWindow.filledRectanglesCount = getFilledRectanglesCount();
            executionDataWindow.usedColorsCount = getUsedColorsCount();
            stepIndexValue.stepIndex++;
            executionControls.onStepPerformed();
        }
    }

    ClosedContourSetterQmlInterface {
        id: closedContourSetter
        objectName: "closedContourSetter"

        onClosedContourIsSet: {
            kovtunMethodPainter.update();
            stepIndexValue.stepIndex = 0;
            executionDataWindow.reset();
        }

        onErrorOccured: {
            contourSettingErrorWindow.errorText = errorDescription;
            contourSettingErrorWindow.visible = true;
        }
    }

    FileDialog {
        id: fileDialog
        title: "JSON-контур"
        onAccepted: {
            closedContourSetter.setByFile(fileDialog.fileUrl);
            visible = false;
        }
        onRejected: {
            visible = false;
        }
    }

    ContourSettingErrorWindow {
        id: contourSettingErrorWindow
        visible: false
    }
}
