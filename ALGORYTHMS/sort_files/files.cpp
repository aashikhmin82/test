#include <dirent.h>
#include <iostream>
#include <memory>
#include <sys/stat.h>
#include <vector>

#include "files.h"

std::vector <std::string> get_files (const std::string& dir)
{
    std::vector <std::string> files;
    std::shared_ptr <DIR> directory_ptr(opendir(dir.c_str()), [](DIR* dir){if(dir) closedir(dir); });
    struct dirent *dirent_ptr;
    if (!directory_ptr)
    {
        std::cerr << "[get_files] Error" << std::endl;
        return files;
    }

    while ((dirent_ptr = readdir(directory_ptr.get())) != nullptr)
    {
        files.push_back(std::string(dirent_ptr->d_name));
    }

    return files;
}

int get_file_size (const std::string& filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

std::vector<std::pair<std::string, size_t>> get_file_stat (const std::string& filename)
{
    struct stat stat_buf;
    stat(filename.c_str(), &stat_buf);

    std::vector <std::pair<std::string, size_t>> fstat_info {
        { "size", stat_buf.st_size },
        { "inode", stat_buf.st_ino },
        { "mode", stat_buf.st_mode },
        { "nlink", stat_buf.st_nlink },
        { "uid", stat_buf.st_uid },
        { "gid", stat_buf.st_gid },
    };

    return fstat_info;
}
