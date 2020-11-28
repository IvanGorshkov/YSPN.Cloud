/**
 * Project Untitled
 */


#ifndef _WATCHER_H
#define _WATCHER_H
#include <boost/filesystem.hpp>
#include <string>
#include <map>
#include <sys/inotify.h>

class Watcher {
public:
    
/**
 * @param string
 */
void WatchFile(boost::filesystem::path file);
    
/**
 * @param string
 */
void WatchDirectory(boost::filesystem::path path);
    
void GetNextEvent();
    
/**
 * @param payh
 */
void UnwatcFile(boost::filesystem::path file);
    
/**
 * @param uint32_t
 */
void SetEventMask(uint32_t eventMask);
private: 
    uint32_t EventMask;
    int InotifyFd;
    std::map<int, boost::filesystem::path> DirectorieMap;
    
/**
 * @param int
 */
void removeWatch(int wd);
    
/**
 * @param int
 */
boost::filesystem::path wdToPath(int wd);
};

#endif //_WATCHER_H