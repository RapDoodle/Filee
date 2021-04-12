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
