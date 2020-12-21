#pragma once

#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QMainWindow>
#include <QMessageBox>
#include <QModelIndex>
#include <QSizePolicy>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringList>
#include <QStringListModel>
#include <QTreeView>
#include <algorithm>
#include <optional>
#include <iostream>
#include <vector>
#include "App.h"
#include "structs/FileMeta.h"

#include <QFileDialog>
#include <QDesktopServices>
#include <QInputDialog>
#include <QFile>
#include "AppExceptions.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public QStandardItem {
 Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

 private:
  void parseVectorFiles();
  static std::string getFilePath(const FileMeta &file);
  std::string getAbsoluteFilePath(const FileMeta &file);
  FileMeta &getFile();
  int getFileId();
  static bool compareFileMeta(const FileMeta &x, const FileMeta &y);

 private:
  void downloadFileCallbackOk();
  void downloadFileCallbackError(const std::string &msg);
  void refreshCallbackOk();
  void refreshCallbackError(const std::string &msg);
  void uploadFileCallbackOk();
  void uploadFileCallbackError(const std::string &msg);

 private:
  void printMsgBox();
  QString _msg;

 signals:
  void printMsgBoxSignal();

 private slots:
  void slotCustomMenuRequested(QPoint pos);
  void open_file();
  void rename_file();
  void download_on_device();
  void delete_from_device();
  void delete_from_cloud();
  void view_history_file();
  void view_properties();
  void on_treeView_clicked(const QModelIndex &index);
  void onBtnAddFile();
  void onBtnRefresh();
  static void onBtnAbout();

 private:
  Ui::MainWindow *ui;
  QStandardItemModel _cloudModel;
  std::vector<FileMeta> _filesInfo;
  App _app;
};
