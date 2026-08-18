#pragma once

#include "Dataset.h"
#include "XSCTreeMetaData.h"


#include "actions/StringAction.h"
#include "event/EventListener.h"

namespace mv {
    class CoreInterface;
}

using namespace mv;
using namespace mv::gui;
using namespace mv::util;


class InfoAction : public GroupAction
{
    Q_OBJECT

public:


    InfoAction(QObject* parent, XSCTreeMeta& clusters);

public: // Action getters

    StringAction& getTreeMetaInfoAction() { return _treeMetaInfoAction; }
    StringAction& getLeafInfoAction() { return _leafInfoAction; }
    StringAction& getPropertyInfoAction() { return _propertyInfoAction; }

protected:
    Dataset<XSCTreeMeta>       _clusters;                  /** Clusters dataset smart pointer */
    StringAction    _treeMetaInfoAction;
    StringAction    _leafInfoAction;
    StringAction    _propertyInfoAction;

    mv::EventListener     _eventListener;             /** Listen to HDPS events */
};