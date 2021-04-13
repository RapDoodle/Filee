#ifndef COMMON_H
#define COMMON_H

#include <QRandomGenerator>
#include <QStringList>
#include <QNetworkInterface>
#include <QTime>

const static QStringList sizeUnits = { "bytes", "KB", "MB", "GB", "TB", "PB" };

const static QStringList toneAdjectives = {"Agreeable", "Animated", "Bright", "Clever", "Encouraging",
                                           "Fresh", "Gentle", "Hopeful", "Kind", "Loving", "Open",
                                           "Pleased", "Supportive", "Sympathetic", "Warm", "Annoyed",
                                           "Bitter", "Disgruntled", "Disgusted", "Evil", "Guilty",
                                           "Hostile", "Hurtful", "Nasty", "Obnoxious", "Oppressive",
                                           "Overbearing", "Resentful", "Sarcastic", "Sardonic", "Ambivalent",
                                           "Anxious", "Bashful", "Candid", "Cautious", "Horrified", "Intelligent",
                                           "Mysterious", "Pragmatic", "Political", "Quizzical", "Religious",
                                           "Secretive", "Secular", "Strong"};

const static QStringList animalNames = {"Aardvark", "Ainu", "Akbash", "Akita", "Albatross", "Alligator", "Alpaca",
                                        "Anchovies", "Angelfish", "Ant", "Anteater", "Antelope", "Arapaima", "Armadillo",
                                        "Aurochs", "Aussiedor", "Avocet", "Axolotl", "Aye Aye", "Baboon",
                                        "Badger", "Baiji", "Balinese", "Bandicoot", "Barb", "Barn Owl",
                                        "Barnacle", "Barracuda", "Bassador", "Bat", "Beagador", "Beagle",
                                        "Bear", "Beaver", "Beetle", "Bichir", "Binturong", "Bird", "Birman",
                                        "Bison", "Blobfish", "Blue Jay", "Bobcat", "Bombay", "Bongo", "Bonobo",
                                        "Booby", "Borador", "Bordoodle", "Bowfin", "Boxador", "Boxer Dog",
                                        "Buffalo", "Bulldog", "Bullfrog", "Bumblebee", "Burmese", "Butterfly",
                                        "Caiman", "Camel", "Capybara", "Caracal", "Carp", "Cassowary", "Cat",
                                        "Catfish", "Cavador", "Cavapoo", "Centipede", "Chameleon", "Chamois",
                                        "Cheetah", "Chicken", "Chihuahua", "Chimaera", "Chinook", "Chipmunk",
                                        "Chipoo", "Chow Chow", "Cichlid", "Clownfish", "Coati", "Cockapoo",
                                        "Cockatoo", "Cockroach", "Codfish", "Collie", "Coral", "Corgidor",
                                        "Corgipoo", "Cougar", "Cow", "Coyote", "Crab", "Crane", "Crocodile",
                                        "Cuscus", "Dachsador", "Dachshund", "Dalmador", "Dalmatian", "Deer",
                                        "Dhole", "Dingo", "Discus", "Dodo", "Dog", "Dolphin", "Donkey", "Dormouse",
                                        "Doxiepoo", "Dragonfly", "Drever", "Drum Fish", "Duck", "Dugong", "Dunker",
                                        "Eagle", "Earwig", "Echidna", "Eel", "Elephant", "Emu", "Ermine", "Eskipoo",
                                        "Falcon", "Fangtooth", "Ferret", "Fin Whale", "Fish", "Flamingo", "Flounder",
                                        "Fly", "Fossa", "Fox", "Frog", "Fur Seal", "Gar", "Gecko", "Gerbil", "Gharial",
                                        "Gibbon", "Giraffe", "Glow Worm", "Goat", "Goldador", "Goose", "Gopher", "Gorilla",
                                        "Grey Seal", "Greyhound", "Grouse", "Guppy", "Hagfish", "Hamster", "Hare", "Harrier",
                                        "Havanese", "Havapoo", "Hedgehog", "Heron", "Herring", "Himalayan", "Honey Bee",
                                        "Hoopoe", "Hornbill", "Horse", "Human", "Huskador", "Hyena", "Ibis", "Iguana",
                                        "Impala", "Indri", "Insects", "Jackal", "Jaguar", "Javanese", "Jellyfish",
                                        "Jerboa", "Kakapo", "Kangaroo", "Keeshond", "King Crab", "Kinkajou", "Kiwi",
                                        "Koala", "Krill", "Kudu", "Labradane", "Ladybug", "Lamprey", "Lemming",
                                        "Lemur", "Leopard", "Lhasapoo", "Liger", "Lion", "Lionfish", "Lizard",
                                        "Llama", "Loach", "Lobster", "Lungfish", "Lynx", "Macaw", "Magpie", "Maltese",
                                        "Maltipoo", "Manatee", "Mandrill", "Manta Ray", "Markhor", "Marmot", "Mastador",
                                        "Mastiff", "Mayfly", "Meerkat", "Megalodon", "Milkfish", "Millipede", "Mink",
                                        "Mole", "Molly", "Mongoose", "Mongrel", "Monkey", "Monkfish", "Moorhen",
                                        "Moose", "Moray Eel", "Moth", "Mouse", "Mule", "Muskox", "Muskrat", "Narwhal",
                                        "Newfypoo", "Newt", "Numbat", "Ocelot", "Octopus", "Okapi", "Olm", "Opossum",
                                        "Ostrich", "Otter", "Oyster", "Pademelon", "Pangolin", "Panther", "Parrot",
                                        "Peacock", "Peekapoo", "Pekingese", "Pelican", "Penguin", "Persian", "Pheasant",
                                        "Pig", "Pigeon", "Pika", "Pike Fish", "Piranha", "Pitador", "Platypus", "Pointer",
                                        "Pomapoo", "Poochon", "Poodle", "Poogle", "Pool Frog", "Porcupine", "Porpoise",
                                        "Possum", "Prawn", "Puffin", "Pug", "Pugapoo", "Puma", "Puss Moth", "Pyrador",
                                        "Quail", "Quetzal", "Quokka", "Quoll", "Rabbit", "Raccoon", "Ragdoll", "Rat",
                                        "Red Finch", "Red Fox", "Red Panda", "Red Wolf", "Reindeer", "Robin", "Rockfish",
                                        "Sable", "Saiga", "Salmon", "Samoyed", "Saola", "Sardines", "Sawfish", "Schnoodle",
                                        "Scorpion", "Sea Lion", "Sea Otter", "Sea Slug", "Seahorse", "Seal", "Serval",
                                        "Shark", "Sheep", "Shiba Inu", "Shih Poo", "Shih Tzu", "Shrimp", "Siamese",
                                        "Siberian", "Siberpoo", "Skunk", "Sloth", "Slow Worm", "Snail", "Snake", "Snowshoe",
                                        "Snowy Owl", "Somali", "Spanador", "Sparrow", "Sponge", "Squid", "Squirrel",
                                        "Starfish", "Stingray", "Stoat", "Sturgeon", "Sun Bear", "Swai Fish", "Swan",
                                        "Tamaskan", "Tang", "Tapir", "Tarpon", "Tarsier", "Tawny Owl", "Termite",
                                        "Terrier", "Tetra", "Tiffany", "Tiger", "Toadfish", "Tortoise", "Toucan",
                                        "Tree Frog", "Tuatara", "Tuna", "Turkey", "Uakari", "Uguisu", "Utonagan",
                                        "Vaquita", "Vulture", "Wallaby", "Walrus", "Warthog", "Wasp", "Weasel",
                                        "Westiepoo", "Whippet", "Whoodle", "Wild Boar", "Wolf", "Wolf Eel", "Wolffish",
                                        "Wolverine", "Wombat", "Wood Frog", "Woodlouse", "Wrasse", "Xerus", "Yak",
                                        "Zebra", "Zebu", "Zonkey", "Zorse"};

class Common
{
public:
    static QString randomName();
    static QList<QHostAddress> getLocalIpAddresses();
    static QString humanReadableSize(qint64);
};

#endif // COMMON_H
