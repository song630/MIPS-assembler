#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void open_file();
    void save_file();
    void on_clicked_dis();
    void on_clicked_assem();
    ~MainWindow();
    friend void get_instructions(char *open_path);

private:
    // Ui::MainWindow *ui;
    QAction *open_action;
    QAction *save_action;
    QAction *assem_action;
    QAction *dis_action;
    QTextEdit *text_edit;
};

#endif // MAINWINDOW_H
