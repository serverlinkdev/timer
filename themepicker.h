#ifndef THEMEPICKER_H
#define THEMEPICKER_H

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QObject>
#include <QSpacerItem>
#include <QVBoxLayout>

class ThemePicker : public QDialog
{
    Q_OBJECT
public:
    explicit ThemePicker(QDialog *parent = nullptr);
    ~ThemePicker();

signals:
    void getCurrentTheme();
    void getThemesList();
    void setCssStyleStyleSheet(QString themeName);

public slots:
    void onCurrentTheme(QString theme);
    void onSendThemesList(QStringList cssStylesList);

private:
    QLabel *lbl = nullptr;
    QComboBox *box = nullptr;
    QSpacerItem *spacer = nullptr;
    QDialogButtonBox *btn = nullptr;
    QVBoxLayout *layout = nullptr;

    void populateCombobox(QStringList cssStylesList);
    void onComboBoxItemActivated();

protected:
    void showEvent(QShowEvent *event) override;
};

#endif // THEMEPICKER_H
