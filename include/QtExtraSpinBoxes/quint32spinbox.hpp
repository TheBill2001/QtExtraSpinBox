// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef QTEXTRASPINBOXES_QUINT32SPINBOX_HPP
#define QTEXTRASPINBOXES_QUINT32SPINBOX_HPP

#include <QtExtraSpinBoxes/qtextraspinboxes_export.hpp>

#include <QAbstractSpinBox>

namespace QtExtraSpinBoxes
{
class QUInt32SpinBoxPrivate;
class QTEXTRASPINBOXES_EXPORT QUInt32SpinBox : public QAbstractSpinBox
{
    Q_OBJECT
    Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
    Q_PROPERTY(QString cleanText READ cleanText)
    Q_PROPERTY(QtExtraSpinBoxes::QUInt32SpinBox::Type minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(QtExtraSpinBoxes::QUInt32SpinBox::Type maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(QtExtraSpinBoxes::QUInt32SpinBox::Type singleStep READ singleStep WRITE setSingleStep)
    Q_PROPERTY(StepType stepType READ stepType WRITE setStepType)
    Q_PROPERTY(QtExtraSpinBoxes::QUInt32SpinBox::Type value READ value WRITE setValue NOTIFY valueChanged USER true)
    Q_PROPERTY(int displayIntegerBase READ displayIntegerBase WRITE setDisplayIntegerBase)
    Q_DISABLE_COPY(QUInt32SpinBox)
public:
    using Type = quint32;

    explicit QUInt32SpinBox(QWidget *parent = nullptr);
    ~QUInt32SpinBox() override;

    QString cleanText() const;

    Type minimum() const;
    void setMinimum(Type minimum);

    Type maximum() const;
    void setMaximum(Type maximum);

    void setRange(Type minimum, Type maximum);

    Type singleStep() const;
    void setSingleStep(Type value);

    int displayIntegerBase() const;
    void setDisplayIntegerBase(int base);

    QString prefix() const;
    void setPrefix(const QString &prefix);

    QString suffix() const;
    void setSuffix(const QString &suffix);

    QAbstractSpinBox::StepType stepType() const;
    void setStepType(StepType stepType);

    Type value() const;

    void fixup(QString &input) const override;
    void stepBy(int steps) override;
    QValidator::State validate(QString &input, int &pos) const override;

    bool event(QEvent *event) override;
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public Q_SLOTS:
    void clear() override;
    void setValue(QtExtraSpinBoxes::QUInt32SpinBox::Type value);

Q_SIGNALS:
    void textChanged(const QString &text);
    void valueChanged(QtExtraSpinBoxes::QUInt32SpinBox::Type value);

protected:
    virtual QString textFromValue(Type value) const;
    virtual Type valueFromText(const QString &text) const;

    void initStyleOption(QStyleOptionSpinBox *option) const override;
    QAbstractSpinBox::StepEnabled stepEnabled() const override;

    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

#if QT_CONFIG(contextmenu)
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_CONFIG(contextmenu)

    void focusOutEvent(QFocusEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif // QT_CONFIG(wheelevent)

private:
    Q_DECLARE_PRIVATE(QUInt32SpinBox)
    QScopedPointer<QUInt32SpinBoxPrivate> d_ptr;
};
} // namespace QtExtraSpinBoxes

#endif // QTEXTRASPINBOXES_QUINT32SPINBOX_HPP
