// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "QtExtraSpinBox/qint64spinbox.hpp"

#include "QtExtraSpinBox/private/qextraspinbox_p.hpp"

namespace QtExtraSpinBox
{
class QInt64SpinBoxPrivate : public QExtraSpinBoxPrivate<QInt64SpinBox::Type>
{
    Q_DISABLE_COPY(QInt64SpinBoxPrivate)
    Q_DECLARE_PUBLIC(QInt64SpinBox)
public:
    explicit QInt64SpinBoxPrivate(QInt64SpinBox *qq)
        : QExtraSpinBoxPrivate{qq}
    {
    }

protected:
    QLineEdit *q_edit() const override
    {
        Q_Q(const QInt64SpinBox);
        return q->lineEdit();
    }

    void q_initStyleOption(QStyleOptionSpinBox *option) const override
    {
        Q_Q(const QInt64SpinBox);
        return q->initStyleOption(option);
    }

    QAbstractSpinBox::StepEnabled q_stepEnabled() const override
    {
        Q_Q(const QInt64SpinBox);
        return q->stepEnabled();
    }

    QValidator::State q_validate(QString &input, int &pos) const override
    {
        Q_Q(const QInt64SpinBox);
        return q->validate(input, pos);
    }

    QString textFromValue(QInt64SpinBox::Type value) const override
    {
        Q_Q(const QInt64SpinBox);
        return q->textFromValue(value);
    }

    QInt64SpinBox::Type valueFromText(const QString &text) const override
    {
        Q_Q(const QInt64SpinBox);
        return q->valueFromText(text);
    }

