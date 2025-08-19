// #include <QtExtraSpinBox/quint32spinbox.hpp>

#include <QApplication>
#include <QFormLayout>
#include <QMainWindow>
#include <QSpinBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto *mainWindow = new QMainWindow();
    mainWindow->setMinimumSize(400, 400);
    mainWindow->setWindowTitle("QtExtraSpinBox Example");

    // auto *scrollArea = new QScrollArea(mainWindow);
    // mainWindow->setCentralWidget(scrollArea);

    // auto *contentWidget = new QWidget(scrollArea);
    // scrollArea->setWidget(contentWidget);

    auto *contentWidget = new QWidget(mainWindow);
    mainWindow->setCentralWidget(contentWidget);

    auto *formLayout = new QFormLayout(contentWidget);
    contentWidget->setLayout(formLayout);

    // auto *spinBox1 = new QtExtraSpinBox::QUInt32SpinBox(contentWidget);
    // spinBox1->setMinimumWidth(100);
    // formLayout->addRow("UInt32 - 1", spinBox1);

    // QObject::connect(spinBox1, &QtExtraSpinBox::QUInt32SpinBox::valueChanged, spinBox1, [](quint32 value) {
    //     qInfo() << value;
    // });

    auto *spinBox2 = new QSpinBox(contentWidget);
    spinBox2->setMinimumWidth(100);
    formLayout->addRow("UInt32 - 2", spinBox2);

    mainWindow->show();

    return app.exec();
}
