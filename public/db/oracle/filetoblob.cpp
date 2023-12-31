/*
 *  程序名：filetoblob.cpp，此程序演示开发框架操作Oracle数据库。
 *  把当前目录的pic_in.jpeg文件写入Oracle的BLOB字段中。
*/
#include "_ooci.h"   // 开发框架操作Oracle的头文件。

int main(int argc,char *argv[])
{
  connection conn; // 数据库连接类
  
  // 连接数据库，返回值0-成功，其它-失败
  // 失败代码在conn.m_cda.rc中，失败描述在conn.m_cda.message中。
  if (conn.connecttodb("scott/tiger@snorcl11g_132","Simplified Chinese_China.AL32UTF8") != 0)
  {
    printf("connect database failed.\n%s\n",conn.m_cda.message); return -1;
  }
  
  sqlstatement stmt(&conn); // SQL语句操作类
  
  // 为了方便演示，把girls表中的记录全删掉，再插入一条用于测试的数据。
  // 不需要判断返回值
  stmt.prepare("\
    BEGIN\
      delete from girls;\
      insert into girls(id,name,pic) values(1,'超女姓名',empty_blob());\
    END;");
  
  // 执行SQL语句，一定要判断返回值，0-成功，其它-失败。
  if (stmt.execute() != 0)
  {
    printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message); return -1;
  }

  // 使用游标从girls表中提取id为1的pic字段
  // 注意了，同一个sqlstatement可以多次使用
  // 但是，如果它的sql改变了，就要重新prepare和bindin或bindout变量
  stmt.prepare("select pic from girls where id=1 for update");
  stmt.bindblob();

  // 执行SQL语句，一定要判断返回值，0-成功，其它-失败。
  if (stmt.execute() != 0)
  {
    printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message); return -1;
  }

  // 获取一条记录，一定要判断返回值，0-成功，1403-无记录，其它-失败。
  if (stmt.next() != 0) return 0;
  
  // 把磁盘文件pic_in.jpeg的内容写入BLOB字段，一定要判断返回值，0-成功，其它-失败。
  if (stmt.filetolob((char *)"pic_in.jpeg") != 0)
  {
    printf("stmt.filetolob() failed.\n%s\n",stmt.m_cda.message); return -1;
  }

  conn.commit(); // 提交事
}