    void emitSignalsImpl() override
    {
        Q_Q(QInt64SpinBox);
        Q_EMIT q->textChanged(q_edit()->displayText());
        Q_EMIT q->valueChanged(value);
    }
};

QInt64SpinBox::QInt64SpinBox(QWidget *parent)
    : QAbstractSpinBox{parent}
    , d_ptr{new QInt64SpinBoxPrivate(this)}
{
    Q_D(QInt64SpinBox);
    d->init();
}

QInt64SpinBox::~QInt64SpinBox() = default;

QString QInt64SpinBox::cleanText() const
{
    Q_D(const QInt64SpinBox);
    return d->cleanText();
}

QInt64SpinBox::Type QInt64SpinBox::minimum() const
{
    Q_D(const QInt64SpinBox);
    return d->minimum;
}

void QInt64SpinBox::setMinimum(Type minimum)
{
    Q_D(QInt64SpinBox);
    d->setMinimum(minimum);
}

QInt64SpinBox::Type QInt64SpinBox::maximum() const
{
    Q_D(const QInt64SpinBox);
    return d->maximum;
}

void QInt64SpinBox::setMaximum(Type maximum)
{
    Q_D(QInt64SpinBox);
    d->setMaximum(maximum);
}

void QInt64SpinBox::setRange(Type minimum, Type maximum)
{
    Q_D(QInt64SpinBox);
    d->setRange(minimum, maximum);
}

QInt64SpinBox::Type QInt64SpinBox::singleStep() const
{
    Q_D(const QInt64SpinBox);
    return d->singleStep;
}

void QInt64SpinBox::setSingleStep(Type value)
{
    Q_D(QInt64SpinBox);
    d->setSingleStep(value);
}

int QInt64SpinBox::displayIntegerBase() const
{
    Q_D(const QInt64SpinBox);
    return d->displayIntegerBase;
}

void QInt64SpinBox::setDisplayIntegerBase(int base)
{
    Q_D(QInt64SpinBox);
    d->setDisplayIntegerBase(base);
}

QString QInt64SpinBox::prefix() const
{
    Q_D(const QInt64SpinBox);
    return d->prefix;
}

void QInt64SpinBox::setPrefix(const QString &prefix)
{
    Q_D(QInt64SpinBox);
    d->setPrefix(prefix);
}

QString QInt64SpinBox::suffix() const
{
    Q_D(const QInt64SpinBox);
    return d->suffix;
}

void QInt64SpinBox::setSuffix(const QString &suffix)
{
    Q_D(QInt64SpinBox);
    d->setSuffix(suffix);
}

QAbstractSpinBox::StepType QInt64SpinBox::stepType() const
{
    Q_D(const QInt64SpinBox);
    return d->stepType;
}

void QInt64SpinBox::setStepType(StepType stepType)
{
    Q_D(QInt64SpinBox);
    d->stepType = stepType;
}

void QInt64SpinBox::fixup(QString &input) const
{
    Q_D(const QInt64SpinBox);
    d->fixup(input);
}

void QInt64SpinBox::stepBy(int steps)
{
    Q_D(QInt64SpinBox);
    d->stepBy(steps);
}

QValidator::State QInt64SpinBox::validate(QString &input, int &pos) const
{
    Q_D(const QInt64SpinBox);
    return d->validateImpl(input, pos);
}

bool QInt64SpinBox::event(QEvent *event)
{
    Q_D(QInt64SpinBox);
    auto ret = d->event(event);
    if (ret.has_value()) {
        return ret.value();
    }
    return QWidget::event(event);
}

QInt64SpinBox::Type QInt64SpinBox::value() const
{
    Q_D(const QInt64SpinBox);
    return d->value;
}

QSize QInt64SpinBox::minimumSizeHint() const
{
    Q_D(const QInt64SpinBox);
    return d->minimumSizeHint();
}

QSize QInt64SpinBox::sizeHint() const
{
    Q_D(const QInt64SpinBox);
    return d->sizeHint();
}

void QInt64SpinBox::clear()
{
    Q_D(QInt64SpinBox);
    d->clear();
}

void QInt64SpinBox::setValue(Type value)
{
    Q_D(QInt64SpinBox);
    d->setValue(value, EmitIfChanged);
}

QString QInt64SpinBox::textFromValue(Type value) const
{
    Q_D(const QInt64SpinBox);
    return d->textFromValueImpl(value);
}

void QInt64SpinBox::initStyleOption(QStyleOptionSpinBox *option) const
{
    Q_D(const QInt64SpinBox);
    return d->initStyleOptionImpl(option);
}

QAbstractSpinBox::StepEnabled QInt64SpinBox::stepEnabled() const
{
    Q_D(const QInt64SpinBox);
    return d->stepEnabledImpl();
}

QInt64SpinBox::Type QInt64SpinBox::valueFromText(const QString &text) const
{
    Q_D(const QInt64SpinBox);
    return d->valueFromTextImpl(text);
}

void QInt64SpinBox::changeEvent(QEvent *event)
{
    Q_D(QInt64SpinBox);
    d->changeEvent(event);
    QWidget::changeEvent(event);
}

void QInt64SpinBox::closeEvent(QCloseEvent *event)
{
    Q_D(QInt64SpinBox);
    d->closeEvent(event);
    QWidget::closeEvent(event);
}

#if QT_CONFIG(contextmenu)
void QInt64SpinBox::contextMenuEvent(QContextMenuEvent *event)
{
    Q_D(QInt64SpinBox);
    d->contextMenuEvent(event);
    QWidget::contextMenuEvent(event);
}
#endif // QT_CONFIG(contextmenu)

void QInt64SpinBox::focusOutEvent(QFocusEvent *event)
{
    Q_D(QInt64SpinBox);
    d->focusOutEvent(event);
    QWidget::focusOutEvent(event);
    Q_EMIT editingFinished();
}

void QInt64SpinBox::hideEvent(QHideEvent *event)
{
    Q_D(QInt64SpinBox);
    d->hideEvent(event);
    QWidget::hideEvent(event);
}

void QInt64SpinBox::keyPressEvent(QKeyEvent *event)
{
    Q_D(QInt64SpinBox);
    d->keyPressEvent(event);
}

void QInt64SpinBox::keyReleaseEvent(QKeyEvent *event)
{
    Q_D(QInt64SpinBox);
    d->keyReleaseEvent(event);
}

void QInt64SpinBox::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(QInt64SpinBox);
    d->mouseMoveEvent(event);
}

void QInt64SpinBox::mousePressEvent(QMouseEvent *event)
{
    Q_D(QInt64SpinBox);
    d->mousePressEvent(event);
}

void QInt64SpinBox::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(QInt64SpinBox);
    d->mouseReleaseEvent(event);
}

void QInt64SpinBox::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    Q_D(QInt64SpinBox);
    d->resizeEvent(event);
}

void QInt64SpinBox::showEvent(QShowEvent *event)
{
    Q_D(QInt64SpinBox);
    d->showEvent(event);
}

void QInt64SpinBox::timerEvent(QTimerEvent *event)
{
    Q_D(QInt64SpinBox);
    d->timerEvent(event);
    QWidget::changeEvent(event);
}

#if QT_CONFIG(wheelevent)
void QInt64SpinBox::wheelEvent(QWheelEvent *event)
{
    Q_D(QInt64SpinBox);
    d->wheelEvent(event);
}
#endif // QT_CONFIG(wheelevent)
} // namespace QtExtraSpinBox
