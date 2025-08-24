// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "QtExtraSpinBoxes/qint32spinbox.hpp"

#ifndef QTEXTRASPINBOXES_32BIT_INT
#include "QtExtraSpinBoxes/private/qextraspinbox_p.hpp"

namespace QtExtraSpinBoxes
{
class QInt32SpinBoxPrivate : public QExtraSpinBoxPrivate<QInt32SpinBox::Type>
{
    Q_DISABLE_COPY(QInt32SpinBoxPrivate)
    Q_DECLARE_PUBLIC(QInt32SpinBox)
public:
    explicit QInt32SpinBoxPrivate(QInt32SpinBox *qq)
        : QExtraSpinBoxPrivate{qq}
    {
    }

protected:
    QLineEdit *q_edit() const override
    {
        Q_Q(const QInt32SpinBox);
        return q->lineEdit();
    }

    void q_initStyleOption(QStyleOptionSpinBox *option) const override
    {
        Q_Q(const QInt32SpinBox);
        return q->initStyleOption(option);
    }

    QAbstractSpinBox::StepEnabled q_stepEnabled() const override
    {
        Q_Q(const QInt32SpinBox);
        return q->stepEnabled();
    }

    QValidator::State q_validate(QString &input, int &pos) const override
    {
        Q_Q(const QInt32SpinBox);
        return q->validate(input, pos);
    }

    QString textFromValue(QInt32SpinBox::Type value) const override
    {
        Q_Q(const QInt32SpinBox);
        return q->textFromValue(value);
    }

    QInt32SpinBox::Type valueFromText(const QString &text) const override
    {
        Q_Q(const QInt32SpinBox);
        return q->valueFromText(text);
    }

