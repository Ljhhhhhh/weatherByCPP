#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/format.hpp>
#include <regex>

namespace fs = boost::filesystem;

// 全国气象站点参数结构体
struct st_site {
    char provname[31]; // 省份
    char siteid[11]; // 站号
    char sitename[31]; // 站名
    double lat; // 纬度
    double lon; // 经度
    double height; // 海拔高度
};

// 全国气象站点分钟观测数据结构
struct st_surfdata {
    char siteid[11];     // 站号
    std::string datatime;   // 数据时间：格式yyyymmddhh24miss
    int t;              // 气温：单位，0.1摄氏度。
    int p;              // 气压：0.1百帕。
    int u;              // 相对湿度，0-100之间的值。
    int wd;             // 风向，0-360之间的值。
    int wf;             // 风速：单位0.1m/s
    int r;              // 降雨量：0.1mm。
    int vis;            // 能见度：0.1米。
};

// 存放全国气象站点参数的容器
std::vector<struct st_site> sites;

// 存放全国气象站点分钟观测数据的容器
std::vector<struct st_surfdata> surfData;

std::string LocalTime();

class ptime;

void LoadSite(const char *infile, fs::path logFilePath);

// 模拟生成全国气象站点分钟观测数据，存放在 surfData 容器中
void CrtSurfData();

// 把容器vsurfdata中的全国气象站点分钟观测数据写入文件。
bool CrtSurfFile(std::string curTime, std::string fileName);

// 获取当前时间，当作观测时间
std::string curTime;

// 获取当前项目根目录的路径
fs::path projectRoot = fs::absolute(fs::current_path());
// 创建log文件夹路径
fs::path logFolderPath = projectRoot / "../log";

int main(int argc, char const *argv[]) {
    if (argc != 4) {
        // 如果参数非法，给出帮助文档。
        std::cout << "Using:./crtsurfdata1 inifile outpath logfile" << std::endl;
        std::cout
                << "Example:/project/idc1/bin/crtsurfdata1 /project/idc1/ini/stcode.ini /tmp/surfdata /log/idc/crtsurfdata1.log"
                << std::endl;
        std::cout << std::endl;
        std::cout << "inifile 全国气象站点参数文件名。" << std::endl;
        std::cout << "outpath 全国气象站点数据文件存放的目录。" << std::endl;
        std::cout << "logfile 本程序运行的日志文件名。" << std::endl;
        std::cout << std::endl;
        return -1;
    } else {
        // 参数合法
        std::cout << "参数合法." << std::endl;
    }


    // 确保log文件夹存在，如果不存在则创建
    if (!fs::exists(logFolderPath)) {
        fs::create_directory(logFolderPath);
    }

    // 创建logger.log文件路径
    fs::path logFilePath = logFolderPath / argv[3];

    // 将数据写入文件
    std::ofstream outputStream(logFilePath.string());
    outputStream << "开始运行" << std::endl;
    // TODO:
    LoadSite(argv[2], logFilePath);
    memset(&curTime, 0, size(curTime));
    curTime = LocalTime();
    // 模拟生成全国气象站点分钟观测数据，存放在 surfData 容器中
    CrtSurfData();
    CrtSurfFile(curTime, argv[3]);
    outputStream << "结束运行" << std::endl;

    return 0;
}

void LoadSite(const char *infile, fs::path logFilePath) {
    if (fs::exists(infile)) {
        std::ifstream file(infile);

        if (file.is_open()) {
            std::vector<std::string> tokens;
            struct st_site site{};
            // 逐行读取文件内容
            std::string line;
            while (std::getline(file, line)) {
                boost::split(tokens, line, boost::is_any_of(","));
                if (tokens.size() == 6) {
                    memset(&site, 0, sizeof(struct st_site));
                    strncpy(site.provname, tokens[0].c_str(), sizeof(site.provname) - 1);
                    strncpy(site.siteid, tokens[1].c_str(), sizeof(site.siteid) - 1);
                    strncpy(site.sitename, tokens[2].c_str(), sizeof(site.sitename) - 1);
                    site.lat = std::stod(tokens[3]);
                    site.lon = std::stod(tokens[4]);
                    site.height = std::stod(tokens[5]);
                    sites.emplace_back(site);
                }
            }

            file.close();

            for (auto &item: sites) {
                std::cout << "provName = " << item.provname << ", siteId = " << item.siteid << ", siteName = "
                          << item.sitename
                          << ", lat = " << item.lat << ", lon = " << item.lon << ", height = " << item.height
                          << std::endl;
            }
        } else {
            std::cout << "Failed to open file." << std::endl;
        }
    } else {
        std::cout << "File does not exist." << std::endl;
    }
}

