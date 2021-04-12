#include "common.h"

Common::Common()
{

}

QString Common::randomName()
{
    // Securely seeded generator.
    QRandomGenerator gen = QRandomGenerator::securelySeeded();

    quint32 toneIndex = gen.bounded(toneAdjectives.size());
    quint32 animalNamesIndex = gen.bounded(animalNames.size());

    return toneAdjectives.at(toneIndex) + " " + animalNames.at(animalNamesIndex);
}
