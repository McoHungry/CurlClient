#ifndef LOGINPANEL_H
#define LOGINPANEL_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

class LoginPanel : public QWidget
{
    Q_OBJECT
public:
    explicit LoginPanel(QWidget *parent = 0);

    QLineEdit* _username;
    QLineEdit* _password;

signals:

public slots:
    void slotButtonClicked();
};

#endif // LOGINPANEL_H
