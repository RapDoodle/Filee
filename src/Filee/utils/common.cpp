#include "common.h"

QString Common::randomName()
{
    // Securely seeded generator.
    QRandomGenerator gen = QRandomGenerator::securelySeeded();

    quint32 toneIndex = gen.bounded(toneAdjectives.size());
    quint32 animalNamesIndex = gen.bounded(animalNames.size());

    return toneAdjectives.at(toneIndex) + " " + animalNames.at(animalNamesIndex);
}

QList<QHostAddress> Common::getLocalIpAddresses()
{
    QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    QList<QHostAddress> loopbackInterfaces;

    for (int i = 0; i < addresses.count(); i++) {
        if(!addresses[i].isLoopback())
            if (addresses[i].protocol() == QAbstractSocket::IPv4Protocol &&
                    !addresses[i].isInSubnet(QHostAddress::parseSubnet("169.254.0.0/16")))
                loopbackInterfaces.push_back(addresses[i]);
    }
    
    return loopbackInterfaces;
}

// Get the size that is human readable, for example, 1.61 GB
QString Common::humanReadableSize(qint64 fileSize)
{
    double size = fileSize;
    int unitIdx = 0;
    while (size > 1024) {
        if (unitIdx > sizeUnits.size())
            break;
        size /= 1024;
        unitIdx++;
    }
    return QString::number(size, 'f', 2) + " " + sizeUnits.at(unitIdx);
}
