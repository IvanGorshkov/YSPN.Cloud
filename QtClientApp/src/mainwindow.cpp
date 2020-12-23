#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"
#include <functional>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      _app(std::bind(&MainWindow::callbackOk, this, std::placeholders::_1),
           std::bind(&MainWindow::callbackError, this, std::placeholders::_1)) {
  ui->setupUi(this);

  ui->centralwidget->setMinimumSize(800, 550);
  ui->centralwidget->setWindowIconText("YSNP.Cloud");
  ui->treeView->setAnimated(true);
  ui->treeView->setHeaderHidden(true);
  ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
  ui->lb_loading->setHidden(true);

  updateFiles();

  connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));
  connect(ui->btn_refresh, SIGNAL(clicked(bool)), this, SLOT(onBtnRefresh()));
  connect(ui->btn_AddFile, SIGNAL(clicked(bool)), this, SLOT(onBtnAddFile()));
  connect(ui->btn_About, SIGNAL(clicked(bool)), this, SLOT(onBtnAbout()));
  connect(ui->btn_Settings, SIGNAL(clicked(bool)), this, SLOT(onBtnSettings()));
  connect(this, &MainWindow::printMsgBoxSignal, this, &MainWindow::printMsgBox);

  keyCMDR = new QShortcut(this);
  keyCMDR->setKey(Qt::Key_R);
  connect(keyCMDR, SIGNAL(activated()), this, SLOT(slotShortcutCMDR()));
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::printMsgBox() {
  QMessageBox msgBox;
  QString mes = _msg;
  msgBox.setText(mes);
  if (msgBox.exec())
    stopLoadingLabel();
}

void MainWindow::printInfoBox(const QString &title, const QString &text) {
  QMessageBox::information(this, title, text);
}

FileMeta &MainWindow::getFile() {
  auto idFile = getFileId();
  auto fileMeta = std::find_if(_filesInfo.begin(), _filesInfo.end(), [&idFile](const FileMeta &meta) {
    return meta.fileId == idFile;
  });
  return *fileMeta;
}

int MainWindow::getFileId() {
  return _cloudModel.itemFromIndex(ui->treeView->currentIndex())->toolTip().toInt();
}

std::string MainWindow::getAbsoluteFilePath(const FileMeta &file) {
  return _app.GetSyncFolder() + '/' + getFilePath(file);
}

std::string MainWindow::getFilePath(const FileMeta &file) {
  return (file.filePath.empty() ? file.filePath : file.filePath + '/') + file.fileName + file.fileExtension;
}

bool MainWindow::compareFileMeta(const FileMeta &x, const FileMeta &y) {
  return getFilePath(x) < getFilePath(y);
}

void MainWindow::updateFiles() {
  startLoadingLabel();
  _cloudModel.clear();
  _filesInfo = _app.GetFiles();
  parseVectorFiles();
  ui->treeView->setModel(&_cloudModel);
  stopLoadingLabel();
}

void MainWindow::callbackOk(const std::string &msg) {
  _msg = QString::fromStdString(msg);
  Q_EMIT printMsgBoxSignal();
  updateFiles();
}

void MainWindow::callbackError(const std::string &msg) {
  _msg = QString::fromStdString(msg);
  Q_EMIT printMsgBoxSignal();
}

