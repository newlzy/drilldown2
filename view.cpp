#include "view.h"
#include "imageitem.h"
#include "informationwindow.h"

View::View(const QString &items, const QString &images, QWidget *parent):QGraphicsView(parent)
{
    itemTable = new QSqlRelationalTableModel(this);
    itemTable->setTable(items);
    //指定列的数据由外键提供
    itemTable->setRelation(1,QSqlRelation(images,"itemid","file"));
    itemTable->select(); //用数据填充模型

    scene = new QGraphicsScene(this); //管理大量2D图形项的表面。场景
    scene->setSceneRect(0,0,465,365); //场景的边界矩形
    setScene(scene); //设置场景

    addItems(); //添加图形和数据

    //这样的窗口不能被拉大或拉小
    setMinimumSize(470,370); //小部件的最小大小
    setMaximumSize(470,370); //最大

    //线性梯度在开始点和结束点之间插值颜色。在这些点之外，根据当前设置的扩展方法，梯度可以被填充、反射或重复。
    QLinearGradient gradient(QPointF(0,0),QPointF(0,370));
    gradient.setColorAt(0,QColor("#868423"));
    gradient.setColorAt(1,QColor("#5d5b59"));
    setBackgroundBrush(gradient); //设置场景的背景刷
}

//鼠标释放
void View::mouseReleaseEvent(QMouseEvent *event)
{
    //返回位于pos位置的项
    if(QGraphicsItem *item = itemAt(event->pos())){
        if(ImageItem *image = qgraphicsitem_cast<ImageItem*>(item))
            showInformation(image);
    }
    QGraphicsView::mouseReleaseEvent(event); //事件传递
}

//提交修改后的数据
void View::updateImage(int id, const QString &fileName)
{
    //QGraphicsItem图形项的基类。scene返回现场所有项目的有序列表。顺序决定堆叠顺序。
    QList<QGraphicsItem*> items = scene->items();

    //这个函数是为STL兼容而提供的。它等价于isEmpty()，如果列表为空则返回true。
    while(!items.empty()){
        //删除列表中的第一项并返回它。
        QGraphicsItem *item = items.takeFirst();

        if(ImageItem *image = qgraphicsitem_cast<ImageItem*>(item)){
            if(image->id() == id){
                image->setPixmap(QPixmap(":/"+fileName));
                image->adjust();
                break;
            }
        }
    }
}

void View::addItems()
{
    //数据库返回查询的行数
    int itemCount = itemTable->rowCount();

    int imageOffset = 150; //图像偏移,第一排图片的原点与第二排的距离。
    int leftMargin = 70; //左边距
    int topMargin = 40; //顶部边距

    //每个图像
    for(int i = 0; i < itemCount; i++){
        //QSqlRecord封装一条数据库记录。record返回模型中的行记录
        QSqlRecord record = itemTable->record(i);

        //返回记录中名为id的字段的值
        int id = record.value("id").toInt();
        QString file = record.value("file").toString();
        QString item = record.value("itemtype").toString();

        int columnOffset = ((i % 2) * 37); //列偏移
        //小数取模大数等于它自己
        int x = ((i % 2) * imageOffset) + leftMargin + columnOffset;
        int y = ((i/2) * imageOffset) + topMargin;

        //自定义图形像类
        ImageItem *image = new ImageItem(id,QPixmap(":/"+file));
        image->setData(0,i); //将参数2自定义数据设为键值
        image->setPos(x,y);
        scene->addItem(image); //添加图像到场景

        //创建并添加一个文本项目到场景，并返回项目指针。
        QGraphicsTextItem *label = scene->addText(item);
        label->setDefaultTextColor(QColor("#d7d655")); //设置文本颜色
        //boundingRect项目外部的边界矩形。
        QPointF labelOffset((120 - label->boundingRect().width()) / 2,120.0);
        label->setPos(QPointF(x,y) + labelOffset); //文本的x,y
    }
}

//查找打开的数据修改窗口
InformationWindow *View::findWindow(int id) const
{
    for(auto window : informationWindows){
        if(window && (window->id() == id))
            return window;
    }
    return nullptr;
}

//鼠标释放后
void View::showInformation(ImageItem *image)
{
    int id  = image->id();
    if(id < 0 || id >= itemTable->rowCount())
        return;

    //修改数据的自定义窗口类
    InformationWindow *window = findWindow(id);
    if(!window){
        window = new InformationWindow(id,itemTable,this);

        //修改窗口点击提交按钮时发出imageChanged信号。
        connect(window,QOverload<int,const QString&>::of(&InformationWindow::imageChanged),this,QOverload<int,const QString&>::of(&View::updateImage));

        window->move(pos() + QPoint(20,40)); //移动
        window->show(); //显示修改窗口
        //这里实现打开多个修改窗口
        informationWindows.append(window);
    }

    if(window->isVisible()){ //小部件是否可见
        //将这个小部件提升到父小部件堆栈的顶部。在这个调用之后，小部件将在任何重叠的兄弟小部件前面显示出来。
        window->raise();
        //将包含此小部件的顶级小部件设置为活动窗口。
        window->activateWindow();
    }else
        window->show();

}
