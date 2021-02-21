#ifndef STU_LOGIN_INFO_INSTANCE_H
#define STU_LOGIN_INFO_INSTANCE_H

#include <QString>
#include "common/common.h"


//单例模式，主要保存当前登录学生用户，服务器信息
class Stu_Login_Instance
{
public:
     static Stu_Login_Instance *getInstance(); //保证唯一一个实例
     static void destroy(); //释放堆区空间

     void set_login_info( QString user, QString ip, QString port);//设置登陆信息
     QString get_user() const;   //获取登录用户
     QString get_ip() const;    //获取登录ip
     QString get_port() const;    //获取登录port

private:
    //构造和析构函数为私有的
    Stu_Login_Instance();
    ~Stu_Login_Instance();
    //把复制构造函数和=操作符也设为私有,防止被复制
    Stu_Login_Instance(const Stu_Login_Instance&);
    Stu_Login_Instance& operator=(const Stu_Login_Instance&);

    //它的唯一工作就是在析构函数中删除Singleton的实例
    class Instance_Destroy
    {
    public:
        ~Instance_Destroy()
        {
            //释放堆区空间
            Stu_Login_Instance::destroy();
        }
    };

    //定义一个静态成员变量，程序结束时，系统会自动调用它的析构函数
    //static类的析构函数在main()退出后调用
    static Instance_Destroy m_instance_destroy; //静态数据成员，类中声明，类外定义


    //静态数据成员，类中声明，类外必须定义
    static Stu_Login_Instance *m_instance;


    QString m_user;   //当前登录用户
    QString m_ip;
    QString m_port;
};


#endif // STU_LOGIN_INFO_INSTANCE_H
