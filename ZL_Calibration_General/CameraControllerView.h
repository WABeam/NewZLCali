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
        int row = 0,col = 0;
        for (int i = 0; i < numChannels; ++i) {
            CameraOutput* channelOutput = new CameraOutput(this);
            m_outputs.append(channelOutput);
            qDebug()<<row << "," << col;
            m_layout->addItem(channelOutput, row, col);
            col++;
            if(col==2)
            {
                row++;
                col=0;
            }
        }


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
