#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QNetworkReply>
#include <QStandardItemModel>
#include "structureelement.h"
#include <QXmlStreamReader>
#include <QMap>
#include <QFile>
#include "mymainwindow.h"
#include "utils.h"

class Parser : public QObject
{
    Q_OBJECT

public:
    static QList<QString> parseFeatures(QNetworkReply *reply);
    static void parseCourses(QNetworkReply *reply, QStandardItemModel *itemModel);
    static void parseFiles(QNetworkReply *reply, Structureelement *course, QString downloadDirectoryPath);
};

#endif // PARSER_H
