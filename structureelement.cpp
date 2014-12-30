/****************************************************************************
** This file is part of Sync-my-L2P.
**
** Sync-my-L2P is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Sync-my-L2P is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with Sync-my-L2P.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/


#include <QStringBuilder>
#include <QClipboard>
#include <QApplication>

#include "structureelement.h"

Structureelement::Structureelement(QString name, QUrl url, int time, qint32 size, QString cid, MyItemType typeEX)
    :QStandardItem(name),
     included(true),
     url(url),
     time(QDateTime::fromMSecsSinceEpoch(qint64(1000) * time)),
     size(size),
     typeEX(typeEX),
     cid(cid)
{
    synchronised = NOT_SYNCHRONISED;
    chooseIcon();
}

QVariant Structureelement::data(int role) const
{
    switch(role)
    {
    case includeRole:
        return included;
    case urlRole:
        return url;
    case sizeRole:
        return size;
    case dateRole:
        return time;
    case synchronisedRole:
        return synchronised;
    case cidRole:
        return cid;
    case Qt::StatusTipRole:
    {
        QString statustip;
        if (typeEX == fileItem)
        {
            statustip.append(text() % " - ");
            if (size > 1048576)
                statustip.append(QString::number(size/1048576.0,'f',2) % " MB");
            else
                 statustip.append(QString::number(size/1024.0,'f',2) % " KB");

            statustip.append(" - " % time.toString("ddd dd.MM.yy hh:mm"));
        }
        return statustip;
    }
    case Qt::ForegroundRole:
        if (included)
        {
            if (synchronised == NOW_SYNCHRONISED)
                return QBrush(Qt::green);
            else if (synchronised == SYNCHRONISED)
                return QBrush(Qt::darkGreen);
            else
                return QBrush(Qt::black);
        }
        else
        {
            return QBrush(Qt::red);
        }
    case Qt::FontRole:
        // Semester und Veranstaltungen fett darstellen
        if (typeEX == courseItem || typeEX == semesterItem)
        {
            QFont BoldFont;
            BoldFont.setBold(true);
            return BoldFont;
        }
    default:
        return QStandardItem::data(role);
    }
}

/// Icon anhand des Types und der Dateiendung auswählen
void Structureelement::chooseIcon()
{
    if(typeEX == fileItem)
    {
        QString filename = text();
        // Hinzufügen des endungsabhängigen Icons
        // PDF
        if (filename.contains(QRegExp(".pdf$", Qt::CaseInsensitive)))
        {
           setIcon(QIcon(":/icons/pdf.png"));
        }
        // ZIP
        else if (filename.contains(QRegExp(".zip$", Qt::CaseInsensitive)))
        {
            setIcon(QIcon(":/icons/zip.png"));
        }
        // RAR
        else if (filename.contains(QRegExp(".rar$", Qt::CaseInsensitive)))
        {
            setIcon(QIcon(":/icons/rar.png"));
        }
        // Sonstige
        else
        {
            setIcon(QIcon(":/icons/otherFile.png"));
        }
    }
    else if(typeEX == courseItem)
    {
        setIcon(QIcon(":/icons/course.png"));
    }
    else if(typeEX == directoryItem)
    {
        setIcon(QIcon(":/icons/directory.png"));
    }
    else if(typeEX == semesterItem)
    {
        setIcon(QIcon(":/icons/semester.png"));
    }
}

/// Vergleich zwischen zwei Items für Sortierung.
/// Ordner haben Vorrang vor Dateien und sonst wird alphabetisch sortiert.
bool Structureelement::operator< (const QStandardItem& other) const
{

    if ((typeEX != fileItem) && (other.type() == fileItem))
    {
        return true;
    }
    else if ((typeEX == fileItem) && (other.type() != fileItem))
    {
        return false;
    }
    else
    {
        return (text().toLower() < other.text().toLower());
    }
}

/// Überladene Funktion für das Setzen der Daten
void Structureelement::setData(const QVariant &value, int role)
{
    switch(role)
    {
    case includeRole:
        this->included = value.toBool();
        break;
    case urlRole:
        this->url = value.toUrl();
        break;
    case sizeRole:
        this->size = value.toInt();
        break;
    case dateRole:
        this->time = value.toDateTime();
        break;
    case synchronisedRole:
        this->synchronised = static_cast<synchroniseStatus>(value.toInt());
        break;
    case cidRole:
        this->cid = value.toString();
        break;
    default:
        QStandardItem::setData(value, role);
    }
}
