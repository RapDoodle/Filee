#ifndef UTILS_H
#define UTILS_H

#include <QDir>
#include <QFile>
#include <QMap>

enum class FileType : char
{
    Image = 0x00,
    Audio = 0x01,
    Video = 0x02
};

const static QMap<QByteArray, QString> imageTypes = {
    {QByteArrayLiteral("\x47\x49\x46\x38\x37\x61"), "gif"},
    {QByteArrayLiteral("\x47\x49\x46\x38\x39\x61"), "gif"},
    {QByteArrayLiteral("\x89\x50\x4E\x47\x0D\x0A\x1A\x0A"), "png"},
    {QByteArrayLiteral("\x42\x4D"), "bmp"},
    {QByteArrayLiteral("\xFF\xD8\xFF"), "jpg"},
    {QByteArrayLiteral("\x00\x00\x00\x0C\x6A\x50\x20\x20"), "jpg"},
    {QByteArrayLiteral("\x49\x49\x2A\x00\x10\x00\x00\x00\x43\x52"), "cr2"}
};

const static QMap<QByteArray, QString> audioTypes = {
    {QByteArrayLiteral("\x49\x44\x33"), "mp3"},
    {QByteArrayLiteral("\x1A\x45\xDF\xA3"), "mka"},
    {QByteArrayLiteral("\x66\x4C\x61\x43"), "flac"},
    {QByteArrayLiteral("\x30\x26\xB2\x75\x8E\x66\xCF\x11\xA6\xD9\x00\xAA\x00\x62\xCE\x6C"), "wma"},
};

const static QMap<QByteArray, QString> videoTypes = {
    {QByteArrayLiteral("\x00\x00\x00\x18\x66\x74\x79\x70\x69\x73\x6F\x6D"), "mp4"},
    {QByteArrayLiteral("\x1A\x45\xDF\xA3"), "mkv"},
    {QByteArrayLiteral("\x4D\x4C\x56\x49"), "MLV"},
    {QByteArrayLiteral("\x00\x00\x01\xBA"), "mpeg"},
    {QByteArrayLiteral("\x47"), "mpeg"},
    {QByteArrayLiteral("\x00\x00\x01\xB3"), "mpeg"},
    {QByteArrayLiteral("\x30\x26\xB2\x75\x8E\x66\xCF\x11\xA6\xD9\x00\xAA\x00\x62\xCE\x6C"), "wmv"},
    {QByteArrayLiteral("\x46\x4C\x56"), "flv"},
};

class Utils
{
public:
    static QString androidFileNameParser(QString path);
private:
    static QString getExtension(QFile*, FileType);

};

#endif // UTILS_H
