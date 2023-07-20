/*
 *  程序名：ftpgetfiles2.cpp，本程序是通用的功能模块，用于把远程ftp服务器的文件下载到本地目录
 *  作者：wydxry
 *  时间：2022.06.06 09:10 
*/

#include "_public.h"
#include "_ftp.h"

void _help();

CLogFile logfile;

Cftp ftp;

struct st_arg
{
    char host[31];           // 远程服务器的IP和端口
    int  mode;               // 传输模式，1-被动模式，2-主动模式，缺省采用被动模式
    char username[31];       // 远程服务器ftp的用户名
    char password[31];       // 远程服务器ftp的密码
    char remotepath[301];    // 远程服务器存放文件的目录
    char localpath[301];     // 本地文件存放的目录
    char matchname[101];     // 待下载文件匹配的规则
    char listfilename[301];  // 下载前列出服务器文件名的文件
} starg;

// 文件信息的结构体
struct st_fileinfo
{
  char filename[301];   // 文件名
  char mtime[21];       // 文件时间
};

// 存放下载前列出服务器文件名的容器
vector<struct st_fileinfo> vlistfile;    

bool _xmltoarg(char *strxmlbuffer);

void EXIT(int sig);

bool _ftpgetfiles();

bool LoadListFile();

int main(int argc, char *argv[])
{
    // 小目标，把ftp服务上某目录中的文件下载到本地的目录中
    if (argc != 3) {
        _help();
        return -1;
    }

    // 关闭全部的信号和输入输出。
    // 设置信号,在shell状态下可用 "kill + 进程号" 正常终止些进程。
    // 但请不要用 "kill -9 +进程号" 强行终止。
    // CloseIOAndSignal(true); 
    signal(SIGINT, EXIT);  
    signal(SIGTERM, EXIT);

    if (logfile.Open(argv[1], "a+") == false) {
        printf("打开日志文件失败（%s）。\n", argv[1]); 
        return -1;
    }

    // 解析xml，得到程序运行的参数。
    if (_xmltoarg(argv[2]) == false) return -1;

    // 登录ftp服务器
    if (ftp.login(starg.host, starg.username, starg.password, starg.mode) == false) {
        logfile.Write("ftp.login(%s,%s,%s) failed.\n", starg.host, starg.username, starg.password); 
        return -1;
    }

    logfile.Write("ftp.login ok.\n");

    // 下载文件功能的主函数
    _ftpgetfiles();

    // 进入ftp服务器存放文件的目录

    // 调用ftp.nlist()方法列出服务器目录中的文件，结果存放到本地文件中

    // 把ftp.nlist()方法获取到的list文件加载到容器vfilelist中

    // 遍历容器vfilelist。

    /*
    for (int i=0;i<vlistfile.size();i++)
    {
        // 调用ftp.get()方法从服务器下载文件。
    }
    */

    ftp.logout();
    
    return 0;
}

void _help()
{
    printf("\n");
    printf("Using:/demo/tools/bin/ftpgetfiles logfilename xmlbuffer\n\n");

    printf("Sample:/demo/tools/bin/procctl 30 /demo/tools/bin/ftpgetfiles /demo/idc/log/ftpgetfiles_surfdata.log \"<host>127.0.0.1:21</host><mode>1</mode><username>wydxry</username><password>wydxry</password><localpath>/idcdata/surfdata</localpath><remotepath>/tmp/idc/surfdata</remotepath><matchname>SURF_ZH*.XML,SURF_ZH*.CSV</matchname>\"\n\n\n");

    printf("本程序是通用的功能模块，用于把远程ftp服务器的文件下载到本地目录。\n");
    printf("logfilename是本程序运行的日志文件。\n");
    printf("xmlbuffer为文件下载的参数，如下：\n");
    printf("<host>127.0.0.1:21</host> 远程服务器的IP和端口。\n");
    printf("<mode>1</mode> 传输模式，1-被动模式，2-主动模式，缺省采用被动模式。\n");
    printf("<username>wydxry</username> 远程服务器ftp的用户名。\n");
    printf("<password>wydxrypwd</password> 远程服务器ftp的密码。\n");
    printf("<remotepath>/tmp/idc/surfdata</remotepath> 远程服务器存放文件的目录。\n");
    printf("<localpath>/idcdata/surfdata</localpath> 本地文件存放的目录。\n");
    printf("<matchname>SURF_ZH*.XML,SURF_ZH*.CSV</matchname> 待下载文件匹配的规则。"\
            "不匹配的文件不会被下载，本字段尽可能设置精确，不建议用*匹配全部的文件。\n\n\n");
    printf("<listfilename>/idcdata/ftplist/ftpgetfiles_surfdata.list</listfilename> 下载前列出服务器文件名的文件。\n\n\n");
}

