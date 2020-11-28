/**
 * Project Untitled
 */


#ifndef _NOTIFIER_H
#define _NOTIFIER_H
#include "Notification.h"
#include "Watcher.h"

class Notifier {
public: 
    
void Run();
    
void Stop();
    
/**
 * @param Event
 * @param void function Notification
 */
Notifier& OnEvent(Event event, std::function <void(Notification)>);
    
/**
 * @param path
 */
Notifier& WatchFile(boost::filesystem::path file);
    
/**
 * @param path
 */
Notifier& UnwatchFile(boost::filesystem::path file);
private:
    std::shared_ptr<Watcher> _watcher;
};

#endif //_NOTIFIER_H