void MainWindow::parseVectorFiles() {
  auto *root = new QStandardItem;
  root->setText("Cloud YSNP");
  root->setFlags(Qt::ItemIsDropEnabled);
  root->setEnabled(true);
  root->setIcon(QIcon("../icons/cloud.png"));
  _cloudModel.setItem(0, 0, root);
  QModelIndex rootIndex = _cloudModel.index(0, 0);

  QList<QString> files_paths;
  for (auto &file : _filesInfo) {
    std::string full_path = getFilePath(file);
    QString path = QString(full_path.c_str());
    if (path[0] == '/') {
      path.remove(0, 1);
    }
    files_paths.append(path);
  }
  std::sort(files_paths.begin(), files_paths.end());
  std::sort(_filesInfo.begin(), _filesInfo.end(), compareFileMeta);

  for (int i = 0; i < files_paths.size(); i++) {
    QStringList listPath = files_paths[i].split('/');
    QModelIndex parentIndex = rootIndex;
    std::string filePath = "";
    for (int j = 0; j < listPath.size(); ++j) {
      filePath.append("/" + listPath[j].toStdString());
      if (listPath[j].contains(".")) {
        FileMeta fileInfoFromVector = _filesInfo[i];
        if (getFilePath(_filesInfo[i]) != filePath) {
          for (auto &p : _filesInfo) {
            if (getFilePath(p) == filePath) {
              fileInfoFromVector = p;
              break;
            }
          }
        }
        auto *insertFile = new QStandardItem;
        insertFile->setText(listPath[j]);
        insertFile->setWhatsThis(listPath[j]);
        insertFile->setToolTip(QString::number(fileInfoFromVector.fileId));
        insertFile->setFlags(Qt::ItemIsEnabled);
        if (fileInfoFromVector.fileExtension == "txt")
          if (fileInfoFromVector.isDownload)
            insertFile->setIcon(QIcon("../icons/txt_download.png"));
          else
            insertFile->setIcon(QIcon("../icons/txt.png"));
        else {
          if ((fileInfoFromVector.fileExtension == "pptx") || (fileInfoFromVector.fileExtension == "ppt"))
            if (fileInfoFromVector.isDownload)
              insertFile->setIcon(QIcon("../icons/pptx_download.png"));
            else
              insertFile->setIcon(QIcon("../icons/pptx.png"));
          else {
            if ((fileInfoFromVector.fileExtension == "docx") || (fileInfoFromVector.fileExtension == "doc"))
              if (fileInfoFromVector.isDownload)
                insertFile->setIcon(QIcon("../icons/docx_download.png"));
              else
                insertFile->setIcon(QIcon("../icons/docx.png"));
            else {
              if (fileInfoFromVector.fileExtension == "html")
                if (fileInfoFromVector.isDownload)
                  insertFile->setIcon(QIcon("../icons/html_download.png"));
                else
                  insertFile->setIcon(QIcon("../icons/html.png"));
              else {
                if (fileInfoFromVector.fileExtension == "pdf")
                  if (fileInfoFromVector.isDownload)
                    insertFile->setIcon(QIcon("../icons/pdf_download.png"));
                  else
                    insertFile->setIcon(QIcon("../icons/pdf.png"));
                else {
                  if (fileInfoFromVector.fileExtension == "py")
                    if (fileInfoFromVector.isDownload)
                      insertFile->setIcon(QIcon("../icons/py_download.png"));
                    else
                      insertFile->setIcon(QIcon("../icons/py.png"));
                  else {
                    if (fileInfoFromVector.fileExtension == "cpp")
                      if (fileInfoFromVector.isDownload)
                        insertFile->setIcon(QIcon("../icons/cpp_download.png"));
                      else
                        insertFile->setIcon(QIcon("../icons/cpp.png"));
                    else {
                      if ((fileInfoFromVector.fileExtension == "jpg") ||
                          (fileInfoFromVector.fileExtension == "jpeg") ||
                          (fileInfoFromVector.fileExtension == "png") ||
                          (fileInfoFromVector.fileExtension == "bmp") ||
                          (fileInfoFromVector.fileExtension == "svg") ||
                          (fileInfoFromVector.fileExtension == "webp"))
                        if (fileInfoFromVector.isDownload)
                          insertFile->setIcon(QIcon("../icons/image_download.png"));
                        else
                          insertFile->setIcon(QIcon("../icons/image.png"));
                      else {
                        if (fileInfoFromVector.isDownload)
                          insertFile->setIcon(QIcon("../icons/file_download.png"));
                        else
                          insertFile->setIcon(QIcon("../icons/file.png"));
                      }
                    }
                  }
                }
              }
            }
          }
        }
        auto parent = _cloudModel.itemFromIndex(parentIndex);
        parent->appendRow(insertFile);
        _cloudModel.setItem(parent->row(), parent);
      } else {
        if (_cloudModel.hasChildren(parentIndex)) {
          auto parent = _cloudModel.itemFromIndex(parentIndex);

          bool exist = false;
          for (int k = 0; k < parent->rowCount(); k++) {
            auto child = parent->child(k);
            if (child->whatsThis() == listPath[j]) {
              exist = true;
              parentIndex = parentIndex.child(k, 0);
            }
          }
          if (!exist) {
            auto *insertFolder = new QStandardItem;
            insertFolder->setText(listPath[j]);
            insertFolder->setWhatsThis(listPath[j]);
            insertFolder->setIcon(QIcon("../icons/folder.png"));

            parent = _cloudModel.itemFromIndex(parentIndex);
            parent->appendRow(insertFolder);
            _cloudModel.setItem(parent->row(), parent);

            parentIndex = _cloudModel.indexFromItem(insertFolder);
          }
        } else {
          auto *insertFolder = new QStandardItem;
          insertFolder->setText(listPath[j]);
          insertFolder->setWhatsThis(listPath[j]);
          insertFolder->setIcon(QIcon("../icons/folder.png"));

          auto parent = _cloudModel.itemFromIndex(parentIndex);
          parent->appendRow(insertFolder);
          _cloudModel.setItem(parent->row(), parent);
          parentIndex = _cloudModel.indexFromItem(insertFolder);
        }
      }
    }
  }
}

