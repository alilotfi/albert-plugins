// Copyright (c) 2022 Manuel Schneider

#pragma once
#include <QString>
#include <vector>
#include "albert.h"

class Plugin:
        public albert::ExtensionPlugin,
        public albert::IndexQueryHandler,
        public albert::ConfigWidgetProvider
{
    Q_OBJECT ALBERT_PLUGIN
public:
    Plugin();
    std::vector<albert::IndexItem> indexItems() const override;
    QWidget* buildConfigWidget() override;
private:

    struct SysItem {
        std::shared_ptr<albert::StandardItem> item;
        QString command;
        QStringList aliases;
    };
    std::vector<SysItem> items_;
};