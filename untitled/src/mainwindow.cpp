#include "include/mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto file1 = FileMeta{
            .fileId = 1,
            .version = 1,
            .fileName = "file1",
            .fileExtension = "txt",
            .filePath = "static/upload/",
            .fileSize = 150,
            .chunksCount = 12,
            .isDownload = false,
            .isDeleted = false,
            .isCurrent = true,
            .updateDate = "31.12.1970",
            .createDate = "31.11.1970"};
    _filesInfo.push_back(file1);

    auto file2 = FileMeta{
            .fileId = 2,
            .version = 10,
            .fileName = "file2",
            .fileExtension = "txt",
            .filePath = "static/",
            .fileSize = 76,
            .chunksCount = 25,
            .isDownload = true,
            .isDeleted = false,
            .isCurrent = true,
            .updateDate = "31.10.1970",
            .createDate = "31.09.1970"};
    _filesInfo.push_back(file2);

    auto file3 = FileMeta{
            .fileId = 3,
            .version = 6,
            .fileName = "file3",
            .fileExtension = "docx",
            .filePath = "static/uploads/",
            .fileSize = 89,
            .chunksCount = 17,
            .isDownload = true,
            .isDeleted = false,
            .isCurrent = true,
            .updateDate = "31.08.1970",
            .createDate = "31.07.1970"};
    _filesInfo.push_back(file3);

    auto file4 = FileMeta{
            .fileId = 4,
            .version = 15,
            .fileName = "file4",
            .fileExtension = "jpeg",
            .filePath = "static/uploads2/",
            .fileSize = 123,
            .chunksCount = 10,
            .isDownload = false,
            .isDeleted = false,
            .isCurrent = true,
            .updateDate = "31.06.1970",
            .createDate = "31.05.1970"};
    _filesInfo.push_back(file4);

    auto file5 = FileMeta{
            .fileId = 5,
            .version = 4,
            .fileName = "file5",
            .fileExtension = "png",
            .filePath = "papka/",
            .fileSize = 50,
            .chunksCount = 15,
            .isDownload = true,
            .isDeleted = false,
            .isCurrent = true,
            .updateDate = "31.04.1970",
            .createDate = "31.03.1970"};
    _filesInfo.push_back(file5);

    auto file6 = FileMeta{
            .fileId = 6,
            .version = 2,
            .fileName = "file6",
            .fileExtension = "html",
            .filePath = "static/uploads/",
            .fileSize = 124,
            .chunksCount = 1,
            .isDownload = true,
            .isDeleted = false,
            .isCurrent = true,
            .updateDate = "31.02.1970",
            .createDate = "31.01.1970"};
    _filesInfo.push_back(file6);

    auto file7 = FileMeta{
            .fileId = 7,
            .version = 51,
            .fileName = "file7",
            .fileExtension = "cpp",
            .filePath = "papka3/",
            .fileSize = 900,
            .chunksCount = 3,
            .isDownload = false,
            .isDeleted = false,
            .isCurrent = true,
            .updateDate = "31.12.1976",
            .createDate = "31.12.1974"};
    _filesInfo.push_back(file7);


    ui->centralwidget->setMinimumSize(800, 550);
    ui->centralwidget->setWindowIconText("YSNP.Cloud");
    parseVectorFiles();
    ui->treeView->setAnimated(true);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setModel(&_cloudModel);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));

    connect(ui->btn_refresh, SIGNAL(clicked(bool)), this, SLOT(onBtnRefresh()));
    connect(ui->btn_AddFile, SIGNAL(clicked(bool)), this, SLOT(onBtnAddFile()));
    connect(ui->btn_About, SIGNAL(clicked(bool)), this, SLOT(onBtnAbout()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onBtnAddFile() {
    qDebug() << "Something";
}

void MainWindow::onBtnRefresh() {
    qDebug() << "Something amazing";
}

void MainWindow::slotCustomMenuRequested(QPoint pos) {
    auto *menu = new QMenu(this);
    auto *openFile = new QAction(trUtf8("Открыть"), this);
    auto *renameFile = new QAction(trUtf8("Переименовать"), this);
    auto *downloadToDevice = new QAction(trUtf8("Скачать на устройство"), this);
    auto *deleteFromDevice = new QAction(trUtf8("Удалить с устройства"), this);
    auto *deleteFromCloud = new QAction(trUtf8("Удалить из облака"), this);
    auto *history = new QAction(trUtf8("История редактирования"), this);
    auto *properties = new QAction(trUtf8("Свойства"), this);

    connect(openFile, SIGNAL(triggered()), this, SLOT(open_file()));
    connect(renameFile, SIGNAL(triggered()), this, SLOT(rename_file()));
    connect(downloadToDevice, SIGNAL(triggered()), this, SLOT(download_on_device()));
    connect(deleteFromDevice, SIGNAL(triggered()), this, SLOT(delete_from_device()));
    connect(deleteFromCloud, SIGNAL(triggered()), this, SLOT(delete_from_cloud()));
    connect(history, SIGNAL(triggered()), this, SLOT(view_history_file()));
    connect(properties, SIGNAL(triggered()), this, SLOT(view_properties()));

    menu->addAction(openFile);
    menu->addAction(renameFile);
    menu->addAction(downloadToDevice);
    menu->addAction(deleteFromDevice);
    menu->addAction(deleteFromCloud);
    menu->addAction(history);
    menu->addAction(properties);

    if (ui->treeView->indexAt(pos).flags() == 32)
        menu->popup(ui->treeView->viewport()->mapToGlobal(pos));
}

bool compareFileMeta(const FileMeta &x, const FileMeta &y) {
    return x.getFullPath() < y.getFullPath();
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
        std::string full_path = file.getFullPath();
        files_paths.append(QString(full_path.c_str()));
    }
    std::sort(files_paths.begin(), files_paths.end());
    std::sort(_filesInfo.begin(), _filesInfo.end(), compareFileMeta);

    for (int i = 0; i < files_paths.size(); i++) {
        QStringList listPath = files_paths[i].split('/');
        QModelIndex parentIndex = rootIndex;
        for (int j = 0; j < listPath.size(); ++j) {
            if (listPath[j].contains(".")) {
                auto *insertFile = new QStandardItem;
                insertFile->setText(listPath[j]);
                insertFile->setWhatsThis(listPath[j]);
                insertFile->setToolTip(QString::number(_filesInfo[i].fileId));
                insertFile->setFlags(Qt::ItemIsEnabled);
                if (_filesInfo[i].fileExtension == "txt")
                    insertFile->setIcon(QIcon("../icons/txt.png"));
                else {
                    if ((_filesInfo[i].fileExtension == "pptx") || (_filesInfo[i].fileExtension == "ppt"))
                        insertFile->setIcon(QIcon("../icons/pptx.png"));
                    else {
                        if ((_filesInfo[i].fileExtension == "docx") || (_filesInfo[i].fileExtension == "doc"))
                            insertFile->setIcon(QIcon("../icons/docx.png"));
                        else {
                            if (_filesInfo[i].fileExtension == "html")
                                insertFile->setIcon(QIcon("../icons/html.png"));
                            else {
                                if (_filesInfo[i].fileExtension == "pdf")
                                    insertFile->setIcon(QIcon("../icons/pdf.png"));
                                else {
                                    if (_filesInfo[i].fileExtension == "py")
                                        insertFile->setIcon(QIcon("../icons/py.png"));
                                    else {
                                        if (_filesInfo[i].fileExtension == "cpp")
                                            insertFile->setIcon(QIcon("../icons/cpp.png"));
                                        else {
                                            if ((_filesInfo[i].fileExtension == "jpg") ||
                                                (_filesInfo[i].fileExtension == "jpeg") ||
                                                (_filesInfo[i].fileExtension == "png") ||
                                                (_filesInfo[i].fileExtension == "bmp") ||
                                                (_filesInfo[i].fileExtension == "svg") ||
                                                (_filesInfo[i].fileExtension == "webp"))
                                                insertFile->setIcon(QIcon("../icons/image.png"));
                                            else
                                                insertFile->setIcon(QIcon("../icons/file.png"));
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

void MainWindow::open_file() {
    QModelIndex index = ui->treeView->currentIndex();
    QMessageBox msgBox;
    QString mes = QString::number(index.row()) + ", " + QString::number(index.column());
    msgBox.setText(mes);
    msgBox.exec();
}

void MainWindow::rename_file() {

}

void MainWindow::download_on_device() {

}

void MainWindow::delete_from_device() {

}

void MainWindow::delete_from_cloud() {

}

void MainWindow::view_history_file() {

}

void MainWindow::view_properties() {
    QModelIndex index = ui->treeView->currentIndex();
    QStandardItem *file = _cloudModel.itemFromIndex(index);
    int idFile = file->toolTip().toInt();

    FileMeta allFileInfo;
    for (auto &i : _filesInfo) {
        if (i.fileId == idFile) {
            allFileInfo = i;
            break;
        }
    }
    auto *actionsListModel = new QStringListModel(this);

    QStringList actionsWithFile;

    QString isDownload = (allFileInfo.isDownload) ? "да" : "нет";
    QString size = "Размер файла: " + QString::number(allFileInfo.fileSize) + "Мб";
    QString version = "Версия файла: " + QString::number(allFileInfo.version);
    QString download = "Файл скачан на устройство: " + isDownload;
    QString dateUpdate = "Дата последнего обновления: " + QString::fromStdString(allFileInfo.updateDate);
    QString dateCreate = "Дата создания: " + QString::fromStdString(allFileInfo.createDate);
    actionsWithFile << size << version << download << dateUpdate << dateCreate;

    actionsListModel->setStringList(actionsWithFile);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setModel(actionsListModel);
}

void MainWindow::on_treeView_clicked(const QModelIndex &index) {
    auto *zeroModel = new QStringListModel(this);
    ui->listView->setModel(zeroModel);
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
