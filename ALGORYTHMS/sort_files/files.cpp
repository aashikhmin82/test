#include <iostream>
#include <dirent.h>
#include <memory>
#include <vector>
#include <sys/stat.h>

#include "files.h"

std::vector <std::string> get_files (const std::string& dir)
{
    std::vector <std::string> files;
    std::shared_ptr <DIR> directory_ptr(opendir(dir.c_str()), [](DIR* dir){dir && closedir(dir); });
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

    std::vector <std::pair<std::string, size_t>> fstat_info;
    fstat_info.push_back(std::make_pair("size", stat_buf.st_size));
    fstat_info.push_back(std::make_pair("inode", stat_buf.st_ino));
    fstat_info.push_back(std::make_pair("mode", stat_buf.st_mode));
    fstat_info.push_back(std::make_pair("nlink", stat_buf.st_nlink));
    fstat_info.push_back(std::make_pair("uid", stat_buf.st_uid));
    fstat_info.push_back(std::make_pair("gid", stat_buf.st_gid));
    std::cout << "[DEL] " << filename << " : " << std::endl;
    std::cout << "[DEL] Size -> " << stat_buf.st_size << std::endl;
    std::cout << "[DEL] Dev -> " << stat_buf.st_dev << std::endl;
    std::cout << "[DEL] Inode -> " << stat_buf.st_ino << std::endl;
    std::cout << "[DEL] Mode -> " << stat_buf.st_mode << std::endl;
    std::cout << "[DEL] Nlink -> " << stat_buf.st_nlink << std::endl;
    std::cout << "[DEL] Uid -> " << stat_buf.st_uid << std::endl;
    std::cout << "[DEL] Gid -> " << stat_buf.st_gid << std::endl;
    std::cout << "[DEL] rdev -> " << stat_buf.st_rdev << std::endl;
    std::cout << "[DEL] atime -> " << stat_buf.st_atime << std::endl;
    std::cout << "[DEL] mtime -> " << stat_buf.st_mtime << std::endl;
    std::cout << "[DEL] ctime -> " << stat_buf.st_ctime << std::endl;
    return fstat_info;
}