void MainWindow::slotCustomMenuRequested(QPoint pos) {
  auto *menu = new QMenu(this);
  auto *openFile = new QAction(trUtf8("Открыть"), this);
  auto *saveFile = new QAction(trUtf8("Сохранить"), this);
  auto *renameFile = new QAction(trUtf8("Переименовать"), this);
  auto *downloadToDevice = new QAction(trUtf8("Скачать на устройство"), this);
  auto *deleteFromDevice = new QAction(trUtf8("Удалить с устройства"), this);
//  auto *deleteFromCloud = new QAction(trUtf8("Удалить из облака"), this);
//  auto *history = new QAction(trUtf8("История редактирования"), this);
  auto *properties = new QAction(trUtf8("Свойства"), this);

  connect(openFile, SIGNAL(triggered()), this, SLOT(open_file()));
  connect(saveFile, SIGNAL(triggered()), this, SLOT(save_file()));
  connect(renameFile, SIGNAL(triggered()), this, SLOT(rename_file()));
  connect(downloadToDevice, SIGNAL(triggered()), this, SLOT(download_on_device()));
  connect(deleteFromDevice, SIGNAL(triggered()), this, SLOT(delete_from_device()));
//  connect(deleteFromCloud, SIGNAL(triggered()), this, SLOT(delete_from_cloud()));
//  connect(history, SIGNAL(triggered()), this, SLOT(view_history_file()));
  connect(properties, SIGNAL(triggered()), this, SLOT(view_properties()));

  menu->addAction(openFile);
  menu->addAction(saveFile);
  menu->addAction(renameFile);
  menu->addAction(downloadToDevice);
  menu->addAction(deleteFromDevice);
//  menu->addAction(deleteFromCloud);
//  menu->addAction(history);
  menu->addAction(properties);

  if (ui->treeView->indexAt(pos).flags() == 32)
    menu->popup(ui->treeView->viewport()->mapToGlobal(pos));
}

void MainWindow::open_file() {
  auto fileMeta = getFile();

  if (fileMeta.isDownload) {
    QString path = "file://" + QString::fromStdString(getAbsoluteFilePath(fileMeta));
    QDesktopServices::openUrl(QUrl(path));
  } else {
    printInfoBox("Открытие файлы", "Файл не скачан на устройство");
  }
}

void MainWindow::save_file() {
  auto fileMeta = getFile();

  if (fileMeta.isDownload) {
    _app.ModifyFile(getAbsoluteFilePath(fileMeta));
  } else {
    printInfoBox("Сохранение", "Файл не скачан на устройство");
  }
}

void MainWindow::rename_file() {
  auto fileMeta = getFile();

  if (fileMeta.isDownload) {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Переименование"),
                                         tr("Имя файла:"), QLineEdit::Normal,
                                         QString::fromStdString(fileMeta.fileName), &ok);

    QString oldPath = QString::fromStdString(getAbsoluteFilePath(fileMeta));
    if (ok && !text.isEmpty()) {
      fileMeta.fileName = text.toStdString();
    } else {
      return;
    }
    QString newPath = QString::fromStdString(getAbsoluteFilePath(fileMeta));

    QFile::rename(oldPath, newPath);
    _app.RenameFile(oldPath.toStdString(), newPath.toStdString());
  } else {
    printInfoBox("Переименование", "Файл не скачан на устройство");
  }
}

void MainWindow::download_on_device() {
  auto fileMeta = getFile();

  if (!fileMeta.isDownload) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Скачивание", "Вы действительно хотите скачать файл?",
                                  QMessageBox::Cancel | QMessageBox::Ok);
    if (reply == QMessageBox::Cancel) {
      return;
    }

    _app.DownloadFile(fileMeta.fileId);

  } else {
    printInfoBox("Скачивание", "Файл уже скачан на устройство");
  }
}

void MainWindow::delete_from_device() {
  auto fileMeta = getFile();

  if (fileMeta.isDownload) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Удаление", "Вы действительно хотите удалить файл?",
                                  QMessageBox::Cancel | QMessageBox::Ok);

    if (reply == QMessageBox::Cancel) {
      return;
    }

    QString path = QString::fromStdString(getAbsoluteFilePath(fileMeta));

    QFile::remove(path);
    _app.DeleteFile(path.toStdString());
  } else {
    printInfoBox("Удаление", "Файл не скачан на устройство");
  }
}

void MainWindow::delete_from_cloud() {

}

void MainWindow::view_history_file() {

}

