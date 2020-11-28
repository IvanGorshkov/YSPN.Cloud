#ifndef _APP_H
#define _APP_H

#include <string>

class App {
public:

    void UploadFile();

    void EditFile();

    void DeleteFile();

    void AddFolder();

    void DeleteFolder();

    void MoveFile();

    void SignIn();

    void SignUp();

    void ChangePassword();

    void DeleteAccount();

    void RenameFile();

    void ChangeDefaultFolder();

    void ShowFileHistory();

    void RollBackFileContent();

    void DownloadFile();

    void Refresh();

    void ExitAccount();

    void DeleteFileFromComputer();

    void DeleteFileFromService();

    void Operation1();

private:
    std::string _fileName;
    std::string _filePath;
    std::string _folderName;
    std::string _folderPath;
   // Indexer _indexer;
   // InternalDB _internalDB;
   // ClientNetwork _clientNetwork;
   // Notifier _notifier;
};

#endif //_APP_H