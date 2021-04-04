#ifndef FILESENDER_H
#define FILESENDER_H

#include <QObject>

class FileSender : public QObject
{
    Q_OBJECT
public:
    explicit FileSender(QObject *parent = nullptr);

signals:

};

#endif // FILESENDER_H
