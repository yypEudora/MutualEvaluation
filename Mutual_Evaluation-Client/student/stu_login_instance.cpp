#include "student/stu_login_instance.h"

//static类的析构函数在main()退出后调用
//静态数据成员，类中声明，类外定义
Stu_Login_Instance::Instance_Destroy Stu_Login_Instance::m_instance_destroy;

//静态变量动态分配空间
//静态数据成员，类中声明，类外必须定义
Stu_Login_Instance* Stu_Login_Instance::m_instance = new Stu_Login_Instance;

Stu_Login_Instance::Stu_Login_Instance()
{

}

Stu_Login_Instance::~Stu_Login_Instance()
{

}

//把复制构造函数和=操作符也设为私有,防止被复制
Stu_Login_Instance::Stu_Login_Instance(const Stu_Login_Instance& )
{
}

Stu_Login_Instance& Stu_Login_Instance::operator=(const Stu_Login_Instance&)
{
    return *this;
}

//获取唯一的实例
Stu_Login_Instance *Stu_Login_Instance::getInstance()
{
    return m_instance;
}

//释放堆区空间
void Stu_Login_Instance::destroy()
{
    if(NULL != Stu_Login_Instance::m_instance)
    {
        qDebug() << Stu_Login_Instance::m_instance->get_user() <<"instance is detele";
        delete Stu_Login_Instance::m_instance;
        Stu_Login_Instance::m_instance = NULL;

    }
}

//设置登录信息
void Stu_Login_Instance::set_login_info( QString current_user, QString ip, QString port)
{
    m_user = current_user;
    m_ip = ip;
    m_port = port;
}

//获取登录用户
QString Stu_Login_Instance::get_user() const
{
    return m_user;
}

//获取登录ip
QString Stu_Login_Instance::get_ip() const
{
    return m_ip;
}

//获取登录port
QString Stu_Login_Instance::get_port() const
{
    return m_port;
}
