#include "convertercontroller.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <algorithm>
#include <QGuiApplication>
#include <QClipboard>
ConverterController* ConverterController::instance = nullptr;
ConverterController::ConverterController(QObject *parent)
    : QObject{parent}
{}

QString ConverterController::jsontoXml(QString object)
{
    QByteArray jsonData = object.toUtf8();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON Parse Error:" << parseError.errorString();
        return "JSON Parse Error";
    }

    QJsonObject jsonObj = doc.object();
    QString xmlResult = jsonObjectToXml(jsonObj);
    return xmlResult;
}

QString ConverterController::jsonObjectToXml(QJsonObject jsonObj)
{
    QString xml;
    QStringList roots = jsonObj.keys();
    for(QString root:roots)
    {
        xml += "<" + root + ">";
        QJsonValue val = jsonObj.value(root);
        if(val.isObject())
        {
            xml += jsonObjectToXml(val.toObject());
        }
        else if(val.isArray())
        {
            QJsonArray arr = val.toArray();
            for (QJsonValue item : arr) {
                xml += "<item>";    //döngü olduğu için bu şekilde
                if (item.isObject()) {
                    xml += jsonObjectToXml(item.toObject());
                } else {
                    xml += item.toVariant().toString();
                }
                xml += "</item>";
            }
        }
        else{
            xml += val.toVariant().toString();
        }
        xml += "</" + root + ">";
    }
    return xml;
}
QString ConverterController::xmlToJson(QString object)
{
    QDomDocument doc;
    QString errorMsg;
    int errorLine, errorColumn;
    if (!doc.setContent(object, &errorMsg, &errorLine, &errorColumn)) {
        qWarning() << "XML parsing error at line:" << errorLine << "column:" << errorColumn << errorMsg;
        return "{}";
    }
    QJsonObject jsonObject;
    QDomElement root = doc.documentElement();
    convertXmlNodeToJson(root, jsonObject);
    QJsonDocument jsonDoc(jsonObject);
    return QString(jsonDoc.toJson(QJsonDocument::Indented));
}

void ConverterController::convertXmlNodeToJson(const QDomNode &node, QJsonObject &parentObject)
{
    if (node.isElement()) {
        QDomElement element = node.toElement();
        QString nodeName = element.tagName();

        QJsonObject currentObject;

        QDomNamedNodeMap attributes = element.attributes();
        for (int i = 0; i < attributes.count(); i++) {
            QDomNode attr = attributes.item(i);
            currentObject[attr.nodeName()] = attr.nodeValue();
        }

        QDomNodeList childNodes = element.childNodes();

        if (childNodes.count() == 1 && childNodes.at(0).isText()) {
            QString text = childNodes.at(0).toText().data().trimmed();
            if (!text.isEmpty()) {
                if (attributes.count() > 0) {
                    currentObject["#text"] = text;
                } else {
                    parentObject[nodeName] = text;
                    return;
                }
            }
        } else {
            QMap<QString, QJsonArray> arrayMap;

            for (int i = 0; i < childNodes.count(); i++) {
                QDomNode childNode = childNodes.at(i);

                if (childNode.isText()) {
                    QString text = childNode.toText().data().trimmed();
                    if (!text.isEmpty()) {
                        currentObject["#text"] = text;
                    }
                } else if (childNode.isElement()) {
                    QDomElement childElement = childNode.toElement();
                    QString childName = childElement.tagName();

                    QJsonObject childObject;
                    convertXmlNodeToJson(childNode, childObject);

                    if (childObject.contains(childName)) {
                        if (!arrayMap.contains(childName)) {
                            arrayMap[childName] = QJsonArray();
                            if (currentObject.contains(childName)) {
                                arrayMap[childName].append(currentObject[childName]);
                                currentObject.remove(childName);
                            }
                        }
                        arrayMap[childName].append(childObject[childName]);
                    } else {
                        QJsonObject::const_iterator it;
                        for (it = childObject.constBegin(); it != childObject.constEnd(); ++it) {
                            currentObject[it.key()] = it.value();
                        }
                    }
                }
            }

             QMap<QString, QJsonArray>::const_iterator it;
            for (it = arrayMap.constBegin(); it != arrayMap.constEnd(); ++it) {
                currentObject[it.key()] = it.value();
            }
        }

        if (!currentObject.isEmpty()) {
            parentObject[nodeName] = currentObject;
        } else {
            parentObject[nodeName] = QJsonValue();
        }
    }
}

void ConverterController::copyText(QString text)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(text);
}

