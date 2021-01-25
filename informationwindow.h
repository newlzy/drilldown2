#ifndef INFORMATIONWINDOW_H
#define INFORMATIONWINDOW_H

#include <QDialog>
#include <QtSql>
#include <QtWidgets>

//修改数据的窗口类
class InformationWindow : public QDialog
{
    Q_OBJECT

public:
    InformationWindow(int id, QSqlRelationalTableModel *items, QWidget *parent = nullptr);

    int id() const;

signals:
    void imageChanged(int id, const QString &fileName);

private slots:
    void revert(); //恢复
    void submit(); //提交
    void enableButtons(bool enable = true); //不启动按钮


private:
    void createButtons(); //创建3个按钮和实现功能

    int itemId;
    QString displayedImage; //图片路径数据

    QComboBox *imageFileEditor = nullptr;
    QLabel *itemText = nullptr;
    QTextEdit *descriptionEditor = nullptr;

    QPushButton *closeButton = nullptr; //关闭
    QPushButton *submitButton = nullptr; //提交
    QPushButton *revertButton = nullptr; //恢复
    QDialogButtonBox *buttonBox = nullptr;

    //QDataWidgetMapper类提供了数据模型的一部分到小部件之间的映射。
    QDataWidgetMapper *mapper = nullptr;


};

#endif // INFORMATIONWINDOW_H
