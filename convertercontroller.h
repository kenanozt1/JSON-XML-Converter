#ifndef CONVERTERCONTROLLER_H
#define CONVERTERCONTROLLER_H

#include <QObject>
#include <QDomNode>
class ConverterController : public QObject
{
    Q_OBJECT
    explicit ConverterController(QObject *parent = nullptr);
    static ConverterController* instance;
public:
    ConverterController(const ConverterController& obj) = delete;
    static ConverterController* getInstance(){
        if(instance == nullptr){
            instance = new ConverterController();
            return instance;
        }
        else{
            return instance;
        }
    }

    Q_INVOKABLE QString jsontoXml(QString object);
    QString jsonObjectToXml(QJsonObject jsonObj);

    Q_INVOKABLE QString xmlToJson(QString object);
    void convertXmlNodeToJson(const QDomNode &node, QJsonObject &jsonObject);

    Q_INVOKABLE void copyText(QString text);
private:
    QString originalText;
signals:
};

#endif // CONVERTERCONTROLLER_H
