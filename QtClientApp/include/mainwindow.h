#pragma once

#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QHBoxLayout>
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QModelIndex>
#include <QMovie>
#include <QSettings>
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
#include "AppExceptions.h"
#include "structs/FileMeta.h"
#include <QShortcut>

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
  void updateFiles();
  void startLoadingLabel();
  void stopLoadingLabel();

 private:
  void callbackOk(const std::string &msg);
  void callbackError(const std::string &msg);

 private:
  void printMsgBox();
  QString _msg;

  void printInfoBox(const QString &title, const QString &text);

 signals:
  void printMsgBoxSignal();

 private slots:
  void slotShortcutCMDR();

  void slotCustomMenuRequested(QPoint pos);
  void changeDirectory();
  void open_file();
  void save_file();
  void rename_file();
  void download_on_device();
  void delete_from_device();
  void delete_from_cloud();
  void view_history_file();
  void view_properties();
  void onBtnAddFile();
  void onBtnSettings();
  void onBtnRefresh();
  static void onBtnAbout();

 private:
  Ui::MainWindow *ui;
  QShortcut *keyCMDR;
  QWidget *_settingsForm;
  QMovie *_movie;
  QStandardItemModel _cloudModel;
  std::vector<FileMeta> _filesInfo;
  App _app;
};
