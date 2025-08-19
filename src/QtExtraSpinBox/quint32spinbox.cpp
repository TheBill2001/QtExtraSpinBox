// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "QtExtraSpinBox/quint32spinbox.hpp"

#include "QtExtraSpinBox/private/qextraspinbox_p.hpp"

namespace QtExtraSpinBox
{
class QUInt32SpinBoxPrivate : public QExtraSpinBoxPrivate<QUInt32SpinBox::Type>
{
    Q_DISABLE_COPY(QUInt32SpinBoxPrivate)
    Q_DECLARE_PUBLIC(QUInt32SpinBox)
public:
    explicit QUInt32SpinBoxPrivate(QUInt32SpinBox *qq)
        : QExtraSpinBoxPrivate{qq}
    {
    }

protected:
    QLineEdit *q_edit() const override
    {
        Q_Q(const QUInt32SpinBox);
        return q->lineEdit();
    }

    void q_initStyleOption(QStyleOptionSpinBox *option) const override
    {
        Q_Q(const QUInt32SpinBox);
        return q->initStyleOption(option);
    }

    QAbstractSpinBox::StepEnabled q_stepEnabled() const override
    {
        Q_Q(const QUInt32SpinBox);
        return q->stepEnabled();
    }

    QValidator::State q_validate(QString &input, int &pos) const override
    {
        Q_Q(const QUInt32SpinBox);
        return q->validate(input, pos);
    }

    QString textFromValue(QUInt32SpinBox::Type value) const override
    {
        Q_Q(const QUInt32SpinBox);
        return q->textFromValue(value);
    }

    QUInt32SpinBox::Type valueFromText(const QString &text) const override
    {
        Q_Q(const QUInt32SpinBox);
        return q->valueFromText(text);
    }

