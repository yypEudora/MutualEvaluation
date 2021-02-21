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

/**
 * @brief Stu_Login_Instance::Stu_Login_Instance 把复制构造函数和=操作符也设为私有,防止被复制
 */
Stu_Login_Instance::Stu_Login_Instance(const Stu_Login_Instance& )
{
}


Stu_Login_Instance& Stu_Login_Instance::operator=(const Stu_Login_Instance&)
{
    return *this;
}


/**
 * @brief Stu_Login_Instance::getInstance 获取唯一的实例
 * @return m_instance 唯一实例
 */
Stu_Login_Instance *Stu_Login_Instance::getInstance()
{
    return m_instance;
}


/**
 * @brief Stu_Login_Instance::destroy 释放堆区空间
 */
void Stu_Login_Instance::destroy()
{
    if(NULL != Stu_Login_Instance::m_instance)
    {
        qDebug() << Stu_Login_Instance::m_instance->get_user() <<"instance is detele";
        delete Stu_Login_Instance::m_instance;
        Stu_Login_Instance::m_instance = NULL;

    }
}


/**
 * @brief Stu_Login_Instance::set_login_info 设置登录信息
 * @param user, ip, port 设置用户的id,ip和port
 */
void Stu_Login_Instance::set_login_info( QString user, QString ip, QString port)
{
    m_user = user;
    m_ip = ip;
    m_port = port;
}


/**
 * @brief Stu_Login_Instance::get_user 获取登录id
 */
QString Stu_Login_Instance::get_user() const
{
    return m_user;
}


/**
 * @brief Stu_Login_Instance::get_ip 获取登录ip
 */
QString Stu_Login_Instance::get_ip() const
{
    return m_ip;
}


/**
 * @brief Stu_Login_Instance::get_port 获取登录port
 */
QString Stu_Login_Instance::get_port() const
{
    return m_port;
}
