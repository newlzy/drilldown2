#include "informationwindow.h"
#pragma execution_character_set("utf-8")


InformationWindow::InformationWindow(int id, QSqlRelationalTableModel *items, QWidget *parent):QDialog(parent)
{
    QLabel *itemLabel = new QLabel(tr("项目:"));
    QLabel *descriptionLabel = new QLabel(tr("描述: "));
    QLabel *imageFileLabel = new QLabel(tr("图片文件: "));

    createButtons(); //创建3按钮

    itemText = new QLabel;
    descriptionEditor = new QTextEdit;

    imageFileEditor = new QComboBox;
    //设置模型。relationModel返回一个QSqlTableModel对象，用于访问列为外键的表
    imageFileEditor->setModel(items->relationModel(1));
    //模型中的列。返回模型中指定为file的列
    imageFileEditor->setModelColumn(items->relationModel(1)->fieldIndex("file"));

    mapper = new QDataWidgetMapper(this); //部件之间的映射
    mapper->setModel(items); //设置模型
    //在调用submit()之前，模型不会更新。
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    //将项委托设置为委托。委托将用于将数据从模型写入小部件，并将数据从小部件写入模型。
    mapper->setItemDelegate(new QSqlRelationalDelegate(mapper));
    mapper->addMapping(imageFileEditor,1); //图片文件
    mapper->addMapping(itemText,2,"text"); //项目
    mapper->addMapping(descriptionEditor,3);//描述
    //如果方向是水平的(默认)，则用来自索引处行的数据填充小部件，否则使用来自索引处列的数据。
    mapper->setCurrentIndex(id);

    //描述文本。每当文档的内容发生变化时发出信号。启动恢复和提交按钮
    connect(descriptionEditor,&QTextEdit::textChanged,[=](){
        enableButtons();
    });
    //图片数据修改后启动2按钮
    connect(imageFileEditor,QOverload<int>::of(&QComboBox::currentIndexChanged),[=](){
        enableButtons();
    });

    //管理输入小部件的表单及其相关标签。是一个方便的布局类，它以两列的形式布局它的子类。
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(itemLabel,itemText);
    formLayout->addRow(imageFileLabel, imageFileEditor);
    formLayout->addRow(descriptionLabel, descriptionEditor);

    //垂直排列小部件。
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);
    setLayout(layout);

    itemId = id;
    displayedImage = imageFileEditor->currentText();

    //标识：这种类型的小部件如果有父部件则是子部件
    setWindowFlag(Qt::Window);
    enableButtons(false);
    setWindowTitle(itemText->text()); //标题

}

int InformationWindow::id() const
{
    return itemId;
}

//恢复
void InformationWindow::revert()
{
    mapper->revert(); //用模型的当前数据重新填充所有小部件。所有未提交的更改都将丢失。
    enableButtons(false); //不启动按钮
}

//提交。文本修改后发出信号
void InformationWindow::submit()
{
    //报错当前文本
    QString newImage = (imageFileEditor->currentText());
    if(displayedImage != newImage){ //文本有修改
        displayedImage = newImage;
        //发射信号，里面有修改后的文本
        emit imageChanged(itemId,newImage);
    }

    //将映射的小部件的所有更改提交到模型。对于每个映射的部分，项目委托从小部件读取当前值，并在模型中设置它。最后，调用模型的submit()方法。
    mapper->submit();
    //此属性保存当前行或列。被修改的行
    mapper->setCurrentIndex(itemId);
    enableButtons(false); //不启动按钮
}

//不启动恢复和提交按钮。默认情况启动按钮
void InformationWindow::enableButtons(bool enable)
{
    //setEnabled默认为true。enable参数为false，小部件不启动
    revertButton->setEnabled(enable);
    submitButton->setEnabled(enable);
}

//创建按钮
void InformationWindow::createButtons()
{
    closeButton = new QPushButton(tr("&关闭"));
    revertButton = new QPushButton(tr("&恢复"));
    submitButton = new QPushButton(tr("&提交"));

    closeButton->setDefault(true); //为默认按钮

    //关闭
    connect(closeButton,&QPushButton::clicked,this,&InformationWindow::close);
    //恢复
    connect(revertButton, &QPushButton::clicked, this, &InformationWindow::revert);
    //提交
    connect(submitButton, &QPushButton::clicked, this, &InformationWindow::submit);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->addButton(submitButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(revertButton, QDialogButtonBox::ResetRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::RejectRole);

}
