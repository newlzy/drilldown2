#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView> //一个用于显示QGraphicsScene内容的小部件。QGraphicsScene类提供了一个用于管理大量2D图形项的表面。
#include <QtSql>
#include <QtWidgets>

class ImageItem;
class InformationWindow; //自定义修改数据的窗口

class View : public QGraphicsView
{
    Q_OBJECT
public:
    View(const QString &items, const QString &images,QWidget *parent = nullptr);

protected:
    //鼠标释放
    void mouseReleaseEvent(QMouseEvent *event)override;

private slots:
    void updateImage(int id, const QString &fileName); //提交修改数据

private:
    void addItems(); //添加图像
    InformationWindow *findWindow(int id)const;
    //鼠标释放后
    void showInformation(ImageItem *image);

    QGraphicsScene *scene; //管理大量2D图形项的表面。
    //这是为了实现打开多个修改窗口
    QList<InformationWindow *> informationWindows;

    //为单个数据库表提供了一个可编辑的数据模型，并提供了外键支持
    QSqlRelationalTableModel *itemTable;

};

#endif // VIEW_H
