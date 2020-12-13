//
// Created by dantedoyl on 04.12.2020.
//

#include "FileSystemEvent.h"


FileSystemEvent::FileSystemEvent(
        const int wd,
        uint32_t mask,
        const boost::filesystem::path& path,
        const std::chrono::steady_clock::time_point& eventTime)
        : wd(wd),
        mask(mask),
        path(path),
        eventTime(eventTime)
{
}

FileSystemEvent::~FileSystemEvent()
{
}