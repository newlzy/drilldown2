#include "imageitem.h"


ImageItem::ImageItem(int id, const QPixmap &pixmap, QGraphicsItem *parent):QGraphicsPixmapItem(pixmap,parent)
{
    recordId = id;
    setAcceptHoverEvents(true); //接受悬停事件
    timeLine.setDuration(150); //时间轴的总持续时间(以毫秒为单位)。
    //设置时间轴的帧计数器。
    timeLine.setFrameRange(0,150);

    //时间轴在运行状态时定期发出frameChanged信号，但只有当当前帧发生变化时才会发出。
    connect(&timeLine,&QTimeLine::frameChanged,this,&ImageItem::setFrame);

    //finished信号在QTimeLine结束时发出(即，到达它的时间行结束时)，并且不循环。
    connect(&timeLine,&QTimeLine::finished,this,&ImageItem::updateItemPosition);
    adjust(); //缩放
}

//缩放图像
void ImageItem::adjust()
{
    //设置转换矩阵。fromScale创建对应x水平缩放和y垂直缩放的矩阵。boundingRect这个纯虚函数将项目的外部边界定义为一个矩形;所有绘制都必须被限制在一个项目的边界矩形内。
    setTransform(QTransform::fromScale(120/boundingRect().width(),120/boundingRect().height()));
}

int ImageItem::id() const
{
    return recordId;
}

//悬停输入事件
void ImageItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //时间轴的方向。时间轴的当前时间随着时间的增加而增加(例如，从0到结束/持续时间)。
    timeLine.setDirection(QTimeLine::Forward);

    if(z != 1.0){
        z = 1.0;
        updateItemPosition();
    }
    //时间轴没用运行时
    if(timeLine.state() == QTimeLine::NotRunning)
        timeLine.start(); //启动时间。QTimeLine将进入运行状态，

}

//悬停离开事件
void ImageItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    //时间轴的当前时间随着时间减少(即，从结束/持续时间向0移动)。
    timeLine.setDirection(QTimeLine::Backward);
    if(z != 0.0)
        z = 0.0;

    //时间线没有运行时
    if(timeLine.state() == QTimeLine::NotRunning)
        timeLine.start(); //启动时间
}

void ImageItem::setFrame(int frame)
{
    adjust(); //缩放图像
    //center返回矩形的中心点。
    QPointF center = boundingRect().center();

    //fromTranslate创建一个矩阵后平移。
    setTransform(QTransform::fromTranslate(center.x(),center.y()),true);
    setTransform(QTransform::fromScale(1 + frame / 330.0, 1 + frame / 333.0),true);
    setTransform(QTransform::fromTranslate(-center.x(), - center.y()),true);
}

void ImageItem::updateItemPosition()
{
    //将项目的z值设置为z。z值决定兄弟(相邻)项目的堆叠顺序。一个高Z值的同级项目总是被绘制在另一个低Z值的同级项目上
    setZValue(z);
}
