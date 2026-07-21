#pragma once
#include "XSCTreeMetaData_export.h"
#include <RawData.h>
#include <Set.h>
#include <QJsonObject>
using namespace mv;
using namespace mv::plugin;

// =============================================================================
// Data Type
// =============================================================================
class InfoAction;
const mv::DataType XSCTreeMetaType = mv::DataType(QString("XSCTreeMeta"));

// =============================================================================
// Raw Data
// =============================================================================

class XSCTREEMETADATA_EXPORT XSCTreeMetaData : public mv::plugin::RawData
{
public:

    XSCTreeMetaData(PluginFactory* factory) : mv::plugin::RawData(factory, XSCTreeMetaType) { }
    ~XSCTreeMetaData(void) override;

    void init() override;

    /**
     * Create dataset for raw data
     * @param guid Globally unique dataset identifier (use only for deserialization)
     * @return Smart pointer to dataset
     */
    Dataset<mv::DatasetImpl> createDataSet(const QString& guid = "") const override;
    void setTreeMetaDataRaw(QJsonObject jsonString);
    void setTreeMetaLeafNamesRaw(QStringList jsonString);
    QJsonObject& getTreeMetaDataRaw();
    QStringList& getTreeMetaLeafNamesRaw();
    QString& getTreeMetaPropertyNamesRaw();

public: // Serialization

    /**
     * Load widget action from variant
     * @param Variant representation of the widget action
     */
    void fromVariantMap(const QVariantMap& variantMap) override;

    /**
     * Save widget action to variant
     * @return Variant representation of the widget action
     */
    QVariantMap toVariantMap() const override;

private:
    QJsonObject _data;
    QStringList _leafNames;
    QString _propertyNames;
};

class  XSCTREEMETADATA_EXPORT XSCTreeMeta : public mv::DatasetImpl
{
public:
    XSCTreeMeta(QString dataName, bool mayUnderive = true, const QString& guid = "") :
        mv::DatasetImpl(dataName, mayUnderive, guid)
    {
        
    }

    ~XSCTreeMeta() override { }
    void init() override;
    Dataset<mv::DatasetImpl> copy() const override
    {
        auto text = new XSCTreeMeta(getRawDataName());

        text->setText(this->text());
        text->indices = indices;

        return text;
    }

    /**
     * Create subset from the current selection and specify where the subset will be placed in the data hierarchy
     * @param guiName Name of the subset in the GUI
     * @param parentDataSet Smart pointer to parent dataset in the data hierarchy (default is below the set)
     * @param visible Whether the subset will be visible in the UI
     * @return Smart pointer to the created subset
     */
    Dataset<mv::DatasetImpl> createSubsetFromSelection(const QString& guiName, const Dataset<DatasetImpl>& parentDataSet = Dataset<mv::DatasetImpl>(), const bool& visible = true) const override
    {
        return mv::data().createSubsetFromSelection(getSelection(), toSmartPointer(), guiName, parentDataSet, visible);
    }

public: // Selection

    /**
     * Get selection
     * @return Selection indices
     */
    std::vector<std::uint32_t>& getSelectionIndices() override;

    /**
     * Select by indices
     * @param indices Selection indices
     */
    void setSelectionIndices(const std::vector<std::uint32_t>& indices) override;

    /** Determines whether items can be selected */
    bool canSelect() const override;

    /** Determines whether all items can be selected */
    bool canSelectAll() const override;

    /** Determines whether there are any items which can be deselected */
    bool canSelectNone() const override;

    /** Determines whether the item selection can be inverted (more than one) */
    bool canSelectInvert() const override;

    /** Select all items */
    void selectAll() override;

    /** De-select all items */
    void selectNone() override;

    /** Invert item selection */
    void selectInvert() override;
    void setTreeMetaData(QJsonObject jsonString);
    void setTreeMetaLeafNames(QStringList jsonString);
    QJsonObject& getTreeMetaData();
    QStringList& getTreeMetaLeafNames();
    QString& getTreeMetaPropertyNames();


public: // Serialization

    /**
     * Load widget action from variant
     * @param Variant representation of the widget action
     */
    void fromVariantMap(const QVariantMap& variantMap) override;

    /**
        * Save widget action to variant
        * @return Variant representation of the widget action
        */
    QVariantMap toVariantMap() const override;


    QSharedPointer<InfoAction>      _infoAction;
    std::vector<unsigned int> indices;



};

// =============================================================================
// Factory
// =============================================================================

class XSCTreeMetaDataFactory : public RawDataFactory
{
    Q_INTERFACES(mv::plugin::RawDataFactory mv::plugin::PluginFactory)
        Q_OBJECT
        Q_PLUGIN_METADATA(IID   "nl.BioVault.XSCTreeMetaData"
            FILE  "XSCTreeMetaData.json")

public:
    XSCTreeMetaDataFactory(void);
    ~XSCTreeMetaDataFactory(void) override {}

    mv::plugin::RawData* produce() override;
};
