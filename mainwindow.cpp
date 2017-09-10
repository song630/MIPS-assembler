#include <utility>
#include "DIS/binary_instruction.h" // self-defined
#include "DIS/read_in_instr.h"
#include "DIS/table_drive.h"

#include "ASSEM/instruction.h"
#include "ASSEM/assemble.h"
#include "ASSEM/table_driven.h"

#include "mainwindow.h"
// #include "ui_mainwindow.h"
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <iostream> // C++
#include <stdio.h>
using namespace std;

extern char *open_path;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)
{
    open_action = new QAction(QIcon(":\\images\\file-open"), tr("&Open..."), this); // open
    open_action->setShortcuts(QKeySequence::Open);
    open_action->setStatusTip(tr("Open an existing file"));

    save_action = new QAction(QIcon(":\\images\\file-save"), tr("&Save..."), this); // save
    save_action->setShortcuts(QKeySequence::Save);
    save_action->setStatusTip(tr("Save a new file"));

    dis_action = new QAction(QIcon(":/images/file-save"), tr("&disassemble"), this);
    dis_action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D)); // ctrl + D: disassemble
    dis_action->setStatusTip(tr("Disassemble the code"));

    assem_action = new QAction(QIcon(":/images/file-save"), tr("assemble"), this);
    assem_action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A)); // ctrl + A: assemble
    assem_action->setStatusTip(tr("Assemble the code"));

    QMenu *column1 = menuBar()->addMenu(tr("&File")); // add open and save to menu bar
    column1->addAction(open_action);
    column1->addAction(save_action);
    QMenu *column2 = menuBar()->addMenu(tr("&Run"));
    column2->addAction(assem_action);
    column2->addAction(dis_action);

    QToolBar *tool_bar = addToolBar(tr("&File"));
    tool_bar->addAction(dis_action);
    tool_bar->addAction(assem_action);

    text_edit = new QTextEdit(this);
    setCentralWidget(text_edit); // set an element as central component

    connect(open_action, &QAction::triggered, this, &MainWindow::open_file);
    connect(save_action, &QAction::triggered, this, &MainWindow::save_file);
    connect(dis_action, &QAction::triggered, this, &MainWindow::on_clicked_dis);
    connect(assem_action, &QAction::triggered, this, &MainWindow::on_clicked_assem);
}

void MainWindow::open_file()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open file"), ".", tr("Text Files(*.txt *.coe *.ASM *.bin)")); // open a window, return a string
    if(!path.isEmpty()) // if the path is valid
    {
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) // if the file can be opened
        {
            QMessageBox::warning(this, tr("Read File"), tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream in(&file);
        text_edit->setText(in.readAll());
        file.close();
    }
    else
        QMessageBox::warning(this, tr("Path"), tr("You did not select any file."));
}

void MainWindow::save_file()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Open File"), ".", tr("Text Files(*.txt *.coe *.ASM *.bin)"));
    if(!path.isEmpty()) // invalid path
    {
        QFile file(path);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) // fail to open the file
        {
            QMessageBox::warning(this, tr("Write File"), tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
        out << text_edit->toPlainText();
        file.close();
    }
    else
        QMessageBox::warning(this, tr("Path"), tr("You did not select any file."));
}

void MainWindow::on_clicked_dis()
{
    QString path = "D://temp.txt"; // first put the current content of TextEdit to a temp file
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) // fail to open the file
    {
        QMessageBox::warning(this, tr("Write File"), tr("Cannot open file:\n%1").arg(path));
        return;
    }
    QTextStream out(&file);
    out << text_edit->toPlainText();
    file.close();

    char *open_path = path.toLatin1().data(); // convert "QString" to "char*" in C
    try {
        get_instructions(open_path);
    } catch (Exception& e) {
        e.print_error();
    }
    map_table object;
    object.init_all_maps();
    try {
        object.main_process();
    } catch (Exception& e) {
        e.print_error();
    }
    open_path = path.toLatin1().data(); // ===== must be added,
    // because after the function get_instructions(open_path), open_path is modified.
    write_instructions(open_path); // when save_file() is executed, we get a valid open_path

    index = "00";
    instr_set.clear();
    hex_table.clear();
    label_set.clear();

    // open the preview of the disassembled text
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) // if the file can be opened
    {
        QMessageBox::warning(this, tr("Read File"), tr("Cannot open file:\n%1").arg(path));
        return;
    }
    QTextStream in(&file);
    text_edit->setText(in.readAll());
    file.close();

    remove(open_path); // remove the temp file
}

void MainWindow::on_clicked_assem()
{
    QString path = "D://temp.txt"; // first put the current content of TextEdit to a temp file
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) // fail to open the file
    {
        QMessageBox::warning(this, tr("Write File"), tr("Cannot open file:\n%1").arg(path));
        return;
    }
    QTextStream out(&file);
    out << text_edit->toPlainText();
    file.close();

    char *open_path = path.toLatin1().data(); // begin to assemble
    try {
        get_Instructions(open_path);
    } catch (Exception& e) {
        e.print_error();
    }
    init_reg_map();
    table_process object;
    object.init();
    try {
        object.assem_all_to_bin();
    } catch (Exception& e) {
        e.print_error();
    }
    open_path = path.toLatin1().data();
    write_to_file(open_path);

    L_set.clear();
    I_set.clear();
    R_map.clear();

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) // if the file can be opened
    {
        QMessageBox::warning(this, tr("Read File"), tr("Cannot open file:\n%1").arg(path));
        return;
    }
    QTextStream in(&file);
    text_edit->setText(in.readAll());
    file.close();

    remove(open_path); // remove the temp file
}

MainWindow::~MainWindow()
{

}
