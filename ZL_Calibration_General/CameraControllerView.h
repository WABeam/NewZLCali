#ifndef CAMERACONTROLLERVIEW_H
#define CAMERACONTROLLERVIEW_H

#include <QGraphicsWidget>
#include <QGraphicsItem>
#include <QGraphicsGridLayout>
#include "CameraOutput.h"
#include <QVector>

#include <QDebug>
class CameraOutputGrid : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit CameraOutputGrid(QGraphicsItem* parent = nullptr):
        QGraphicsWidget(parent),
        m_layout(new QGraphicsGridLayout(this))
    {
        setLayout(m_layout);
        int numChannels = 4;

        // Create CameraOutput instances for each channel and add them to the layout
        // int row = 0,col = 0;
        //        for (int i = 0; i < numChannels; ++i) {
        //            CameraOutput* channelOutput = new CameraOutput(this);
        //            m_outputs.append(channelOutput);
        //            //qDebug()<<row << "," << col;
        //            m_layout->addItem(channelOutput, i/numChannels, i%numChannels);
        //        }

        CameraOutput* channelOutput = new CameraOutput(this);
        CameraOutput* channelOutput1 = new CameraOutput(this);
        CameraOutput* channelOutput2 = new CameraOutput(this);
        CameraOutput* channelOutput3 = new CameraOutput(this);
        //m_outputs.append(channelOutput);
        //qDebug()<<row << "," << col;
        m_layout->addItem(channelOutput, 0, 0);
        m_layout->addItem(channelOutput1, 1, 1);
        m_layout->addItem(channelOutput2, 1, 0);
        m_layout->addItem(channelOutput3, 1, 1);


    }
    ~CameraOutputGrid()
    {
        qDeleteAll(m_outputs);
    }
private:
    QGraphicsGridLayout* m_layout;
    QVector<CameraOutput*> m_outputs;
};

#endif // CAMERACONTROLLERVIEW_H
