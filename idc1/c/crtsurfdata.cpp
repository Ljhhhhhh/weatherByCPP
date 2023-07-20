//
// Created by Lujh on 2023/7/20.
//

#include "_public.h"

CLogFile logFile(10);

int main(int argc, char *argv[]) {
    // inifile outpath logfile
    if (argc != 4) {
        // 如果参数非法，给出帮助文档。
        printf("Using:./crtsurfdata1 inifile outpath logfile\n");
        printf("Example:/Users/pipilu/Documents/learn/idc/idc1/bin/crtsurfdata1 /Users/pipilu/Documents/learn/idc/idc1/ini/stcode.ini /tmp/surfdata /log/idc/crtsurfdata1.log\n\n");

        printf("inifile 全国气象站点参数文件名。\n");
        printf("outpath 全国气象站点数据文件存放的目录。\n");
        printf("logfile 本程序运行的日志文件名。\n\n");

        return -1;
    }

    if (logFile.Open(argv[3], "a+", false) == false) {
        printf("logfile.Open(%s) failed.\n", argv[3]);
        return -1;
    } else {
        // 参数合法
        printf("参数合法.\n");
    }

    logFile.Write("crtsurfdata 开始运行。 \n");
    for (int i = 0; i < 10000000; i++) {
        logFile.Write("这是第%010d条日志记录。\n", i);
    }
    logFile.Write("crtsurfdata 结束运行。 \n");

    return 0;
}