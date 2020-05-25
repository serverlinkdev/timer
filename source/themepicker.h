#ifndef THEMEPICKER_H
#define THEMEPICKER_H

#include <QObject>
#include <QDialog>
#include <QComboBox>

class ThemePicker : public QDialog
{
    Q_OBJECT
public:
    explicit ThemePicker(QDialog *parent = nullptr);
    ~ThemePicker();

signals:
    void getCssStylesList();
    void cssStylesStringListUpdated(QStringList cssStylesStringList);
    void setCssStyleStyleSheet(QString themeName);

public slots:
    void recvCssStyles(QStringList cssStylesList);

private:
    QComboBox *box = nullptr;
    void populateCombobox(QStringList cssStylesStringList);
    QStringList cssStylesStringList;
    void setCssStylesList(QStringList cssStylesList);

    // QWidget interface
protected:
    void showEvent(QShowEvent *event) override;
};

#endif // THEMEPICKER_H
