#ifndef AUTH_WINDOW_H
#define AUTH_WINDOW_H

#include <QDialog>

namespace Ui {
class auth_window;
}

class auth_window : public QDialog
{
    Q_OBJECT

public:
    explicit auth_window(QWidget *parent = nullptr);
    ~auth_window() override;
    QString GetLogin();
    QString GetPass();

signals:
    void login_button_clicked();
    void register_button_clicked();

private slots:
    void on_inp_login_textEdited(const QString &arg1);

    void on_inp_password_textEdited(const QString &arg1);

    void on_btn_login_clicked();

    void on_btn_register_clicked();

private:
    Ui::auth_window *ui;
    QString _username;
    QString _userpass;
};

#endif // AUTH_WINDOW_H
