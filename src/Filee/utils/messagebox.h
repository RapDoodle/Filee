#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QMessageBox>

class MessageBox
{
public:
    static void messageBoxInfo(QString);
    static void messageBoxWarning(QString);
    static void messageBoxCritical(QString);
};

#endif // MESSAGEBOX_H
