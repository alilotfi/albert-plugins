// Copyright (c) 2023-2024 Manuel Schneider

#include "plugin.h"
#include "ui_configwidget.h"
#include <QSettings>
#include <QThread>
#include <albert/logging.h>
#include <albert/standarditem.h>
#include <albert/util.h>
ALBERT_LOGGING_CATEGORY("chatgpt")
using namespace albert;
using namespace std;

namespace
{
    const char* CFG_OPEN_AI_KEY = "open_ai_key";
    const char* CFG_OPEN_AI_MODEL = "open_ai_model";
    const char* DEF_OPEN_AI_MODEL = "gpt-4o-mini";
}

const QStringList Plugin::icon_urls = {":chatgpt"};

Plugin::Plugin()
{
    auto s = settings();

}

QString Plugin::defaultTrigger() const
{
    return "?";
}

QString Plugin::synopsis() const
{
    static const auto tr_me = tr("<Ask anything>");
    return tr_me;
}

QWidget* Plugin::buildConfigWidget()
{
    auto* widget = new QWidget;
    Ui::ConfigWidget ui;
    ui.setupUi(widget);
    auto s = settings();

    // API Key
    ui.apiKeyLineEdit->setText(s->value(CFG_OPEN_AI_KEY).toString());
    connect(ui.apiKeyLineEdit, &QLineEdit::textChanged, [this](const QString& text)
        { settings()->setValue(CFG_OPEN_AI_KEY, text); });

    // Model
    ui.modelLineEdit->setText(s->value(CFG_OPEN_AI_MODEL, DEF_OPEN_AI_MODEL).toString());
    connect(ui.modelLineEdit, &QLineEdit::textChanged, [this](const QString& text) {
        settings()->setValue(CFG_OPEN_AI_MODEL, text);
    });

    return widget;
}

vector<RankItem> Plugin::handleGlobalQuery(const Query* query)
{
    vector<RankItem> results;
    return results;
}

void Plugin::handleTriggerQuery(Query* query)
{
    auto trimmed = query->string().trimmed();
    if (trimmed.isEmpty())
        return;
}
