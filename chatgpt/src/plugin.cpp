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
    const char* CFG_PROMPT = "PROMPT";
    const char* DEF_PROMPT =
        "Your response must have a very short title, separated by a new line from the answer to the question. "
        "The answer should be very concise and straight to the point, with no extra explanation or long descriptions. "
        "With that in mind, respond to this:";
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
    ui.apiKeyTextEdit->setPlainText(s->value(CFG_OPEN_AI_KEY).toString());
    connect(ui.apiKeyTextEdit, &QPlainTextEdit::textChanged, [this, ui]()
    {
        settings()->setValue(CFG_OPEN_AI_KEY, ui.apiKeyTextEdit->toPlainText());
    });

    // Model
    ui.modelLineEdit->setText(s->value(CFG_OPEN_AI_MODEL, DEF_OPEN_AI_MODEL).toString());
    connect(ui.modelLineEdit, &QLineEdit::textChanged, [this](const QString& text)
    {
        settings()->setValue(CFG_OPEN_AI_MODEL, text);
    });

    // Prompt
    ui.promptTextEdit->setPlainText(s->value(CFG_PROMPT, DEF_PROMPT).toString());
    connect(ui.promptTextEdit, &QPlainTextEdit::textChanged, [this, ui]()
    {
        settings()->setValue(CFG_PROMPT, ui.promptTextEdit->toPlainText());
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
    const auto question = query->string().trimmed();
    if (question.isEmpty())
        return;

    if (not shouldAsk(question))
    {
        query->add(buildHint());
        return;
    }

        auto [title, answer] = ask(question);
    if (!query->isValid())
        return;

    auto items = buildItems(title, answer);
    if (!query->isValid())
        return;
    for (const auto& item : items) { query->add(item); }
}

bool Plugin::shouldAsk(const QString& question)
{
    // Return true if last character of question is a question mark
    return question.right(1) == "?";
}

std::pair<QString, QString> Plugin::ask(QString question)
{
    this_thread::sleep_for(chrono::milliseconds(100));
    return std::pair("title", question);
}

std::shared_ptr<albert::Item> Plugin::buildHint()
{
    return StandardItem::make("hint", "Add a ? at the end to ask ChatGPT.", "", icon_urls);
}

std::vector<std::shared_ptr<Item>> Plugin::buildItems(const QString& title, const QString& answer)
{
    vector<std::shared_ptr<Item>> items;
    const auto fullResponse = title + "\n" + answer;

    items.push_back(
        StandardItem::make(
            "response", title, answer, icon_urls,
            {
                {"copy", "Copy response to clipboard", [=]() { setClipboardText(fullResponse); }},
            }
        )
    );

    items.push_back(
        StandardItem::make(
            "read",
            "Read aloud the response",
            "",
            icon_urls,
            {
                {"read", "Read aloud the response", [=]() { setClipboardText(fullResponse); }},
            }
        )
    );
    items.push_back(
        StandardItem::make(
            "open",
            "Ask ChatGPT in a new thread.",
            "",
            icon_urls,
            {
                {"thread", "Ask ChatGPT in a new thread.", [=]() { setClipboardText(fullResponse); }},
            }
        )
    );
    return items;
}
