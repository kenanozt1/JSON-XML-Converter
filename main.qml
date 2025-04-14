import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0
ApplicationWindow {
    width: 600
    height: 800
    visible: true
    title: qsTr("Json-XML Comverter")
    Rectangle{
        anchors.fill: parent
        color:"#f2f2f3"

        Rectangle{
            id:title
            width: parent.width
            height: parent.height*0.1
            color:"transparent"
            Text {
                text: qsTr("JSON <-> XML Converter")
                // font.bold: true
                font.pixelSize: 40
                anchors.centerIn: parent
                color:"#253244"
            }
        }

        Rectangle{
            id:rectProcess
            anchors.top: title.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 15
            color:"#fafafa"
            border.width: 2
            border.color:"lightgray"
            Rectangle{
                id:rectTabButtons
                width: parent.width
                height: parent.height*0.05
                color:"transparent"
                Rectangle{
                    id:rectJsonToXml
                    width: parent.width/2
                    height: parent.height
                    color:"transparent"
                    Text {
                        text: "JSON to XML"
                        anchors.centerIn: parent
                        font.pixelSize: 20
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            parent.border.width = 0
                            parent.color = "transparent"
                            rectXmlToJson.border.width = 2
                            rectXmlToJson.border.color = "lightgray"
                            rectXmlToJson.color = "#edeeed"
                            loader.sourceComponent = compjsonToXml
                        }
                    }
                }
                Rectangle{
                    id:rectXmlToJson
                    width: parent.width/2
                    height: parent.height
                    color:"#edeeed"
                    border.width: 2
                    border.color: "lightgray"
                    anchors.left: rectJsonToXml.right
                    Text {
                        text: "XML to JSON"
                        anchors.centerIn: parent
                        font.pixelSize: 20
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            parent.border.width = 0
                            parent.color = "transparent"
                            rectJsonToXml.border.width = 2
                            rectJsonToXml.border.color = "lightgray"
                            rectJsonToXml.color = "#edeeed"
                            loader.sourceComponent = compXmlToJson
                        }
                    }
                }
            }
            Loader{
                id:loader
                focus:true
                anchors.top:rectTabButtons.bottom
                sourceComponent: compjsonToXml
            }


            Component{
                id:compjsonToXml
                TabContent{
                    id:jsonToXml
                    width: rectProcess.width
                    height: rectProcess.height - rectTabButtons.height - 30
                    buttonText:"Send Json"
                    placeholder:"Enter JSON"
                    action:function(request){
                        response = converterController.jsontoXml(request)
                    }
                }
            }

            Component{
                id:compXmlToJson
                TabContent{
                    id:xmlToJson
                    width: rectProcess.width
                    height: rectProcess.height - rectTabButtons.height - 30
                    buttonText:"Send Xml"
                    placeholder:"Enter XML"
                    action:function(request){
                        response = converterController.xmlToJson(request)
                    }
                }
            }
        }
    }
}