    void emitSignalsImpl() override
    {
        Q_Q(QInt32SpinBox);
        Q_EMIT q->textChanged(q_edit()->displayText());
        Q_EMIT q->valueChanged(value);
    }
};

QInt32SpinBox::QInt32SpinBox(QWidget *parent)
    : QAbstractSpinBox{parent}
    , d_ptr{new QInt32SpinBoxPrivate(this)}
{
    Q_D(QInt32SpinBox);
    d->init();
}

QInt32SpinBox::~QInt32SpinBox() = default;

QString QInt32SpinBox::cleanText() const
{
    Q_D(const QInt32SpinBox);
    return d->cleanText();
}

QInt32SpinBox::Type QInt32SpinBox::minimum() const
{
    Q_D(const QInt32SpinBox);
    return d->minimum;
}

void QInt32SpinBox::setMinimum(Type minimum)
{
    Q_D(QInt32SpinBox);
    d->setMinimum(minimum);
}

QInt32SpinBox::Type QInt32SpinBox::maximum() const
{
    Q_D(const QInt32SpinBox);
    return d->maximum;
}

void QInt32SpinBox::setMaximum(Type maximum)
{
    Q_D(QInt32SpinBox);
    d->setMaximum(maximum);
}

void QInt32SpinBox::setRange(Type minimum, Type maximum)
{
    Q_D(QInt32SpinBox);
    d->setRange(minimum, maximum);
}

QInt32SpinBox::Type QInt32SpinBox::singleStep() const
{
    Q_D(const QInt32SpinBox);
    return d->singleStep;
}

void QInt32SpinBox::setSingleStep(Type value)
{
    Q_D(QInt32SpinBox);
    d->setSingleStep(value);
}

int QInt32SpinBox::displayIntegerBase() const
{
    Q_D(const QInt32SpinBox);
    return d->displayIntegerBase;
}

void QInt32SpinBox::setDisplayIntegerBase(int base)
{
    Q_D(QInt32SpinBox);
    d->setDisplayIntegerBase(base);
}

QString QInt32SpinBox::prefix() const
{
    Q_D(const QInt32SpinBox);
    return d->prefix;
}

void QInt32SpinBox::setPrefix(const QString &prefix)
{
    Q_D(QInt32SpinBox);
    d->setPrefix(prefix);
}

QString QInt32SpinBox::suffix() const
{
    Q_D(const QInt32SpinBox);
    return d->suffix;
}

void QInt32SpinBox::setSuffix(const QString &suffix)
{
    Q_D(QInt32SpinBox);
    d->setSuffix(suffix);
}

QAbstractSpinBox::StepType QInt32SpinBox::stepType() const
{
    Q_D(const QInt32SpinBox);
    return d->stepType;
}

void QInt32SpinBox::setStepType(StepType stepType)
{
    Q_D(QInt32SpinBox);
    d->stepType = stepType;
}

void QInt32SpinBox::fixup(QString &input) const
{
    Q_D(const QInt32SpinBox);
    d->fixup(input);
}

void QInt32SpinBox::stepBy(int steps)
{
    Q_D(QInt32SpinBox);
    d->stepBy(steps);
}

QValidator::State QInt32SpinBox::validate(QString &input, int &pos) const
{
    Q_D(const QInt32SpinBox);
    return d->validateImpl(input, pos);
}

bool QInt32SpinBox::event(QEvent *event)
{
    Q_D(QInt32SpinBox);
    auto ret = d->event(event);
    if (ret.has_value()) {
        return ret.value();
    }
    return QWidget::event(event);
}

QInt32SpinBox::Type QInt32SpinBox::value() const
{
    Q_D(const QInt32SpinBox);
    return d->value;
}

QSize QInt32SpinBox::minimumSizeHint() const
{
    Q_D(const QInt32SpinBox);
    return d->minimumSizeHint();
}

QSize QInt32SpinBox::sizeHint() const
{
    Q_D(const QInt32SpinBox);
    return d->sizeHint();
}

void QInt32SpinBox::clear()
{
    Q_D(QInt32SpinBox);
    d->clear();
}

void QInt32SpinBox::setValue(Type value)
{
    Q_D(QInt32SpinBox);
    d->setValue(value, EmitIfChanged);
}

QString QInt32SpinBox::textFromValue(Type value) const
{
    Q_D(const QInt32SpinBox);
    return d->textFromValueImpl(value);
}

void QInt32SpinBox::initStyleOption(QStyleOptionSpinBox *option) const
{
    Q_D(const QInt32SpinBox);
    return d->initStyleOptionImpl(option);
}

QAbstractSpinBox::StepEnabled QInt32SpinBox::stepEnabled() const
{
    Q_D(const QInt32SpinBox);
    return d->stepEnabledImpl();
}

QInt32SpinBox::Type QInt32SpinBox::valueFromText(const QString &text) const
{
    Q_D(const QInt32SpinBox);
    return d->valueFromTextImpl(text);
}

void QInt32SpinBox::changeEvent(QEvent *event)
{
    Q_D(QInt32SpinBox);
    d->changeEvent(event);
    QWidget::changeEvent(event);
}

void QInt32SpinBox::closeEvent(QCloseEvent *event)
{
    Q_D(QInt32SpinBox);
    d->closeEvent(event);
    QWidget::closeEvent(event);
}

#if QT_CONFIG(contextmenu)
void QInt32SpinBox::contextMenuEvent(QContextMenuEvent *event)
{
    Q_D(QInt32SpinBox);
    d->contextMenuEvent(event);
    QWidget::contextMenuEvent(event);
}
#endif // QT_CONFIG(contextmenu)

void QInt32SpinBox::focusOutEvent(QFocusEvent *event)
{
    Q_D(QInt32SpinBox);
    d->focusOutEvent(event);
    QWidget::focusOutEvent(event);
    Q_EMIT editingFinished();
}

void QInt32SpinBox::hideEvent(QHideEvent *event)
{
    Q_D(QInt32SpinBox);
    d->hideEvent(event);
    QWidget::hideEvent(event);
}

void QInt32SpinBox::keyPressEvent(QKeyEvent *event)
{
    Q_D(QInt32SpinBox);
    d->keyPressEvent(event);
}

void QInt32SpinBox::keyReleaseEvent(QKeyEvent *event)
{
    Q_D(QInt32SpinBox);
    d->keyReleaseEvent(event);
}

void QInt32SpinBox::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(QInt32SpinBox);
    d->mouseMoveEvent(event);
}

void QInt32SpinBox::mousePressEvent(QMouseEvent *event)
{
    Q_D(QInt32SpinBox);
    d->mousePressEvent(event);
}

void QInt32SpinBox::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(QInt32SpinBox);
    d->mouseReleaseEvent(event);
}

void QInt32SpinBox::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    Q_D(QInt32SpinBox);
    d->resizeEvent(event);
}

void QInt32SpinBox::showEvent(QShowEvent *event)
{
    Q_D(QInt32SpinBox);
    d->showEvent(event);
}

void QInt32SpinBox::timerEvent(QTimerEvent *event)
{
    Q_D(QInt32SpinBox);
    d->timerEvent(event);
    QWidget::timerEvent(event);
}

#if QT_CONFIG(wheelevent)
void QInt32SpinBox::wheelEvent(QWheelEvent *event)
{
    Q_D(QInt32SpinBox);
    d->wheelEvent(event);
}
#endif // QT_CONFIG(wheelevent)
} // namespace QtExtraSpinBoxes
#endif // QTEXTRASPINBOXES_32BIT_INT
