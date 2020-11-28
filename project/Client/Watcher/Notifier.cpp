/**
 * Project Untitled
 */


#include "Notifier.h"


/**
 * Notifier implementation
 */


/**
 * @return void
 */
void Notifier::Run() {
    return;
}

/**
 * @return void
 */
void Notifier::Stop() {
    return;
}

/**
 * @param Event
 * @param void function Notification
 * @return Notifier&
 */
Notifier& Notifier::OnEvent(Event event, std::function <void(Notification)>) {
    return {};
}

/**
 * @param path
 * @return Notifier&
 */
Notifier& Notifier::WatchFile(boost::filesystem::path file) {
    return null;
}

/**
 * @param path
 * @return Notifier&
 */
Notifier& Notifier::UnwatchFile(boost::filesystem::path file) {
    return null;
}