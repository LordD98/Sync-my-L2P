#ifndef L2PITEMMODEL_H
#define L2PITEMMODEL_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QAuthenticator>
#include <QUrl>

#include <QRegExp>
#include <QDomDocument>

#include <QStringBuilder>
#include <QList>
#include <QLinkedList>
#include <QQueue>
#include <QSettings>

#include <QMessageBox>
#include <QFileDialog>

#include <QDir>
#include <QFile>

#include "parser.h"
#include "utils.h"
#include "options.h"

#define DATAFILENAME "data.xml"

class L2pItemModel : public QObject
{
    Q_OBJECT

public:
    L2pItemModel();
    ~L2pItemModel();

    const QStandardItemModel *getData() const { return data; }
    MySortFilterProxyModel *getProxy() { return &proxy; }

    void setOptions(Options* options) { this->options = options; }
    void loadDataFromFile();
    void saveDataToFile();

public slots:
    void loadDataFromServer();

signals:
    void loadingFinished(bool successful);

protected:
    void parseDataFromXml(QDomElement input, QStandardItem *parentItem);
    void parseDataToXml(QDomDocument &output, QStandardItem *item, QDomElement *parentItem);
    void addCoursesFromReply(QNetworkReply* reply);
    void addFeatureFromReply(QNetworkReply* reply, Structureelement *course);
    void addFilesFromReply(QNetworkReply* reply, Structureelement *course);
    QNetworkRequest createApiRequest(Structureelement* course, QString apiCommand);
    void getItemList(QStandardItem *topElement, QList <Structureelement *> &list);
    void requestCourses();
    void requestFeatures();

    QStandardItemModel *data = new QStandardItemModel();
    QStandardItemModel *oldData = nullptr;

    MySortFilterProxyModel proxy;

    struct ReplyInfo
    {
        Structureelement* item;

        enum Type
        {
            courses,
            features,
            files
        };

        Type type;
    };

    QNetworkAccessManager manager;
    QMap<QNetworkReply*, ReplyInfo> replies;

    Options *options = nullptr;


protected slots:
    void serverDataRecievedSlot(QNetworkReply *reply);
};

#endif // L2PITEMMODEL_H
