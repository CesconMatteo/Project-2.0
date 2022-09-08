#ifndef CHARTDATA_H
#define CHARTDATA_H

#include <QString>

class ChartData {
protected:
    QString name;
public:
    ChartData(const QString&);
    virtual ~ChartData();
    const QString& getName() const;
    virtual void setName (const QString&);
};

#endif
