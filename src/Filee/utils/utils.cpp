#include "utils.h"
#include <QDebug>

QString Utils::androidFileNameParser(QString path)
{
    QString name = QDir(path).dirName();

    // Try "absolute" path parsing
    // For example, home%3AS08E06.mp4
    // Split %2F(/) to get S08E06.mp4
    QStringList splitName = name.split("%2F");
    if (splitName.size() > 1) {
        return splitName.at(splitName.size() - 1);
    }

    // Not "absolute" path
    // For example, image%3A19275
    // Split %3A(:) to get 19275.(file extension from binary)
    splitName = name.split("%3A");
    if (splitName.size() == 2) {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly)) {
            QString type = splitName.at(0);
            name = splitName.at(1);
            QString extension;
            if (type == "image") {
                extension = getExtension(&file, FileType::Image);
            } else if (type == "video") {
                extension = getExtension(&file, FileType::Video);
            } else if (type == "audio") {
                extension = getExtension(&file, FileType::Audio);
            }
            if (extension.size() != 0)
                return name + "." + extension;
        }
    }

    return name;
}

QString Utils::getExtension(QFile* file, FileType fileType)
{
    qint64 pos = file->pos();
    file->seek(0);
    QByteArray header;
    // The maximum size of header in the supported files list
    header.resize(sizeof(char) * 18);
    file->read(header.data(), sizeof(char) * 18);

    // Restore to previous position
    file->seek(pos);

    const QMap<QByteArray, QString> *map = nullptr;
    switch(fileType) {
    case FileType::Image: {
        map = &imageTypes;
        break;
    }
    case FileType::Audio: {
        map = &audioTypes;
        break;
    }
    case FileType::Video: {
        map = &videoTypes;
        break;
    }
    default: {
        return "";
    }
    }

    QMapIterator<QByteArray, QString> i(*map);
    while (i.hasNext()) {
        i.next();
        if (header.startsWith(i.key()))
            return i.value();
    }

    return "";
}


