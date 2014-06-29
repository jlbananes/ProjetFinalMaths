import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import OpenGLUnderQML 1.0

Item {

    width : 7680
    height : 4320
    property int xmove: 0
    property int ymove: 0
    property int zmove: 1
    property int clickedButton: 0
    property int mode: 1
    property string path: ""
    property int level: 1

    Drawing {
        id: drawing
        function update()
        {
            //drawing.t += 1/600;
            drawing.x = xmove;
            drawing.y = ymove;
            drawing.z = zmove;
            drawing.clickedButton = clickedButton;

            if(points.checked)
                drawing.mode = 1;
            else if(lines.checked)
                drawing.mode = 2;
            else if(faces.checked)
                drawing.mode = 3;

            drawing.path = textpath.text;
            drawing.level = levelSelection.level;

        }
    }



    Timer {
        id: gameTimer
        interval: 1
        running: true
        repeat: true
        triggeredOnStart: true
        onTriggered:
        {
            drawing.update();
        }
    }

    FileDialog {
         id:fileDialog
         title: "Select a Wavefront file"
         nameFilters: [ "Wavefront files (*.obj)" ]
         selectFolder: false
         selectMultiple: false
         onAccepted: {
             textpath.text = fileDialog.fileUrl;
         }
         onRejected: {
             textpath.text = "";
             fileDialog.close();
         }
     }

    Rectangle {
        z:2
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 0
        border.width: 1
        border.color: "white"
        anchors.fill: label
        anchors.margins: -10
        MouseArea {
            id: button
            hoverEnabled: true
            anchors.fill: parent
            onClicked: { Qt.quit(); }
        }
    }

    ColumnLayout {
        z:2
        id: firstColumn
        Layout.minimumWidth: implicitWidth
        Layout.fillWidth: false

        GroupBox {
            z: 2
            id: mode
            title: "Mode"
            Layout.fillWidth: false
            RowLayout {
                ExclusiveGroup { id: modeGroup }
                RadioButton {
                    id:points
                    checked: true
                    exclusiveGroup: modeGroup
                    text: qsTr("Points")
                }
                RadioButton {
                    id:lines
                    exclusiveGroup: modeGroup
                    text: qsTr("Lines")
                }
                RadioButton {
                    id:faces
                    exclusiveGroup: modeGroup
                    text: qsTr("Faces")
                }
            }
        }

        GroupBox {
            z: 2
            id: importMesh
            title: "Import"
            Layout.fillWidth: false
            RowLayout {
                TextField {
                    id:textpath
                    placeholderText: qsTr("path to the mesh")
                }
                Button {
                    id:buttonImport
                    text: "Import"
                    onClicked: { fileDialog.open(); }
                }
            }
        }

        GroupBox {
            z: 2
            id: levelSelection
            title: "Level"
            Layout.fillWidth: false
            property int level: 1
            RowLayout {
                Button {
                    id:minus
                    text: "-"
                    onClicked: { levelSelection.level = (levelSelection.level<=0)?0:levelSelection.level-1; }
                }
                TextField {
                    id:levelSelected
                    text: levelSelection.level
                    horizontalAlignment: TextInput.AlignHCenter
                }
                Button {
                    id:plus
                    text: "+"
                    onClicked: { levelSelection.level = (levelSelection.level>=100)?100:levelSelection.level+1; }
                }
            }
        }
    }

    Text {
        z:2 //On front
        id: label
        color: "black"
        wrapMode: Text.WordWrap
        text: "Welcome"
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
    }

    MouseArea {
        z:1 //On the background
        id: mouseNav
        hoverEnabled: true
        anchors.fill: parent
        property int clickedX: 0
        property int clickedY: 0

        onWheel: {
            if (wheel.angleDelta.y > 0)
                zmove-=wheel.modifiers?2:1;
            else
                zmove+=wheel.modifiers?2:1;
        }

        onPositionChanged: {
            if (clickedButton == 1)
            {
                xmove = mouseX - clickedX;
                ymove = -(mouseY - clickedY);

                //label.text = "dÃ©placement en X :" + xmove + "  dÃ©placement en y : " + ymove;
            }

            else
            {
                //label.text = " X: " + mouseX + " Y: " + mouseY;
            }
        }
        onPressed: {label.text = "clic souris"; clickedButton = !clickedButton; clickedX = mouseX; clickedY = mouseY; }
        onReleased: {clickedButton = 0;}
    }
}
