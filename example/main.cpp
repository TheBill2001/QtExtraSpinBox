#include <QtExtraSpinBox/QUInt32SpinBox>

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

    auto *contentWidget = new QWidget(mainWindow);
    mainWindow->setCentralWidget(contentWidget);

    auto *formLayout = new QFormLayout(contentWidget);
    contentWidget->setLayout(formLayout);

    auto *spinBox1 = new QtExtraSpinBox::QUInt32SpinBox(contentWidget);
    spinBox1->setMinimumWidth(100);
    spinBox1->setPrefix("Prefix ");
    spinBox1->setSuffix(" Suffix");
    spinBox1->setMaximum(std::numeric_limits<quint32>::max() - 1);
    spinBox1->setMinimum(std::numeric_limits<quint32>::min() + 1);
    spinBox1->setWrapping(true);
    spinBox1->setValue(std::numeric_limits<quint32>::max() - 1);
    spinBox1->setSingleStep(100);
    formLayout->addRow("UInt32 - 1", spinBox1);

    QObject::connect(spinBox1, &QtExtraSpinBox::QUInt32SpinBox::valueChanged, spinBox1, [](quint32 value) {
        qInfo() << value;
    });

    auto *spinBox2 = new QSpinBox(contentWidget);
    spinBox2->setMinimumWidth(100);
    spinBox2->setPrefix("Prefix ");
    spinBox2->setSuffix(" Suffix");
    spinBox2->setMaximum(std::numeric_limits<qint32>::max() - 1);
    spinBox2->setMinimum(std::numeric_limits<qint32>::min() + 1);
    spinBox2->setValue(std::numeric_limits<qint32>::min() + 1);
    spinBox2->setWrapping(true);
    formLayout->addRow("UInt32 - 2", spinBox2);
    QObject::connect(spinBox2, &QSpinBox::valueChanged, spinBox1, [](qint32 value) {
        qInfo() << value;
    });

    mainWindow->show();

    return app.exec();
}