void MainWindow::view_properties() {
  auto fileMeta = getFile();

  if (fileMeta.isDownload) {
    QString actionsWithFile;
    QString isDownload = (fileMeta.isDownload) ? "да" : "нет";

    QString fileSizeName;
    QStringList sizeName = {"Байт", "Кб", "Мб", "Гб"};
    auto fileSize = fileMeta.fileSize;
    int i = 0;

    while (fileSize) {
      if (fileSize < 1024) {
        fileSizeName = sizeName[i];
        break;
      }

      fileSize /= 1024;
      i++;
    }

    QString size = "Размер файла: " + QString::number(fileSize) + ' ' + fileSizeName + "\n";
    QString version = "Версия файла: " + QString::number(fileMeta.version) + "\n";
    QString download = "Файл скачан на устройство: " + isDownload + "\n";
    QString dateUpdate = "Дата изменения: " + QString::fromStdString(fileMeta.updateDate) + "\n";
    QString dateCreate = "Дата создания: " + QString::fromStdString(fileMeta.createDate) + "\n";
    actionsWithFile.append(size);
    actionsWithFile.append(version);
    actionsWithFile.append(download);
    actionsWithFile.append(dateUpdate);
    actionsWithFile.append(dateCreate);

    QMessageBox msg;
    QFont f;
    f.setPointSize(16);
    msg.setWindowTitle("Свойства");
    msg.setText(actionsWithFile);
    msg.setFont(f);
    msg.exec();
  } else {
    printInfoBox("Свойства", "Файл не скачан на устройство");
  }
}

void MainWindow::onBtnSettings() {
  _settingsForm = new QWidget();
  _settingsForm->setAttribute(Qt::WA_DeleteOnClose, true);

  auto *changeDirLayout = new QHBoxLayout(_settingsForm);

  QFont f;
  f.setPointSize(14);
  f.setBold(true);
  auto *labelDir = new QLabel(_settingsForm);
  labelDir->setText("Директория сохранения: ");
  labelDir->setFont(f);

  f.setBold(false);
  auto *dirPath = new QLabel(_settingsForm);
  dirPath->setFont(f);
  dirPath->setText(QString::fromStdString(_app.GetSyncFolder()));

  auto *buttonChangeDir = new QPushButton(_settingsForm);
  connect(buttonChangeDir, SIGNAL(clicked(bool)), this, SLOT(changeDirectory()));
  buttonChangeDir->setText("Изменить");
  buttonChangeDir->setFont(f);

  changeDirLayout->addWidget(labelDir);
  changeDirLayout->addWidget(dirPath);
  changeDirLayout->addWidget(buttonChangeDir);
  changeDirLayout->setSpacing(5);
  changeDirLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  _settingsForm->setContentsMargins(0, 0, 0, 0);
  _settingsForm->setFixedSize(700, 200);
  _settingsForm->setWindowTitle("Настройки");
  _settingsForm->show();
}

void MainWindow::onBtnRefresh() {
  startLoadingLabel();
  _app.Refresh();
}

void MainWindow::onBtnAddFile() {
  QFileDialog dialog;
  dialog.setDirectory(QString::fromStdString(_app.GetSyncFolder()));
  dialog.setFileMode(QFileDialog::AnyFile);
  if (dialog.exec()) {
    startLoadingLabel();
    for (auto &filePath : dialog.selectedFiles()) {
      QFileInfo file(filePath);
      QString newPath = QString::fromStdString(_app.GetSyncFolder() + '/' + file.fileName().toStdString());
      QFile::copy(filePath, newPath);
      _app.CreateFile(newPath.toStdString());
    }
  }
}

void MainWindow::onBtnAbout() {
  QMessageBox msgBox;
  msgBox.setWindowTitle("О программе");

  QString msg = QString("Проект создали:") + "\n" + "Алехин Сергей\n" + "Торжков Максим\n"
      + "Горшков Иван\n" + "Забелина Варвара\n\n" +
      "Попробуйте наш продукт, занимательную игру BeatCovid:\n";
  msgBox.setText(msg);
  QFont font;
  font.setPointSize(14);
  msgBox.setFont(font);
  msgBox.show();
  msgBox.exec();
}

void MainWindow::slotShortcutCMDR() {
  updateFiles();
}

void MainWindow::startLoadingLabel() {
  _movie = new QMovie("../icons/ajax-loader.gif");
  ui->lb_loading->setMovie(_movie);
  ui->lb_loading->setVisible(true);
  ui->lb_loading->setAlignment(Qt::AlignCenter);
  _movie->start();
}

void MainWindow::stopLoadingLabel() {
  _movie->stop();
  ui->lb_loading->setHidden(true);
}

void MainWindow::changeDirectory() {
  QFileDialog dialog;
  dialog.setDirectory(QString::fromStdString(_app.GetSyncFolder()));
  dialog.setOptions(QFileDialog::ShowDirsOnly);
  dialog.setFileMode(QFileDialog::DirectoryOnly);
  if (dialog.exec()) {
    for (auto &file : dialog.selectedFiles()) {
      _app.UpdateSyncFolder(file.toStdString());
    }
    _settingsForm->close();
    onBtnSettings();
  }

}
