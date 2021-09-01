#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <bits/stdc++.h>

#define EVENT_NUM 12

::std::string g_event_str[EVENT_NUM] = {
    "IN_ACCESS",
    "IN_MODIFY",        //文件修改
    "IN_ATTRIB",
    "IN_CLOSE_WRITE",
    "IN_CLOSE_NOWRITE",
    "IN_OPEN",
    "IN_MOVED_FROM",    //文件移动from
    "IN_MOVED_TO",      //文件移动to
    "IN_CREATE",        //文件创建
    "IN_DELETE",        //文件删除
    "IN_DELETE_SELF",
    "IN_MOVE_SELF"
};
::std::vector<::std::string> g_ignore_list{".", ".."};

void usage() {
    fprintf(stderr, "----- \t\t ----------------------------------------------------------------------\n");
    fprintf(stderr, "-init \t\t Install sphinx and sphinx book Theme and complete file initialization.\n");
    fprintf(stderr, "-path \t\t Enter the file directory you want to listen to.\n");
    fprintf(stderr, "-ignore \t Files that need to be ignored.\n");
    fprintf(stderr, "----- \t\t ----------------------------------------------------------------------\n");
    return ;
}
char *g_ans_wd = NULL ;
char *g_ans_ini = NULL ;
const char *ini_info = "start init" ;
void argv_parse(int argc, char *argv[]) {
    int index = 1 ;
    while (index < argc) {
        if (0 == strcmp(argv[index], "-init")) {
            g_ans_ini = (char *)((void *)ini_info) ;
            system("pip3 install sphinx");
            system("pip3 install sphinx_book_theme");
            system("sphinx-quickstart");
            system("mkdir ./watcher") ;
            system("mv ./source ./watcher/");
            system("mv ./Makefile ./watcher/");
            system("mv ./make.bat ./watcher/");
            system("mv ./build ./watcher/");
        } else if (0 == strcmp(argv[index], "-path")) {
            ++index ;
            g_ans_wd = argv[index] ;
        } else if (0 == strcmp(argv[index], "-ignore")) {
            ++index ;
            while (index < argc) {
                g_ignore_list.emplace_back(::std::string(argv[index])) ;
                ++index ;
            }
            for (const auto &it:g_ignore_list) {
                fprintf(stdout, "%s\n", it.c_str());
                // ::std::cout << it << '\n' ;
            }
        } else if (0 == strcmp(argv[index], "-clear")) {
            system("sudo rm -rf ./watcher") ;
            // system("sudo rm -rf ./build") ;
            // system("sudo rm -rf ./source") ;
            // system("sudo rm -rf ./Makefile") ;
            // system("sudo rm -rf ./make.bat") ;
            return ;
        }
        ++index ;
    }
}


int main(int argc, char *argv[])
{
    int fd;
    int wd;
    int len;
    int nread;
    char buf[BUFSIZ];
    struct inotify_event *event;
    // 判断输入参数
    if (2 > argc) {
        fprintf(stderr, "%s\n", argv[0]);
        usage() ;
        return -1;
    }
    argv_parse(argc, argv) ;
 
    // 初始化
    fd = inotify_init();
    if (0 > fd) {
        fprintf(stderr, "inotify_init failed\n");
        return -1;
    }
 
    /* 增加监听事件
     * 监听所有事件：IN_ALL_EVENTS
     * 监听文件是否被创建,删除,移动：IN_CREATE|IN_DELETE|IN_MOVED_FROM|IN_MOVED_TO
     */
    if (NULL == g_ans_wd) {
        fprintf(stderr, "path argv failed\n");
        return -1;
    }
    if (ini_info == g_ans_ini) {
        fprintf(stdout, "%s\n", g_ans_wd);
        ::std::ofstream source_index("./watcher/source/index.rst", ::std::ios::out| ::std::ios::app) ;
        DIR *d = opendir(g_ans_wd);

        if (NULL == d) {
            fprintf(stderr, "opendir argv[path] failed\n");
        }
        struct stat st;
        struct dirent *entry;
        while ((entry = readdir(d)) != NULL) {
            ::std::string file_name(entry->d_name);
            if (file_name.find(".py") != std::string::npos) {
                file_name = file_name.substr(0, file_name.length() - 3);
                fprintf(stdout, "%s --- add \n", entry->d_name);
                source_index << '\n';
                source_index << file_name << '\n';
                source_index << "=======" << '\n';
                source_index << ".. automodule:: ";
                source_index << file_name << '\n';
                source_index << "   :members:\n";
                source_index << '\n' ;
            } 
        }
        closedir(d);
        source_index.close();
        ::std::ofstream source_config("./watcher/source/conf.py", ::std::ios::out| ::std::ios::app) ;
        source_config << '\n' ;
        source_config << "import os" << '\n' ;
        source_config << "extensions = ['sphinx.ext.autodoc', 'sphinx.ext.todo', 'sphinx.ext.viewcode']" << '\n' ;
        source_config << "import sys" << '\n' ;
        source_config << "sys.path.insert(0, os.path.abspath('../../'))" << '\n' ;
        source_config << "html_theme = 'sphinx_book_theme'" << '\n' ;
        source_config.close();
        system("sphinx-build -b html ./watcher/source  ./watcher/html_build") ;
        return -1 ;
    }

    wd = inotify_add_watch(fd, g_ans_wd, IN_MODIFY|IN_CREATE|IN_DELETE|IN_MOVED_FROM|IN_MOVED_TO);
    if (0 > wd) {
        fprintf(stderr, "inotify_add_watch %s failed\n", g_ans_wd);
        return -1;
    }
    for (int i=0 ; i<sizeof(buf) ; ++i) {
        buf[i] = 0;
    }
   
    while ( 0 < (len = read(fd, buf, sizeof(buf) - 1)))  {
        nread = 0;
        while (0 < len) {
            event = (struct inotify_event *)&buf[nread];
            for (int i=0 ; i<EVENT_NUM ; ++i) {
                if((event->mask >> i) & 1) {
                    if(event->len > 0) {
                        if ("IN_MODIFY"==g_event_str[i]) {
                            system("sphinx-build -b html ./watcher/source  ./watcher/html_build") ;
                        }
                        fprintf(stdout, "\033[36m%s --- %s\n\033[0m", event->name, g_event_str[i].c_str());
                    } else {
                        fprintf(stdout, "%s --- %s\n", " ", g_event_str[i].c_str());
                    }

                }
                // fprintf(stdout, "%d: %s --- %s\n", i, event->name, g_event_str[i].c_str());
            }
            nread = nread + sizeof(struct inotify_event) + event->len;
            len = len - sizeof(struct inotify_event) - event->len;
        }
    }
    close(fd);
    return 0;
}