#ifndef HOME_H
#define HOME_H

#include <QWidget>

namespace Ui {
class Home;
}
class MainWindow;

class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(MainWindow *parent = nullptr);
    ~Home();

signals:
    void randomModeSelected();
    void levelModeSelected();

private slots:
    void on_levelButton_clicked();
    void on_randomButton_clicked();

private:
    Ui::Home *ui;
    MainWindow* mainWindow;
};

#endif // HOME_H
