#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeView>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsTextItem>
#include <QStandardItemModel>
#include <QSplitter>
#include <QMessageBox>

#include "CameraControllerView.h"
class ChannelViewerWidget:public QWidget{

    Q_OBJECT


public:
    explicit ChannelViewerWidget(QWidget* parent = nullptr) :QWidget(parent)
    {
        layout = new QHBoxLayout(this);
        treeView = new QTreeView(this);
        graphicsView = new QGraphicsView(this);//右边图像显示通道
        graphicsScene = new QGraphicsScene(this);
        model = new QStandardItemModel(this);


        auto outputGrid = std::make_unique<CameraOutputGrid>();
        graphicsScene->addItem(outputGrid.get());

        m_outputGrid = outputGrid.release();

        setupTreeView();
        graphicsView->setScene(graphicsScene);


        layout->addWidget(treeView,1);
        layout->addWidget(graphicsView,5);

        connect(graphicsScene, &QGraphicsScene::sceneRectChanged, m_outputGrid,
                       [this](const QRectF& rect) {
                           m_outputGrid->resize(rect.size());
                       });
        setLayout(layout);




    }
private slots:
    void handleTreeViewSelection(const QModelIndex& current)
    {
//        // Calculate preferred height for the text item
//        int textHeight = 20; // Adjust as needed

//        if (current.isValid()) {
//            QString channelName = model->data(current).toString();
//            graphicsScene->clear();

//            // Create text item for channel name
//            QGraphicsTextItem* textItem = new QGraphicsTextItem(channelName);
//            textItem->setPos(10, 10);
//            textItem->setTextWidth(180); // Adjust the width of the text
//            graphicsScene->addItem(textItem);

//            // Create widget for image display
//            QGraphicsWidget* imageWidget = new QGraphicsWidget;
//            imageWidget->setPos(10, 10 + textHeight + 10); // Position below text
//            imageWidget->setPreferredSize(200, 200 - textHeight - 10); // Adjust preferred size
//            graphicsScene->addItem(imageWidget);


//        }
    }
private:
    QHBoxLayout* layout;
    //QVBoxLayout* cameraout_layout;
    QTreeView* treeView;
    QGraphicsView* graphicsView;
    QGraphicsScene* graphicsScene;
    QStandardItemModel* model;
    CameraOutputGrid* m_outputGrid;

    void setupTreeView()
    {
        for (int i = 1; i <= 4; ++i) {
            QStandardItem* item = new QStandardItem(QString("CH %1").arg(i));
            model->appendRow(item);
        }
        treeView->setModel(model);

        connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged,
                this, &ChannelViewerWidget::handleTreeViewSelection);
    }
};