void CrtSurfData() {
    // 设置随机数种子。
    srand(time(0));
    struct st_surfdata stsurfData;

    for (int i = 0; i < sites.size(); i++) {
        memset(&stsurfData, 0, sizeof(struct st_surfdata));
        // 用随机数填充分钟观测数据的结构体
        strncpy(stsurfData.siteid, sites[i].siteid, 10); // 站号。
//        strncpy(, curTime, 14);
        stsurfData.datatime = curTime; // 数据时间：格式yyyymmddhh24miss
        stsurfData.t = rand() % 351;       // 气温：单位，0.1摄氏度
        stsurfData.p = rand() % 265 + 10000; // 气压：0.1百帕
        stsurfData.u = rand() % 100 + 1;     // 相对湿度，0-100之间的值。
        stsurfData.wd = rand() % 360;      // 风向，0-360之间的值。
        stsurfData.wf = rand() % 150;      // 风速：单位0.1m/s
        stsurfData.r = rand() % 16;        // 降雨量：0.1mm
        stsurfData.vis = rand() % 5001 + 100000;  // 能见度：0.1米

        std::cout << "站点ID: " << stsurfData.siteid;
        std::cout << " 日期时间: " << stsurfData.datatime;
        std::cout << " 气温: " << stsurfData.t;
        std::cout << " 气压: " << stsurfData.p;
        std::cout << " 湿度: " << stsurfData.u;
        std::cout << " 风向: " << stsurfData.wd;
        std::cout << " 风速: " << stsurfData.wf;
        std::cout << " 降雨量: " << stsurfData.r;
        std::cout << " 能见度: " << stsurfData.vis << std::endl;


        // 把观测数据的结构体放入vsurfdata容器。
        surfData.push_back(stsurfData);
    }
}

std::string LocalTime() {
    // 获取当前系统时间
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

    // 创建一个time_facet对象，用于格式化时间
    boost::posix_time::time_facet *facet = new boost::posix_time::time_facet("%Y%m%dT%H%M%S");

    // 创建一个输出流，并将time_facet对象绑定到输出流上
    std::ostringstream oss;
    oss.imbue(std::locale(std::cout.getloc(), facet));

    // 将时间格式化为字符串
    oss << now;

    // 输出格式化后的时间字符串
    std::cout << "当前时间：" << oss.str() << std::endl;

    return oss.str();
}

// 把容器vsurfdata中的全国气象站点分钟观测数据写入文件
bool CrtSurfFile(std::string curTime, std::string fileName) {
    fs::path weatherData = projectRoot / "../static/weather";
    // 确保log文件夹存在，如果不存在则创建
    if (!fs::exists(weatherData)) {
        fs::create_directory(weatherData);
    }

    std::regex regex("\\.(.*)$");
    std::smatch match;
    std::string fileType;
//    TODO：
    if (std::regex_search(fileName, match, regex)) {
        fileType = match[1];
        std::cout << "提取的内容: " << match[1] << std::endl;
    } else {
        return false;
        std::cout << "请输入文件格式" << std::endl;
    }

    // 创建logger.log文件路径
    fs::path filePath = weatherData / "demo.json" ;// / fileType;

    std::cout << "文件路径: " << filePath << std::endl;
    // 将数据写入文件
    std::ofstream outputStream(filePath.string());
//    outputStream << "开始运行" << std::endl;

    if (fileType == "csv") {
        // 写入第一行标题
        outputStream << "站点代码,数据时间,气温,气压,相对湿度,风向,风速,降雨量,能见度\n";
    } else if (fileType == "xml") {
        outputStream << "<data>\\n";
    } else if (fileType == "json") {
        outputStream << "{\"data\":[\n";
    }


    if (fileType == "csv") {
        // 遍历存放观测数据的vsurfdata容器
        for (int i = 0; i < surfData.size(); i++) {
            // 写入一条记录
            boost::format formatString("%s,%s,%.1f,%.1f,%d,%d,%.1f,%.1f,%.1f\n");
            std::string formattedString = boost::str(formatString %
                                                     surfData[i].siteid %
                                                     surfData[i].datatime %
                                                     (surfData[i].t / 10.0) %
                                                     (surfData[i].p / 10.0) %
                                                     surfData[i].u %
                                                     surfData[i].wd %
                                                     (surfData[i].wf / 10.0) %
                                                     (surfData[i].r / 10.0) %
                                                     (surfData[i].vis / 10.0));
            outputStream << formattedString;

//        printf("生成数据文件%s成功，数据时间%s，记录数%d。\n", filePath.string(), fileName, surfData.size())
        }
    } else if (fileType == "xml") {
        for (int i = 0; i < surfData.size(); i++) {
            std::string result = (boost::format("<siteid>%s</siteid></datatime>%s</datatime><t>%.1f</t><p>%.1f</p>"
                                                "<u>%d</u><wd>%d</wd><wf>%.1f</wf><r>%.1f</r><vis>%.1f</vis>\n")
                                  % surfData[i].siteid
                                  % surfData[i].datatime
                                  % (surfData[i].t / 10.0)
                                  % (surfData[i].p / 10.0)
                                  % surfData[i].u
                                  % surfData[i].wd
                                  % (surfData[i].wf / 10.0)
                                  % (surfData[i].r / 10.0)
                                  % (surfData[i].vis / 10.0)).str();

            outputStream << result;
        }
    } else if (fileType == "json") {
        for (int i = 0; i < surfData.size(); i++) {
            boost::format format(
                    "{\"siteid\":\"%s\",\"datatime\":\"%s\",\"t\":\"%.1f\",\"p\":\"%.1f\",\"u\":\"%d\",\"wd\":\"%d\",\"wf\":\"%.1f\",\"r\":\"%.1f\",\"vis\":\"%.1f\"},");
            std::string result = boost::str(
                    format % surfData[i].siteid % surfData[i].datatime % (surfData[i].t / 10.0) %
                    (surfData[i].p / 10.0) % surfData[i].u % surfData[i].wd % (surfData[i].wf / 10.0) %
                    (surfData[i].r / 10.0) % (surfData[i].vis / 10.0));
            outputStream << result;
        }
    }
    if (fileType == "xml") outputStream << "</data>\n";
    if (fileType == "json") outputStream << "]}\n";
}