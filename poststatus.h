#ifndef POSTSTATUS_H
#define POSTSTATUS_H
#include <QString>

class Poststatus
{
public:
    Poststatus(QString status) : status_(status) {}
    bool hatPost(int fach);

private:
    QString status_;
};

#endif // POSTSTATUS_H
