#include "XSCTreeMetaData.h"
#include "InfoAction.h"
#include <Application.h>
#include <util/Serialization.h>
#include <QtCore>
#include <QtDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <algorithm>
#include <iostream>
#include <iostream>
#include <QJsonDocument>
#include <QJsonValue>

Q_PLUGIN_METADATA(IID "nl.BioVault.XSCTreeMetaData")

using namespace mv;



XSCTreeMetaData::~XSCTreeMetaData(void)
{

}


void XSCTreeMetaData::fromVariantMap(const QVariantMap& variantMap)
{
    RawData::fromVariantMap(variantMap);

    if (variantMap.contains("CSV:TreeMetaData")) {
        QJsonDocument doc = QJsonDocument::fromJson(variantMap["CSV:TreeMetaData"].toByteArray());
        setTreeMetaDataRaw(doc.object());
    }
}

QVariantMap XSCTreeMetaData::toVariantMap() const
{
    QVariantMap variantMap = RawData::toVariantMap();

    QJsonDocument doc(_data);

    variantMap["CSV:TreeMetaData"] = doc.toJson();
    return variantMap;
}

void XSCTreeMetaData::init()
{


}



QString extractPropertyNames(QJsonObject _data) {
    QStringList finalStringList;

    // Get the keys of the main object
    QStringList mainKeys = _data.keys();

    if (!mainKeys.isEmpty()) {
        // Get the first inner object
        QJsonObject innerObject = _data.value(mainKeys.first()).toObject();

        // Iterate over the keys of the inner object
        for (const QString& innerKey : innerObject.keys()) {
            QJsonObject innerInnerObject = innerObject.value(innerKey).toObject();
            QStringList keys = innerInnerObject.keys();

            // Add the keys to the final string
            finalStringList.append(innerKey + ": " + keys.join(", "));
        }
    }

    // Join the final string list with newlines
    QString finalString = finalStringList.join("\n\n");

    return finalString;
}


QStringList extractLeafNames(const QJsonObject& jsonObj) {
    QStringList keys = jsonObj.keys();
    return keys;
}



Dataset<DatasetImpl> XSCTreeMetaData::createDataSet(const QString& guid /*= ""*/) const
{
    auto dataset = Dataset<DatasetImpl>(new XSCTreeMeta(getName(), true, guid));
    return dataset;
}


void XSCTreeMetaData::setTreeMetaDataRaw(QJsonObject jsonString)
{
    //sortJsonObject(jsonString);

    
    //qDebug() << "**************************************************";
    _data = jsonString;
    _leafNames.clear();
    _leafNames = extractLeafNames(_data);
    _leafNames.sort();
    _propertyNames = "";
    _propertyNames = extractPropertyNames(_data);

    //std::cout<< "Species names: " << _speciesNames.join(", ").toStdString() << std::endl;
    //qDebug() << "**************************************************";
}

void XSCTreeMetaData::setTreeMetaLeafNamesRaw(QStringList jsonString)
{
    _leafNames = jsonString;
}

QJsonObject& XSCTreeMetaData::getTreeMetaDataRaw()
{
    return _data;
}

QStringList& XSCTreeMetaData::getTreeMetaLeafNamesRaw()
{
    return _leafNames;
}

QString& XSCTreeMetaData::getTreeMetaPropertyNamesRaw()
{
    return _propertyNames;
}

XSCTreeMetaDataFactory::XSCTreeMetaDataFactory() 
{
    setIconByName("table");
}

mv::plugin::RawData* XSCTreeMetaDataFactory::produce()
{
    return new XSCTreeMetaData(this);
}

void XSCTreeMeta::init()
{
    _infoAction = QSharedPointer<InfoAction>::create(nullptr, *this);

    addAction(*_infoAction.get());

}

std::vector<std::uint32_t>& XSCTreeMeta::getSelectionIndices()
{
    return getSelection<XSCTreeMeta>()->indices;
}

void XSCTreeMeta::setSelectionIndices(const std::vector<std::uint32_t>& indices)
{
}

bool XSCTreeMeta::canSelect() const
{
    return false;
}

bool XSCTreeMeta::canSelectAll() const
{
    return false;
}

bool XSCTreeMeta::canSelectNone() const
{
    return false;
}

bool XSCTreeMeta::canSelectInvert() const
{
    return false;
}

void XSCTreeMeta::selectAll()
{
}

void XSCTreeMeta::selectNone()
{
}

void XSCTreeMeta::selectInvert()
{
}

void XSCTreeMeta::setTreeMetaData(QJsonObject jsonString)
{
    //qDebug() << "%%3ItsSetting3%%";
    getRawData<XSCTreeMetaData>()->setTreeMetaDataRaw(jsonString);
    //qDebug()<< "jsonString"<<jsonString;
    //qDebug() << "%%3ItsSetting3%%";
    //getRawData<XSCTreeMetaData>()->changed();
}
void XSCTreeMeta::setTreeMetaLeafNames(QStringList jsonString)
{
    getRawData<XSCTreeMetaData>()->setTreeMetaLeafNamesRaw(jsonString);
}
QJsonObject& XSCTreeMeta::getTreeMetaData()
{
    return  getRawData<XSCTreeMetaData>()->getTreeMetaDataRaw();// TODO: insert return statement here
}

QStringList& XSCTreeMeta::getTreeMetaLeafNames()
{
    return  getRawData<XSCTreeMetaData>()->getTreeMetaLeafNamesRaw();// TODO: insert return statement here
}

QString& XSCTreeMeta::getTreeMetaPropertyNames()
{
    return  getRawData<XSCTreeMetaData>()->getTreeMetaPropertyNamesRaw();// TODO: insert return statement here
}

void XSCTreeMeta::fromVariantMap(const QVariantMap& variantMap)
{
    DatasetImpl::fromVariantMap(variantMap);

    getRawData<XSCTreeMetaData>()->fromParentVariantMap(variantMap);
    events().notifyDatasetDataSelectionChanged(this);
}

QVariantMap XSCTreeMeta::toVariantMap() const
{
    auto variantMap = DatasetImpl::toVariantMap();

    getRawData<XSCTreeMetaData>()->insertIntoVariantMap(variantMap);

    return variantMap;
}
