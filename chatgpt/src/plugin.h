// Copyright (C) 2023-2024 Manuel Schneider

#pragma once
#include <albert/globalqueryhandler.h>
#include <albert/extensionplugin.h>
#include <QObject>
#include <memory>

class Plugin : public albert::ExtensionPlugin,
               public albert::GlobalQueryHandler
{
    ALBERT_PLUGIN

public:

    Plugin();

    QString defaultTrigger() const override;
    QString synopsis() const override;
    void handleTriggerQuery(albert::Query*) override;
    std::vector<albert::RankItem> handleGlobalQuery(const albert::Query*) override;
    QWidget* buildConfigWidget() override;

private:

    QString iconPath;
    static const QStringList icon_urls;

};
