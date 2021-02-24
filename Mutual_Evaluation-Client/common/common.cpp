#include <QFile>
#include <QMap>
#include <QDir>
#include <QTime>
#include <QFileInfo>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QApplication>
#include <QJsonDocument>
#include <QFileInfoList>
#include <QDesktopWidget>
#include <QCryptographicHash>
#include "des.h"
#include "common.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QString>

Common::Common(QObject *parent)
{
    Q_UNUSED(parent)
}

/**
 * @brief Common::move_to_center  窗口在屏幕中央显示
 * @param tmp     窗口部件
 */
void Common::move_to_center(QWidget *tmp)
{
     // 显示窗口
     tmp->show();
     // 屏幕中间显示
     // 使用qApp->desktop();也可以
     QDesktopWidget* desktop = QApplication::desktop();
     // 移动窗口
     tmp->move((desktop->width() - tmp->width())/2, (desktop->height() - tmp->height())/2);
}

/**
 * @brief Common::get_cfg_value 从配置文件中得到相对应的参数
 * @param title，key,path     配置文件title名称[title],key,配置文件路径
 *
 */
QString Common::get_cfg_value(QString title, QString key, QString path)
{
    QDir dir;
    if(!dir.exists("conf")){
        dir.mkdir("conf");
    }
    QFile file(QString::fromStdString(path.toLocal8Bit().constData()));
    qDebug() << path << "测试路径";

    // 读写方式打开
    if(false == file.open(QIODevice::ReadWrite) )
    {
        // 打开失败
        cout << "file open err";
        return "";
    }

    QByteArray json = file.readAll(); // 读取所有内容
    file.close(); // 关闭文件

    QJsonParseError error;

    // 将来源数据json转化为JsonDocument
    // 由QByteArray对象构造一个QJsonDocument对象，用于我们的读写操作
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    qDebug() << "doc" <<doc;
    if (error.error == QJsonParseError::NoError) // 没有出错
    {
        if (doc.isNull() || doc.isEmpty())
        {
            cout << "doc.isNull() || doc.isEmpty()";
            return "";
        }

        if( doc.isObject()) // 如果对象不为空
        {
            // QJsonObject json对象，描述json数据中{}括起来部分
            QJsonObject obj = doc.object();// 取得最外层这个大对象

            QJsonObject tmp = obj.value( title ).toObject();
            QStringList list = tmp.keys(); // 取出key列表
            for(int i = 0; i < list.size(); ++i)
            {
                if( list.at(i) == key )
                {
                    return tmp.value( list.at(i) ).toString();
                }
            }

        }
    }
    else
    {
        cout << "err = " << error.errorString();
    }

    return "";
}

/**
 * @brief Common::write_login_info  登录信息，写入配置文件
 * @param user，pwd,is_rember,path   用户名,密码，是否记住密码，配置文件路径
 *
 */
void Common::write_login_info(QString user, QString pwd, bool is_remeber, QString path)
{
    // login信息
    QMap<QString, QVariant> login;

    //用户名,密码加密
    QByteArray user_name = info_encrypt(user);
    QByteArray user_pwd = info_encrypt(pwd);

    login.insert("user",user_name);
    login.insert("pwd",user_pwd);

    if(is_remeber == true)
    {
         login.insert("remember", "yes");
    }
    else
    {
        login.insert("remember", "no");
    }

    // QVariant类作为一个最为普遍的Qt数据类型的联合
    // QVariant为一个万能的数据类型--可以作为许多类型互相之间进行自动转换。
    QMap<QString, QVariant> json;
    json.insert("login", login);

    QJsonDocument jsonDocument = QJsonDocument::fromVariant(json);
    if ( jsonDocument.isNull() == true)
    {
        cout << " QJsonDocument::fromVariant(json) err";
        return;
    }

    QFile file(path);
    if( false == file.open(QIODevice::WriteOnly) )
    {
        cout << "file open err";
        return;
    }

    file.write(jsonDocument.toJson());
    file.close();
}

/**
 * @brief Common::write_socket_info  服务器信息，写入配置文件
 * @param ip,port,path  ip地址，端口，配置文件路径
 *
 */
void Common::write_socket_info(QString ip, QString port, QString path)
{
    // socket_server信息
    QMap<QString, QVariant> socket_server;
    socket_server.insert("ip", ip);
    socket_server.insert("port", port);

    // QVariant类作为一个最为普遍的Qt数据类型的联合
    // QVariant为一个万能的数据类型--可以作为许多类型互相之间进行自动转换。
    QMap<QString, QVariant> json;
    json.insert("socket_server", socket_server);

    QJsonDocument jsonDocument = QJsonDocument::fromVariant(json);
    if ( jsonDocument.isNull() == true)
    {
        cout << " QJsonDocument::fromVariant(json) err";
        return;
    }

    QFile file(path);
    if( false == file.open(QIODevice::WriteOnly) )
    {
        cout << "file open err";
        return;
    }

    file.write(jsonDocument.toJson());
    file.close();

}

/**
 * @brief Common::get_file_md5  获取某个文件的md5码
 * @param file_path  文件路径
 *
 */
QString Common::get_file_md5(QString file_path)
{
    QFile localFile(file_path);

    if (!localFile.open(QFile::ReadOnly))
    {
        qDebug() << "file open error.";
        return 0;
    }

    QCryptographicHash ch(QCryptographicHash::Md5);

    quint64 totalBytes = 0;
    quint64 bytesWritten = 0;
    quint64 bytesTowrite =0;
    quint64 loadSize = 1024 * 4;
    QByteArray buf;

    totalBytes = localFile.size();
    bytesTowrite = totalBytes;

    while(1)
    {
        if(bytesTowrite > 0)
        {
            buf = localFile.read(qMin(bytesTowrite, loadSize));
            ch.addData(buf);
            bytesWritten += buf.length();
            bytesTowrite -= buf.length();
            buf.resize(0);
        }
        else
        {
            break;
        }
        if(bytesWritten == totalBytes)
        {
            break;
        }
    }

    localFile.close();
    QByteArray md5 = ch.result();
    return md5.toHex();
}

/**
 * @brief Common::get_str_md5  将某个字符串加密成md5码
 * @param str  字符串
 *
 */
QString Common::get_str_md5(QString str)
{
    QByteArray array;
    //md5加密
    array = QCryptographicHash::hash ( str.toLocal8Bit(), QCryptographicHash::Md5 );

    return array.toHex();
}

/**
 * @brief Common::get_boundary  产生分隔线
 * @param str  字符串
 *
 */
QString Common::get_boundary()
{
    // 随机种子
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QString tmp;

    // 48~122, '0'~'A'~'z'
    for(int i = 0; i < 16; i++)
    {
        tmp[i] = qrand() % (122-48) + 48;
    }

    return QString("------SocketKitFormBoundary%1").arg(tmp);
}

/**
 * @brief Common::info_encrypt  信息加密
 * @param str  字符串
 *
 */
QByteArray Common::info_encrypt(QString str)
{
    QByteArray text = str.toLocal8Bit();
    QByteArray by = text.toBase64();
    return by;
}

/**
 * @brief Common::info_decode  信息解密
 * @param str  字符串
 *
 */
QString Common::info_decode(QByteArray by)
{
    QByteArray text = by.fromBase64(by);
    QString str = QString::fromLocal8Bit(text);
    return str;
}

Common::~Common()
{
}
