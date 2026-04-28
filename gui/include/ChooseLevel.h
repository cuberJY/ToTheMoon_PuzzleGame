#ifndef CHOOSELEVEL_H
#define CHOOSELEVEL_H

#include "LevelMode.h"
#include <QWidget>
#include <QPushButton>

namespace Ui {
class ChooseLevel;
}
class MainWindow;

class ChooseLevel : public QWidget
{
    Q_OBJECT

public:
    void refresh();
    explicit ChooseLevel(MainWindow *parent = nullptr);
    ~ChooseLevel();

signals:
    void levelSelected(int level);

private slots:
    void onLevelButtonClicked(int level);

private:
    Ui::ChooseLevel *ui;
    MainWindow* mainWindow;
};

#endif // CHOOSELEVEL_H
