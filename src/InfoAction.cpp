#include "QtTooltipKit.h"
#include "InfoAction.h"

#include "event/Event.h"

using namespace mv;
using namespace mv::gui;

InfoAction::InfoAction(QObject* parent, XSCTreeMeta& clusters) :
    GroupAction(parent, "Group", true),
    _clusters(&clusters),
    _treeMetaInfoAction(this, "XSCTreeMeta info"),
    _leafInfoAction(this, "Leaf info"),
    _propertyInfoAction(this, "Property info")
{
    setText("XSCTreeMeta JSON Info");


    _treeMetaInfoAction.setIconByName("code");
    qttooltipkit::set(_treeMetaInfoAction, tr("Metadata records"), tr("Inspect the source JSON for species and their trait values."));
    _treeMetaInfoAction.setDefaultWidgetFlags(StringAction::TextEdit);
    _leafInfoAction.setIconByName("leaf");
    qttooltipkit::set(_leafInfoAction, tr("Species identifiers"), tr("Inspect the names used to match metadata rows to the reference tree."));
    _leafInfoAction.setDefaultWidgetFlags(StringAction::TextEdit);
    _propertyInfoAction.setIconByName("list");
    qttooltipkit::set(_propertyInfoAction, tr("Available traits"), tr("Inspect the metadata fields available for species comparisons."));
    _propertyInfoAction.setDefaultWidgetFlags(StringAction::TextEdit);

    addAction(&_leafInfoAction, -1, [this](WidgetAction* action, QWidget* widget) -> void {
        auto textEdit = widget->findChild<QTextEdit*>("LineEdit");

        if (textEdit)
            textEdit->setReadOnly(true);

        });
    addAction(&_treeMetaInfoAction, -1, [this](WidgetAction* action, QWidget* widget) -> void {
        auto textEdit = widget->findChild<QTextEdit*>("LineEdit");

        if (textEdit)
            textEdit->setReadOnly(true);

        });
    
    addAction(&_propertyInfoAction, -1, [this](WidgetAction* action, QWidget* widget) -> void {
        auto textEdit = widget->findChild<QTextEdit*>("LineEdit");

        if (textEdit)
            textEdit->setReadOnly(true);

        });
    
    const auto updateActions = [this]() -> void {
        if (!_clusters.isValid())
            return;


        _treeMetaInfoAction.setString(QJsonDocument(_clusters->getTreeMetaData()).toJson());

        QString textleaf = _clusters->getTreeMetaLeafNames().join("\n");


        QString textProperty = _clusters->getTreeMetaPropertyNames();

        _leafInfoAction.setString(textleaf);
        _propertyInfoAction.setString(textProperty);

        //qDebug() << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";

        //qDebug() << QJsonDocument(_clusters->getTreeMetaData()).toJson();
        //qDebug() << "Species names: " << textleaf ;
        //qDebug() << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";

    };

    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetAdded));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetDataChanged));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetDataSelectionChanged));


    _eventListener.registerDataEventByType(XSCTreeMetaType, [this, updateActions](mv::DatasetEvent* dataEvent) {
        if (!_clusters.isValid())
            return;

        if (dataEvent->getDataset() != _clusters)
            return;

        switch (dataEvent->getType()) {
            case EventType::DatasetAdded:
            case EventType::DatasetDataChanged:
            case EventType::DatasetDataSelectionChanged:
            {
                updateActions();
                break;
            }

            default:
                break;
        }
    });

    updateActions();
}