// 把xml解析到参数starg结构中
bool _xmltoarg(char *strxmlbuffer)
{
    memset(&starg, 0, sizeof(struct st_arg));

    // 远程服务器的IP和端口
    GetXMLBuffer(strxmlbuffer, "host", starg.host, 30);   
    if (strlen(starg.host) == 0) { 
        logfile.Write("host is null.\n");  
        return false; 
    }

    // 传输模式，1-被动模式，2-主动模式，缺省采用被动模式
    GetXMLBuffer(strxmlbuffer, "mode", &starg.mode);   
    if (starg.mode != 2) {
        starg.mode=1;
    }

    // 远程服务器ftp的用户名
    GetXMLBuffer(strxmlbuffer, "username", starg.username, 30);  
    if (strlen(starg.username) == 0) {
        logfile.Write("username is null.\n");  
        return false; 
    } 

    // 远程服务器ftp的密码
    GetXMLBuffer(strxmlbuffer, "password", starg.password, 30);  
    if (strlen(starg.password) == 0) {
        logfile.Write("password is null.\n");  
        return false;
    }

    // 远程服务器存放文件的目录
    GetXMLBuffer(strxmlbuffer, "remotepath", starg.remotepath, 300);  
    if (strlen(starg.remotepath) == 0) {
        logfile.Write("remotepath is null.\n");  
        return false;
    }

    // 本地文件存放的目录
    GetXMLBuffer(strxmlbuffer, "localpath", starg.localpath, 300);   
    if (strlen(starg.localpath) == 0) {
        logfile.Write("localpath is null.\n");  
        return false;
    }

    // 待下载文件匹配的规则
    GetXMLBuffer(strxmlbuffer, "matchname", starg.matchname, 100);   
    if (strlen(starg.matchname) == 0) {
        logfile.Write("matchname is null.\n");  
        return false;
    }

    // 下载前列出服务器文件名的文件
    GetXMLBuffer(strxmlbuffer, "listfilename", starg.listfilename, 300);   
    if (strlen(starg.listfilename) == 0) {
        logfile.Write("listfilename is null.\n");  
        return false;
    }

    return true;
}

void EXIT(int sig)
{
    printf("程序退出，sig=%d\n\n",sig);

    exit(0);
}

// 下载文件功能的主函数
bool _ftpgetfiles() 
{
    // 进入ftp服务器存放文件的目录
    if (ftp.chdir(starg.remotepath) == false) {
        logfile.Write("ftp.chdir(%s) failed.\n", starg.remotepath); 
        return false;
    }

    // 调用ftp.nlist()方法列出服务器目录中的文件，结果存放到本地文件中
    if (ftp.nlist(".", starg.listfilename) == false) {
        logfile.Write("ftp.nlist(%s) failed.\n", starg.remotepath); return false;
    }

    // 把ftp.nlist()方法获取到的list文件加载到容器vlistfile中
    if (LoadListFile() == false) {
        logfile.Write("LoadListFile() failed.\n");  
        return false;
    }

    char strremotefilename[301], strlocalfilename[301];
    
    // 遍历容器vlistfile
    for (int i = 0; i < vlistfile.size(); i++) {
        SNPRINTF(strremotefilename, sizeof(strremotefilename), 300, "%s/%s", starg.remotepath, vlistfile[i].filename);
        SNPRINTF(strlocalfilename, sizeof(strlocalfilename), 300, "%s/%s", starg.localpath, vlistfile[i].filename);

        // 调用ftp.get()方法从服务器下载文件
        logfile.Write("get %s ...", strremotefilename);

        if (ftp.get(strremotefilename, strlocalfilename) == false) {
            logfile.WriteEx("failed.\n"); 
            break;
        }

        logfile.WriteEx("ok.\n");
    }

    return true;
}

// 把ftp.nlist()方法获取到的list文件加载到容器vlistfile中
bool LoadListFile() 
{
    vlistfile.clear();
    
    CFile File;

    if (File.Open(starg.listfilename, "r") == false) {
        logfile.Write("File.Open(%s) 失败。\n", starg.listfilename); 
        return false;
    }

    struct st_fileinfo stfileinfo;

    while (1) {
        memset(&stfileinfo, 0, sizeof(struct st_fileinfo));

        if (File.Fgets(stfileinfo.filename, 300, true) == false) break;

        if (MatchStr(stfileinfo.filename, starg.matchname) == false) continue;
        
        vlistfile.push_back(stfileinfo);
    }

    for (int i = 0; i < vlistfile.size(); i++) {
        logfile.Write("filename=%s=\n", vlistfile[i].filename);
    }

    return true;
}