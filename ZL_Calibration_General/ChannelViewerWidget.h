#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeView>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QStandardItemModel>
#include <QSplitter>
#include <QMessageBox>
class ChannelViewerWidget:public QWidget{

    Q_OBJECT


public:
    explicit ChannelViewerWidget(QWidget* parent = nullptr) :QWidget(parent)
    {
        layout = new QVBoxLayout(this);
        QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
        treeView = new QTreeView(this);
        graphicsView = new QGraphicsView(this);
        graphicsScene = new QGraphicsScene(this);
        model = new QStandardItemModel(this);

        setupTreeView();
        setupGraphicsView();

        // Set minimum and maximum width for QTreeView
        treeView->setMinimumWidth(200); // Adjust as needed
        treeView->setMaximumWidth(200); // Adjust as needed

        splitter->addWidget(treeView);
        splitter->addWidget(graphicsView);

        layout->addWidget(splitter);
        setLayout(layout);
    }
private slots:
    void handleTreeViewSelection(const QModelIndex& current)
    {
        if (current.isValid()) {
            QString channelName = model->data(current).toString();
            graphicsScene->clear();
            QGraphicsTextItem* textItem = graphicsScene->addText(channelName);
            textItem->setPos(10, 10);
        }
    }
private:
    QVBoxLayout* layout;
    QTreeView* treeView;
    QGraphicsView* graphicsView;
    QGraphicsScene* graphicsScene;
    QStandardItemModel* model;

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
    void setupGraphicsView()
    {
        graphicsView->setScene(graphicsScene);
    }
};
