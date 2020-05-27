#ifndef THEMEPICKER_H
#define THEMEPICKER_H

#include <QComboBox>
#include <QDialog>
#include <QObject>

class ThemePicker : public QDialog
{
    Q_OBJECT
public:
    explicit ThemePicker(QDialog *parent = nullptr);
    ~ThemePicker();

signals:
    void getThemesList();
    void setCssStyleStyleSheet(QString themeName);

public slots:
    void onSendThemesList(QStringList cssStylesList);

private:
    QComboBox *box = nullptr;
    void populateCombobox(QStringList cssStylesList);
    void onComboBoxItemActivated();

protected:
    void showEvent(QShowEvent *event) override;
};

#endif // THEMEPICKER_H