    void emitSignalsImpl() override
    {
        Q_Q(QUInt32SpinBox);
        Q_EMIT q->textChanged(q_edit()->displayText());
        Q_EMIT q->valueChanged(value);
    }
};

QUInt32SpinBox::QUInt32SpinBox(QWidget *parent)
    : QAbstractSpinBox{parent}
    , d_ptr{new QUInt32SpinBoxPrivate(this)}
{
    Q_D(QUInt32SpinBox);
    d->init();
}

QUInt32SpinBox::~QUInt32SpinBox() = default;

QString QUInt32SpinBox::cleanText() const
{
    Q_D(const QUInt32SpinBox);
    return d->cleanText();
}

QUInt32SpinBox::Type QUInt32SpinBox::minimum() const
{
    Q_D(const QUInt32SpinBox);
    return d->minimum;
}

void QUInt32SpinBox::setMinimum(Type minimum)
{
    Q_D(QUInt32SpinBox);
    d->setMinimum(minimum);
}

QUInt32SpinBox::Type QUInt32SpinBox::maximum() const
{
    Q_D(const QUInt32SpinBox);
    return d->maximum;
}

void QUInt32SpinBox::setMaximum(Type maximum)
{
    Q_D(QUInt32SpinBox);
    d->setMaximum(maximum);
}

void QUInt32SpinBox::setRange(Type minimum, Type maximum)
{
    Q_D(QUInt32SpinBox);
    d->setRange(minimum, maximum);
}

QUInt32SpinBox::Type QUInt32SpinBox::singleStep() const
{
    Q_D(const QUInt32SpinBox);
    return d->singleStep;
}

void QUInt32SpinBox::setSingleStep(Type value)
{
    Q_D(QUInt32SpinBox);
    d->setSingleStep(value);
}

int QUInt32SpinBox::displayIntegerBase() const
{
    Q_D(const QUInt32SpinBox);
    return d->displayIntegerBase;
}

void QUInt32SpinBox::setDisplayIntegerBase(int base)
{
    Q_D(QUInt32SpinBox);
    d->setDisplayIntegerBase(base);
}

QString QUInt32SpinBox::prefix() const
{
    Q_D(const QUInt32SpinBox);
    return d->prefix;
}

void QUInt32SpinBox::setPrefix(const QString &prefix)
{
    Q_D(QUInt32SpinBox);
    d->setPrefix(prefix);
}

QString QUInt32SpinBox::suffix() const
{
    Q_D(const QUInt32SpinBox);
    return d->suffix;
}

void QUInt32SpinBox::setSuffix(const QString &suffix)
{
    Q_D(QUInt32SpinBox);
    d->setSuffix(suffix);
}

QAbstractSpinBox::StepType QUInt32SpinBox::stepType() const
{
    Q_D(const QUInt32SpinBox);
    return d->stepType;
}

void QUInt32SpinBox::setStepType(StepType stepType)
{
    Q_D(QUInt32SpinBox);
    d->stepType = stepType;
}

void QUInt32SpinBox::fixup(QString &input) const
{
    Q_D(const QUInt32SpinBox);
    d->fixup(input);
}

void QUInt32SpinBox::stepBy(int steps)
{
    Q_D(QUInt32SpinBox);
    d->stepBy(steps);
}

QValidator::State QUInt32SpinBox::validate(QString &input, int &pos) const
{
    Q_D(const QUInt32SpinBox);
    return d->validateImpl(input, pos);
}

bool QUInt32SpinBox::event(QEvent *event)
{
    Q_D(QUInt32SpinBox);
    auto ret = d->event(event);
    if (ret.has_value()) {
        return ret.value();
    }
    return QWidget::event(event);
}

QUInt32SpinBox::Type QUInt32SpinBox::value() const
{
    Q_D(const QUInt32SpinBox);
    return d->value;
}

QSize QUInt32SpinBox::minimumSizeHint() const
{
    Q_D(const QUInt32SpinBox);
    return d->minimumSizeHint();
}

QSize QUInt32SpinBox::sizeHint() const
{
    Q_D(const QUInt32SpinBox);
    return d->sizeHint();
}

void QUInt32SpinBox::clear()
{
    Q_D(QUInt32SpinBox);
    d->clear();
}

void QUInt32SpinBox::setValue(Type value)
{
    Q_D(QUInt32SpinBox);
    d->setValue(value, EmitIfChanged);
}

QString QUInt32SpinBox::textFromValue(Type value) const
{
    Q_D(const QUInt32SpinBox);
    return d->textFromValueImpl(value);
}

void QUInt32SpinBox::initStyleOption(QStyleOptionSpinBox *option) const
{
    Q_D(const QUInt32SpinBox);
    return d->initStyleOptionImpl(option);
}

QAbstractSpinBox::StepEnabled QUInt32SpinBox::stepEnabled() const
{
    Q_D(const QUInt32SpinBox);
    return d->stepEnabledImpl();
}

QUInt32SpinBox::Type QUInt32SpinBox::valueFromText(const QString &text) const
{
    Q_D(const QUInt32SpinBox);
    return d->valueFromTextImpl(text);
}

void QUInt32SpinBox::changeEvent(QEvent *event)
{
    Q_D(QUInt32SpinBox);
    d->changeEvent(event);
    QWidget::changeEvent(event);
}

void QUInt32SpinBox::closeEvent(QCloseEvent *event)
{
    Q_D(QUInt32SpinBox);
    d->closeEvent(event);
    QWidget::closeEvent(event);
}

#if QT_CONFIG(contextmenu)
void QUInt32SpinBox::contextMenuEvent(QContextMenuEvent *event)
{
    Q_D(QUInt32SpinBox);
    d->contextMenuEvent(event);
    QWidget::contextMenuEvent(event);
}
#endif // QT_CONFIG(contextmenu)

void QUInt32SpinBox::focusOutEvent(QFocusEvent *event)
{
    Q_D(QUInt32SpinBox);
    d->focusOutEvent(event);
    QWidget::focusOutEvent(event);
    Q_EMIT editingFinished();
}

void QUInt32SpinBox::hideEvent(QHideEvent *event)
{
    Q_D(QUInt32SpinBox);
    d->hideEvent(event);
    QWidget::hideEvent(event);
}

void QUInt32SpinBox::keyPressEvent(QKeyEvent *event)
{
    Q_D(QUInt32SpinBox);
    d->keyPressEvent(event);
}

void QUInt32SpinBox::keyReleaseEvent(QKeyEvent *event)
{
    Q_D(QUInt32SpinBox);
    d->keyReleaseEvent(event);
}

void QUInt32SpinBox::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(QUInt32SpinBox);
    d->mouseMoveEvent(event);
}

void QUInt32SpinBox::mousePressEvent(QMouseEvent *event)
{
    Q_D(QUInt32SpinBox);
    d->mousePressEvent(event);
}

void QUInt32SpinBox::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(QUInt32SpinBox);
    d->mouseReleaseEvent(event);
}

void QUInt32SpinBox::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    Q_D(QUInt32SpinBox);
    d->resizeEvent(event);
}

void QUInt32SpinBox::showEvent(QShowEvent *event)
{
    Q_D(QUInt32SpinBox);
    d->showEvent(event);
}

void QUInt32SpinBox::timerEvent(QTimerEvent *event)
{
    Q_D(QUInt32SpinBox);
    d->timerEvent(event);
    QWidget::changeEvent(event);
}

#if QT_CONFIG(wheelevent)
void QUInt32SpinBox::wheelEvent(QWheelEvent *event)
{
    Q_D(QUInt32SpinBox);
    d->wheelEvent(event);
}
#endif // QT_CONFIG(wheelevent)
} // namespace QtExtraSpinBox
