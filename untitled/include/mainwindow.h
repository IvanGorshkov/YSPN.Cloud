#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct FileMeta {
  int fileId;
  int version;
  std::string fileName;
  std::string fileExtension;
  std::string filePath;
  int fileSize;
  int chunksCount;
  bool isDownload;
  bool isDeleted;
  bool isCurrent;
  std::string updateDate;
  std::string createDate;

  std::string getFullPath() const {
      return filePath + fileName + "." + fileExtension;
  }
};

class MainWindow : public QMainWindow, public QStandardItem{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QModelIndex _selectedFileIndex;
    QStandardItemModel _cloudModel;
    std::vector<FileMeta> _filesInfo;
    void parseVectorFiles();
private slots:
    static void onBtnAddFile();
    static void onBtnRefresh();
    static void onBtnAbout();
    void slotCustomMenuRequested(QPoint pos);
    void open_file();
    void rename_file();
    void download_on_device();
    void delete_from_device();
    void delete_from_cloud();
    void view_history_file();
    void view_properties();
    void on_treeView_clicked(const QModelIndex &index);
};
#endif // MAINWINDOW_H
