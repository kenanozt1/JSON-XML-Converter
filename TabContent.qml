import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0

Rectangle{
    id:root
    color:"transparent"

    property string buttonText : "Send"
    property var action : function(){}
    property string placeholder
    property string response : ""
    property string request : ""
    ScrollView{
        id:scrollView
        anchors{
            top:root.top
            left:root.left
            right:root.right
            margins:15
        }
        height: root.height*0.4
        background: Rectangle{
            border.width: 2
            border.color: "lightgray"
            radius:5
        }
        TextArea{
            id:textarea
            wrapMode: TextArea.Wrap
            clip:true
            placeholderText: placeholder
            onTextChanged: {
                request = textarea.text
            }
        }
    }

    Rectangle{
        id:sendButton
        anchors.top: scrollView.bottom
        anchors.left: root.left
        anchors.right: root.right
        anchors.margins:  15
        height:root.height*0.1
        color:"#4e606f"
        radius:15
        Text {
            anchors.centerIn: parent
            text: root.buttonText
            font.pixelSize: 18
            color:"white"
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                action(textarea.text);
            }
        }
    }
    ScrollView{
        id:scrollViewResponse
        anchors{
            top:sendButton.bottom
            left:root.left
            right:root.right
            margins:15
        }
        height: root.height*0.4
        background: Rectangle{
            border.width: 2
            border.color: "lightgray"
            radius:5
        }
        TextArea{
            id:textareaResponse
            wrapMode: TextArea.Wrap
            readOnly: true
            clip:true
            text:response
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    converterController.copyText(response);
                    copySuccess.visible = true;
                    timer.start()
                }

            }
        }
    }
    Timer{
        id:timer
        running:false
        repeat: false
        interval: 5000
        onTriggered: {
            copySuccess.visible = false
            timer.stop()
        }
    }

    Text {
        id: copySuccess
        visible:false
        anchors.top: scrollViewResponse.bottom
        anchors.margins: 5
        text: qsTr("Copied!")
        font.pixelSize: 18
        anchors.horizontalCenter: parent.horizontalCenter
        color:"green"
    }
}

