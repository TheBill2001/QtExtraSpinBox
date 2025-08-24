// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "QtExtraSpinBoxes/quint64spinbox.hpp"

#include "QtExtraSpinBoxes/private/qextraspinbox_p.hpp"

namespace QtExtraSpinBoxes
{
class QUInt64SpinBoxPrivate : public QExtraSpinBoxPrivate<QUInt64SpinBox::Type>
{
    Q_DISABLE_COPY(QUInt64SpinBoxPrivate)
    Q_DECLARE_PUBLIC(QUInt64SpinBox)
public:
    explicit QUInt64SpinBoxPrivate(QUInt64SpinBox *qq)
        : QExtraSpinBoxPrivate{qq}
    {
    }

protected:
    QLineEdit *q_edit() const override
    {
        Q_Q(const QUInt64SpinBox);
        return q->lineEdit();
    }

    void q_initStyleOption(QStyleOptionSpinBox *option) const override
    {
        Q_Q(const QUInt64SpinBox);
        return q->initStyleOption(option);
    }

    QAbstractSpinBox::StepEnabled q_stepEnabled() const override
    {
        Q_Q(const QUInt64SpinBox);
        return q->stepEnabled();
    }

    QValidator::State q_validate(QString &input, int &pos) const override
    {
        Q_Q(const QUInt64SpinBox);
        return q->validate(input, pos);
    }

    QString textFromValue(QUInt64SpinBox::Type value) const override
    {
        Q_Q(const QUInt64SpinBox);
        return q->textFromValue(value);
    }

    QUInt64SpinBox::Type valueFromText(const QString &text) const override
    {
        Q_Q(const QUInt64SpinBox);
        return q->valueFromText(text);
    }

