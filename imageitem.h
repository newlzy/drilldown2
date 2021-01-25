#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QtCore>
#include <QtWidgets/QGraphicsPixmapItem>

//要设置项目的图形映射，可以将QPixmap传递给QGraphicsPixmapItem的构造函数，或者调用setPixmap()函数。函数的作用是:返回当前的像素图。
class ImageItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    enum {Type = UserType + 1};

    ImageItem(int id, const QPixmap &pixmap, QGraphicsItem *parent = nullptr);

    //必须返回大于或等于UserType(65536)的值。
    int type() const override {return Type; }
    void adjust();
    int id() const;

protected:
    //悬停输入事件
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    //悬停离开事件
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private slots:
    void setFrame(int frame);
    void updateItemPosition();

private:
    QTimeLine timeLine; //供了一个控制动画的时间线
    int recordId;
    double z;

};

#endif // IMAGEITEM_H