    void emitSignalsImpl() override
    {
        Q_Q(QUInt64SpinBox);
        Q_EMIT q->textChanged(q_edit()->displayText());
        Q_EMIT q->valueChanged(value);
    }
};

QUInt64SpinBox::QUInt64SpinBox(QWidget *parent)
    : QAbstractSpinBox{parent}
    , d_ptr{new QUInt64SpinBoxPrivate(this)}
{
    Q_D(QUInt64SpinBox);
    d->init();
}

QUInt64SpinBox::~QUInt64SpinBox() = default;

QString QUInt64SpinBox::cleanText() const
{
    Q_D(const QUInt64SpinBox);
    return d->cleanText();
}

QUInt64SpinBox::Type QUInt64SpinBox::minimum() const
{
    Q_D(const QUInt64SpinBox);
    return d->minimum;
}

void QUInt64SpinBox::setMinimum(Type minimum)
{
    Q_D(QUInt64SpinBox);
    d->setMinimum(minimum);
}

QUInt64SpinBox::Type QUInt64SpinBox::maximum() const
{
    Q_D(const QUInt64SpinBox);
    return d->maximum;
}

void QUInt64SpinBox::setMaximum(Type maximum)
{
    Q_D(QUInt64SpinBox);
    d->setMaximum(maximum);
}

void QUInt64SpinBox::setRange(Type minimum, Type maximum)
{
    Q_D(QUInt64SpinBox);
    d->setRange(minimum, maximum);
}

QUInt64SpinBox::Type QUInt64SpinBox::singleStep() const
{
    Q_D(const QUInt64SpinBox);
    return d->singleStep;
}

void QUInt64SpinBox::setSingleStep(Type value)
{
    Q_D(QUInt64SpinBox);
    d->setSingleStep(value);
}

int QUInt64SpinBox::displayIntegerBase() const
{
    Q_D(const QUInt64SpinBox);
    return d->displayIntegerBase;
}

void QUInt64SpinBox::setDisplayIntegerBase(int base)
{
    Q_D(QUInt64SpinBox);
    d->setDisplayIntegerBase(base);
}

QString QUInt64SpinBox::prefix() const
{
    Q_D(const QUInt64SpinBox);
    return d->prefix;
}

void QUInt64SpinBox::setPrefix(const QString &prefix)
{
    Q_D(QUInt64SpinBox);
    d->setPrefix(prefix);
}

QString QUInt64SpinBox::suffix() const
{
    Q_D(const QUInt64SpinBox);
    return d->suffix;
}

void QUInt64SpinBox::setSuffix(const QString &suffix)
{
    Q_D(QUInt64SpinBox);
    d->setSuffix(suffix);
}

QAbstractSpinBox::StepType QUInt64SpinBox::stepType() const
{
    Q_D(const QUInt64SpinBox);
    return d->stepType;
}

void QUInt64SpinBox::setStepType(StepType stepType)
{
    Q_D(QUInt64SpinBox);
    d->stepType = stepType;
}

void QUInt64SpinBox::fixup(QString &input) const
{
    Q_D(const QUInt64SpinBox);
    d->fixup(input);
}

void QUInt64SpinBox::stepBy(int steps)
{
    Q_D(QUInt64SpinBox);
    d->stepBy(steps);
}

QValidator::State QUInt64SpinBox::validate(QString &input, int &pos) const
{
    Q_D(const QUInt64SpinBox);
    return d->validateImpl(input, pos);
}

bool QUInt64SpinBox::event(QEvent *event)
{
    Q_D(QUInt64SpinBox);
    auto ret = d->event(event);
    if (ret.has_value()) {
        return ret.value();
    }
    return QWidget::event(event);
}

QUInt64SpinBox::Type QUInt64SpinBox::value() const
{
    Q_D(const QUInt64SpinBox);
    return d->value;
}

QSize QUInt64SpinBox::minimumSizeHint() const
{
    Q_D(const QUInt64SpinBox);
    return d->minimumSizeHint();
}

QSize QUInt64SpinBox::sizeHint() const
{
    Q_D(const QUInt64SpinBox);
    return d->sizeHint();
}

void QUInt64SpinBox::clear()
{
    Q_D(QUInt64SpinBox);
    d->clear();
}

void QUInt64SpinBox::setValue(Type value)
{
    Q_D(QUInt64SpinBox);
    d->setValue(value, EmitIfChanged);
}

QString QUInt64SpinBox::textFromValue(Type value) const
{
    Q_D(const QUInt64SpinBox);
    return d->textFromValueImpl(value);
}

void QUInt64SpinBox::initStyleOption(QStyleOptionSpinBox *option) const
{
    Q_D(const QUInt64SpinBox);
    return d->initStyleOptionImpl(option);
}

QAbstractSpinBox::StepEnabled QUInt64SpinBox::stepEnabled() const
{
    Q_D(const QUInt64SpinBox);
    return d->stepEnabledImpl();
}

QUInt64SpinBox::Type QUInt64SpinBox::valueFromText(const QString &text) const
{
    Q_D(const QUInt64SpinBox);
    return d->valueFromTextImpl(text);
}

void QUInt64SpinBox::changeEvent(QEvent *event)
{
    Q_D(QUInt64SpinBox);
    d->changeEvent(event);
    QWidget::changeEvent(event);
}

void QUInt64SpinBox::closeEvent(QCloseEvent *event)
{
    Q_D(QUInt64SpinBox);
    d->closeEvent(event);
    QWidget::closeEvent(event);
}

#if QT_CONFIG(contextmenu)
void QUInt64SpinBox::contextMenuEvent(QContextMenuEvent *event)
{
    Q_D(QUInt64SpinBox);
    d->contextMenuEvent(event);
    QWidget::contextMenuEvent(event);
}
#endif // QT_CONFIG(contextmenu)

void QUInt64SpinBox::focusOutEvent(QFocusEvent *event)
{
    Q_D(QUInt64SpinBox);
    d->focusOutEvent(event);
    QWidget::focusOutEvent(event);
    Q_EMIT editingFinished();
}

void QUInt64SpinBox::hideEvent(QHideEvent *event)
{
    Q_D(QUInt64SpinBox);
    d->hideEvent(event);
    QWidget::hideEvent(event);
}

void QUInt64SpinBox::keyPressEvent(QKeyEvent *event)
{
    Q_D(QUInt64SpinBox);
    d->keyPressEvent(event);
}

void QUInt64SpinBox::keyReleaseEvent(QKeyEvent *event)
{
    Q_D(QUInt64SpinBox);
    d->keyReleaseEvent(event);
}

void QUInt64SpinBox::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(QUInt64SpinBox);
    d->mouseMoveEvent(event);
}

void QUInt64SpinBox::mousePressEvent(QMouseEvent *event)
{
    Q_D(QUInt64SpinBox);
    d->mousePressEvent(event);
}

void QUInt64SpinBox::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(QUInt64SpinBox);
    d->mouseReleaseEvent(event);
}

void QUInt64SpinBox::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    Q_D(QUInt64SpinBox);
    d->resizeEvent(event);
}

void QUInt64SpinBox::showEvent(QShowEvent *event)
{
    Q_D(QUInt64SpinBox);
    d->showEvent(event);
}

void QUInt64SpinBox::timerEvent(QTimerEvent *event)
{
    Q_D(QUInt64SpinBox);
    d->timerEvent(event);
    QWidget::changeEvent(event);
}

#if QT_CONFIG(wheelevent)
void QUInt64SpinBox::wheelEvent(QWheelEvent *event)
{
    Q_D(QUInt64SpinBox);
    d->wheelEvent(event);
}
#endif // QT_CONFIG(wheelevent)
} // namespace QtExtraSpinBoxes
