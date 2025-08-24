// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#ifndef QTEXTRASPINBOXES_P_H
#define QTEXTRASPINBOXES_P_H

#include <optional>

#include <QAbstractSpinBox>
#include <QAccessibleValueChangeEvent>
#include <QBasicTimer>
#include <QContextMenuEvent>
#include <QEvent>
#include <QGuiApplication>
#include <QLineEdit>
#include <QMenu>
#include <QPointer>
#include <QStyleHints>
#include <QStyleOptionSpinBox>
#include <QTimerEvent>

#ifdef QTEXTRASPINBOXES_QSBDEBUG
#define QSBDEBUG qDebug
#else
#define QSBDEBUG                                                                                                                                               \
    if (false)                                                                                                                                                 \
    qDebug
#endif

using namespace std::chrono_literals;
using namespace Qt::Literals::StringLiterals;

namespace QtExtraSpinBoxes
{
enum EmitPolicy {
    EmitIfChanged,
    AlwaysEmit,
    NeverEmit
};

enum Button {
    None = 0x000,
    Keyboard = 0x001,
    Mouse = 0x002,
    Wheel = 0x004,
    ButtonMask = 0x008,
    Up = 0x010,
    Down = 0x020,
    DirectionMask = 0x040
};

namespace
{
template<class T>
struct dependent_false : std::false_type {
};
}

template<typename T, std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, bool> = true>
constexpr static T valueAdd(T a, T b) noexcept
{
    T r = a + b;
    if (r < a) {
        return std::numeric_limits<T>::max();
    }
    return r;
}

template<typename T, std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, bool> = true>
constexpr static T valueSub(T a, T b) noexcept
{
    if (a < b) {
        return 0;
    }
    return a - b;
}

template<typename T, std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, bool> = true>
constexpr static T valueMul(T a, T b) noexcept
{
    // If b == 0 should error?
    if (a == 0 || b == 0) {
        return 0;
    }
    if (a > std::numeric_limits<T>::max() / b) {
        return std::numeric_limits<T>::max();
    }
    return a * b;
}

template<typename T, std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, bool> = true>
constexpr static T valueAdd(T a, T b) noexcept
{
    if (b > 0) {
        if (a > std::numeric_limits<T>::max() - b) {
            return std::numeric_limits<T>::max();
        }
    } else {
        if (a < std::numeric_limits<T>::min() - b) {
            return std::numeric_limits<T>::min();
        }
    }
    return a + b;
}

template<typename T, std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, bool> = true>
constexpr static T valueSub(T a, T b) noexcept
{
    if (b > 0) {
        if (a < std::numeric_limits<T>::min() + b) {
            return std::numeric_limits<T>::min();
        }
    } else {
        if (a > std::numeric_limits<T>::max() + b) {
            return std::numeric_limits<T>::max();
        }
    }
    return a - b;
}

template<typename T, std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, bool> = true>
constexpr static T valueMul(T a, T b) noexcept
{
    // If b == 0 should error?
    if (a == 0 || b == 0) {
        return 0;
    }

    if (a > 0) {
        if (b > 0) {
            if (a > std::numeric_limits<T>::max() / b) {
                return std::numeric_limits<T>::max();
            }
        } else {
            if (b < std::numeric_limits<T>::min() / a) {
                return std::numeric_limits<T>::min();
            }
        }
    } else {
        if (b > 0) {
            if (a < std::numeric_limits<T>::min() / b) {
                return std::numeric_limits<T>::min();
            }
        } else {
            if (a < std::numeric_limits<T>::max() / b) {
                return std::numeric_limits<T>::max();
            }
        }
    }
    return a * b;
}

template<typename T>
class QExtraSpinBoxPrivate;

template<typename T>
class QExtraSpinBoxValidator : public QValidator
{
public:
    QExtraSpinBoxValidator(QAbstractSpinBox *qptr, QExtraSpinBoxPrivate<T> *dptr);

    QValidator::State validate(QString &input, int &pos) const override;

    void fixup(QString &input) const override;

private:
    QAbstractSpinBox *const qptr;
    QExtraSpinBoxPrivate<T> *const dptr;
};

template<typename T>
class QExtraSpinBoxPrivate
{
    Q_DISABLE_COPY(QExtraSpinBoxPrivate)
    Q_DECLARE_PUBLIC(QAbstractSpinBox)
public:
    T value = 0;
    T minimum = 0;
    T maximum = 99;
    T singleStep = 1;

    int displayIntegerBase = 10;

    QString prefix;
    QString suffix;

    QAbstractSpinBox::StepType stepType = QAbstractSpinBox::DefaultStepType;

    bool pendingEmit = false;
    bool cleared = false;
    bool ignoreCursorPositionChanged = false;
    bool ignoreUpdateEdit = false;

    mutable QSize cachedMinimumSizeHint;
    mutable QSize cachedSizeHint;
    mutable std::optional<T> cachedValue;
    mutable std::optional<QString> cachedText;
    mutable QValidator::State cachedState = QValidator::Invalid;

    Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
    Qt::KeyboardModifier stepModifier = Qt::ControlModifier;
    QStyle::SubControl hoverControl = QStyle::SC_None;

    Button buttonState = None;

    QRect hoverRect;

    QBasicTimer spinClickTimer;
    int spinClickTimerInterval = 100;
    QBasicTimer spinClickThresholdTimer;
    int spinClickThresholdTimerInterval = -1;
    int acceleration = 0;
    qreal effectiveSpinRepeatRate = 1;
    int wheelDeltaRemainder = 0;

    QExtraSpinBoxValidator<T> *validator = nullptr;

    QExtraSpinBoxPrivate(QAbstractSpinBox *qq)
        : q_ptr{qq}
    {
    }

    QString cleanText() const
    {
        return stripped(q_edit()->displayText());
    }

    virtual void init()
    {
        Q_Q(QAbstractSpinBox);
        q->setInputMethodHints(Qt::ImhDigitsOnly);

        validator = new QExtraSpinBoxValidator<T>(q, this);
        q_edit()->setValidator(validator);

        QStyleOptionSpinBox opt;
        q_initStyleOption(&opt);
        spinClickTimerInterval = q->style()->styleHint(QStyle::SH_SpinBox_ClickAutoRepeatRate, &opt, q);
        spinClickThresholdTimerInterval = q->style()->styleHint(QStyle::SH_SpinBox_ClickAutoRepeatThreshold, &opt, q);

        q->setFocusPolicy(Qt::WheelFocus);
        q->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed, QSizePolicy::SpinBox));
        q->setAttribute(Qt::WA_InputMethodEnabled);

        q->setAttribute(Qt::WA_MacShowFocusRect);

        QObject::connect(q_edit(), &QLineEdit::textChanged, q, [this](const QString &text) {
            editorTextChanged(text);
        });

        QObject::connect(q_edit(), &QLineEdit::cursorPositionChanged, q, [this](int oldPos, int newPos) {
            editorCursorPositionChanged(oldPos, newPos);
        });

        updateEdit();
    }

    bool specialValue() const
    {
        Q_Q(const QAbstractSpinBox);
        return (value == minimum && !q->specialValueText().isEmpty());
    }

    void setMinimum(T value)
    {
        setRange(value, maximum > value ? maximum : value);
    }

    void setMaximum(T value)
    {
        setRange(minimum < value ? minimum : value, value);
    }

    void setRange(T min, T max)
    {
        Q_Q(QAbstractSpinBox);

        clearCache();
        minimum = min;
        maximum = (min < max ? max : min);
        cachedSizeHint = QSize();
        cachedMinimumSizeHint = QSize(); // minimumSizeHint cares about min/max

        reset();
        if (const auto boundV = bound(value); boundV != value) {
            setValue(boundV, EmitIfChanged);
        } else if (value == minimum && !q->specialValueText().isEmpty()) {
            updateEdit();
        }

        q->updateGeometry();
    }

    void setSingleStep(quint32 value)
    {
        if (value >= 0) {
            singleStep = value;
            updateEdit();
        }
    }

    void setDisplayIntegerBase(int base)
    {
        // Falls back to base 10 on invalid bases (like QString)
        if (Q_UNLIKELY(base < 2 || base > 36)) {
            Q_Q(QAbstractSpinBox);
            qWarning("%s::setDisplayIntegerBase: Invalid base (%d)", q->metaObject()->className(), base);
            base = 10;
        }

        if (base != displayIntegerBase) {
            displayIntegerBase = base;
            updateEdit();
        }
    }

    void setPrefix(const QString &value)
    {
        prefix = value;
        updateEdit();

        cachedSizeHint = QSize();
        cachedMinimumSizeHint = QSize(); // minimumSizeHint cares about the prefix

        Q_Q(QAbstractSpinBox);
        q->updateGeometry();
    }

    void setSuffix(const QString &value)
    {
        suffix = value;
        updateEdit();

        cachedSizeHint = QSize();

        Q_Q(QAbstractSpinBox);
        q->updateGeometry();
    }

    void setValue(T val, EmitPolicy ep, bool doUpdate = true)
    {
        Q_Q(QAbstractSpinBox);

        const T old = value;
        value = bound(val);
        pendingEmit = false;
        cleared = false;

        if (doUpdate) {
            updateEdit();
        }

        q->update();

        if (ep == AlwaysEmit || (ep == EmitIfChanged && old != value)) {
            emitSignals(ep, old);
        }
    }

    void fixup(QString &input) const

    {
        Q_Q(const QAbstractSpinBox);
        if (!q->isGroupSeparatorShown()) {
            input.remove(q->locale().groupSeparator());
        }
    }

    void stepBy(int steps)
    {
        Q_Q(QAbstractSpinBox);

        const T old = value;
        QString tmp = q_edit()->displayText();
        int cursorPos = q_edit()->cursorPosition();
        bool dontstep = false;
        EmitPolicy e = EmitIfChanged;
        if (pendingEmit) {
            dontstep = q_validate(tmp, cursorPos) != QValidator::Acceptable;
            cleared = false;
            interpret(NeverEmit);
            if (value != old) {
                e = AlwaysEmit;
            }
        }

        if (!dontstep) {
            T singleStep;
            switch (stepType) {
            case QAbstractSpinBox::StepType::AdaptiveDecimalStepType:
                singleStep = calculateAdaptiveDecimalStep(steps);
                break;
            default:
                singleStep = this->singleStep;
            }
            if (steps >= 0) {
                setValue(bound(valueAdd(value, valueMul(singleStep, static_cast<T>(steps))), old, steps), e);
            } else {
                setValue(bound(valueSub(value, valueMul(singleStep, static_cast<T>(steps * -1))), old, steps), e);
            }
        } else if (e == AlwaysEmit) {
            emitSignals(e, old);
        }
        if (q->style()->styleHint(QStyle::SH_SpinBox_SelectOnStep, nullptr, q, nullptr)) {
            q->selectAll();
        }
    }

    QValidator::State validateImpl(QString &input, int &pos) const
    {
        QValidator::State state;
        validateAndInterpret(input, pos, state);
        return state;
    }

    QSize minimumSizeHint() const
    {
        Q_Q(const QAbstractSpinBox);
        if (cachedMinimumSizeHint.isEmpty()) {
            // Use the prefix and range to calculate the minimumSizeHint
            q->ensurePolished();

            const QFontMetrics fm(q->fontMetrics());
            int h = q_edit()->minimumSizeHint().height();
            int w = 0;

            QString s;
            QString fixedContent = prefix + u' ';
            s = textFromValue(minimum);
            s.truncate(18);
            s += fixedContent;
            w = qMax(w, fm.horizontalAdvance(s));
            s = textFromValue(maximum);
            s.truncate(18);
            s += fixedContent;
            w = qMax(w, fm.horizontalAdvance(s));

            if (q->specialValueText().size()) {
                s = q->specialValueText();
                w = qMax(w, fm.horizontalAdvance(s));
            }
            w += 2; // cursor blinking space

            QStyleOptionSpinBox opt;
            q_initStyleOption(&opt);
            QSize hint(w, h);

            cachedMinimumSizeHint = q->style()->sizeFromContents(QStyle::CT_SpinBox, &opt, hint, q);
        }
        return cachedMinimumSizeHint;
    }

    QSize sizeHint() const
    {
        Q_Q(const QAbstractSpinBox);
        if (cachedSizeHint.isEmpty()) {
            q->ensurePolished();

            const QFontMetrics fm(q->fontMetrics());
            int h = q_edit()->sizeHint().height();
            int w = 0;
            QString s;
            QString fixedContent = prefix + suffix + u' ';
            s = textFromValue(minimum);
            s.truncate(18);
            s += fixedContent;
            w = qMax(w, fm.horizontalAdvance(s));
            s = textFromValue(maximum);
            s.truncate(18);
            s += fixedContent;
            w = qMax(w, fm.horizontalAdvance(s));

            if (q->specialValueText().size()) {
                s = q->specialValueText();
                w = qMax(w, fm.horizontalAdvance(s));
            }
            w += 2; // cursor blinking space

            QStyleOptionSpinBox opt;
            q_initStyleOption(&opt);
            QSize hint(w, h);
            cachedSizeHint = q->style()->sizeFromContents(QStyle::CT_SpinBox, &opt, hint, q);
        }
        return cachedSizeHint;
    }

    void clear()
    {
        q_edit()->setText(prefix + suffix);
        q_edit()->setCursorPosition(prefix.size());
        cleared = true;
    }

    QString textFromValueImpl(T value) const
    {
        QString str;

        if (displayIntegerBase != 10) {
            const auto prefix = value < 0 ? "-"_L1 : ""_L1;
            str = prefix + QString::number(qAbs(value), displayIntegerBase);
        } else {
            Q_Q(const QAbstractSpinBox);
            QLocale loc = q->locale();
            if (q->isGroupSeparatorShown())
                loc.setNumberOptions(loc.numberOptions() & ~QLocale::OmitGroupSeparator);
            else
                loc.setNumberOptions(loc.numberOptions() | QLocale::OmitGroupSeparator);
            str = loc.toString(value);
        }

        return str;
    }

    T valueFromTextImpl(const QString &text) const
    {
        QString copy = text;
        int pos = q_edit()->cursorPosition();
        QValidator::State state = QValidator::Acceptable;
        return validateAndInterpret(copy, pos, state);
    }

    void initStyleOptionImpl(QStyleOptionSpinBox *option) const
    {
        if (!option) {
            return;
        }

        Q_Q(const QAbstractSpinBox);
        option->initFrom(q);
        option->activeSubControls = QStyle::SC_None;
        option->buttonSymbols = q->buttonSymbols();
        option->subControls = QStyle::SC_SpinBoxEditField;
        if (q->style()->styleHint(QStyle::SH_SpinBox_ButtonsInsideFrame, nullptr, q)) {
            option->subControls |= QStyle::SC_SpinBoxFrame;
        }
        if (q->buttonSymbols() != QAbstractSpinBox::NoButtons) {
            option->subControls |= QStyle::SC_SpinBoxUp | QStyle::SC_SpinBoxDown;
            if (buttonState & Up) {
                option->activeSubControls = QStyle::SC_SpinBoxUp;
            } else if (buttonState & Down) {
                option->activeSubControls = QStyle::SC_SpinBoxDown;
            }
        }

        if (buttonState) {
            option->state |= QStyle::State_Sunken;
        } else {
            option->activeSubControls = hoverControl;
        }

        option->stepEnabled = q->style()->styleHint(QStyle::SH_SpinControls_DisableOnBounds, nullptr, q)
            ? q_stepEnabled()
            : (QAbstractSpinBox::StepDownEnabled | QAbstractSpinBox::StepUpEnabled);

        option->frame = q->hasFrame();
    }

    QAbstractSpinBox::StepEnabled stepEnabledImpl() const
    {
        Q_Q(const QAbstractSpinBox);
        if (q->isReadOnly()) {
            return QAbstractSpinBox::StepNone;
        }
        if (q->wrapping()) {
            return QAbstractSpinBox::StepEnabled(QAbstractSpinBox::StepUpEnabled | QAbstractSpinBox::StepDownEnabled);
        }
        QAbstractSpinBox::StepEnabled ret = QAbstractSpinBox::StepNone;
        if (value < maximum) {
            ret |= QAbstractSpinBox::StepUpEnabled;
        }
        if (value > minimum) {
            ret |= QAbstractSpinBox::StepDownEnabled;
        }
        return ret;
    }

    std::optional<bool> event(QEvent *event)
    {
        switch (event->type()) {
        case QEvent::FontChange:
        case QEvent::StyleChange:
            cachedSizeHint = cachedMinimumSizeHint = QSize();
            break;
        case QEvent::ApplicationLayoutDirectionChange:
        case QEvent::LayoutDirectionChange:
            updateEditFieldGeometry();
            break;
        case QEvent::HoverEnter:
        case QEvent::HoverLeave:
        case QEvent::HoverMove:
            updateHoverControl(static_cast<const QHoverEvent *>(event)->position().toPoint());
            break;
        case QEvent::ShortcutOverride:
            if (q_edit()->event(event)) {
                return true;
            }
            break;
            // NOTE: This requires Qt Private API
            // #ifdef QT_KEYPAD_NAVIGATION
            //         case QEvent::EnterEditFocus:
            //         case QEvent::LeaveEditFocus:
            //             if (QApplicationPrivate::keypadNavigationEnabled()) {
            //                 const bool b = d->edit->event(event);
            //                 q_edit()->setSelection(d->edit->displayText().size() - d->suffix.size(), 0);
            //                 if (event->type() == QEvent::LeaveEditFocus)
            //                     emit editingFinished();
            //                 if (b)
            //                     return true;
            //             }
            //             break;
            // #endif
        case QEvent::InputMethod:
            return q_edit()->event(event);
        default:
            break;
        }
        return {};
    }

    void changeEvent(QEvent *event)
    {
        Q_Q(const QAbstractSpinBox);
        switch (event->type()) {
        case QEvent::StyleChange:
            spinClickTimerInterval = q->style()->styleHint(QStyle::SH_SpinBox_ClickAutoRepeatRate, nullptr, q);
            spinClickThresholdTimerInterval = q->style()->styleHint(QStyle::SH_SpinBox_ClickAutoRepeatThreshold, nullptr, q);
            if (q_edit()) {
                q_edit()->setFrame(!q->style()->styleHint(QStyle::SH_SpinBox_ButtonsInsideFrame, nullptr, q));
            }
            stepModifier = static_cast<Qt::KeyboardModifier>(q->style()->styleHint(QStyle::SH_SpinBox_StepModifier, nullptr, q));
            reset();
            updateEditFieldGeometry();
            break;
        case QEvent::LocaleChange:
            updateEdit();
            break;
        case QEvent::EnabledChange:
            if (!q->isEnabled()) {
                reset();
            }
            break;
        case QEvent::ActivationChange:
            if (!q->isActiveWindow()) {
                reset();
                if (pendingEmit) { // pendingEmit can be true even if it hasn't changed.
                    interpret(EmitIfChanged); // E.g. 10 to 10.0
                }
            }
            break;
        default:
            break;
        }
    }

    void closeEvent(QCloseEvent *event)
    {
        reset();
        if (pendingEmit) {
            interpret(EmitIfChanged);
        }
    }

#if QT_CONFIG(contextmenu)
    void contextMenuEvent(QContextMenuEvent *event)
    {
        // Require private API.
        // #ifdef Q_OS_WASM
        //         if (!qstdweb::haveAsyncify()) {
        //             qDebug() << " Skipping context menu for spinbox since asyncify is off";
        //             return;
        //         }
        // #endif

        QPointer<QMenu> menu = q_edit()->createStandardContextMenu();
        if (!menu) {
            return;
        }

        reset();

        const auto actions = menu->actions();
        auto selectAllActionIt = std::find_if(actions.begin(), actions.end(), [](QAction *action) {
            return action ? (action->objectName() == "select-all"_L1) : false;
        });

        if (selectAllActionIt != actions.end()) {
            QAction *selAll = new QAction(tr("&Select All"), menu);
#if QT_CONFIG(shortcut)
            selAll->setShortcut(QKeySequence::SelectAll);
#endif

            menu->insertAction(*selectAllActionIt, selAll);
            menu->removeAction(*selectAllActionIt);
            menu->addSeparator();
            const uint se = q_stepEnabled();
            QAction *up = menu->addAction(tr("&Step up"));
            up->setEnabled(se & QAbstractSpinBox::StepUpEnabled);
            QAction *down = menu->addAction(tr("Step &down"));
            down->setEnabled(se & QAbstractSpinBox::StepDownEnabled);
            menu->addSeparator();

            Q_Q(QAbstractSpinBox);

            const QPointer<QAbstractSpinBox> that = q;
            const QPoint pos = (event->reason() == QContextMenuEvent::Mouse)
                ? event->globalPos()
                : q->mapToGlobal(QPoint(event->pos().x(), 0)) + QPoint(q->width() / 2, q->height() / 2);
            const QAction *action = menu->exec(pos);
            delete static_cast<QMenu *>(menu);
            if (that && action) {
                if (action == up) {
                    q->stepBy(1);
                } else if (action == down) {
                    q->stepBy(-1);
                } else if (action == selAll) {
                    q->selectAll();
                }
            }
        }
        event->accept();
    }
#endif // QT_CONFIG(contextmenu)

    void focusOutEvent(QFocusEvent *event)
    {
        Q_Q(QAbstractSpinBox);

        if (pendingEmit) {
            interpret(EmitIfChanged);
        }

        reset();
        q_edit()->event(event);
        updateEdit();

        // NOTE: This requires Qt Private API
        // #ifdef QT_KEYPAD_NAVIGATION
        //         // editingFinished() is already emitted on LeaveEditFocus
        //         if (!QApplicationPrivate::keypadNavigationEnabled())
        // #endif
    }

    void hideEvent(QHideEvent *event)
    {
        reset();
        if (pendingEmit) {
            interpret(EmitIfChanged);
        }
    }

    void keyPressEvent(QKeyEvent *event)
    {
        Q_Q(QAbstractSpinBox);

        keyboardModifiers = event->modifiers();

        if (!event->text().isEmpty() && q_edit()->cursorPosition() < prefix.size()) {
            q_edit()->setCursorPosition(prefix.size());
        }

        int steps = 1;
        bool isPgUpOrDown = false;
        switch (event->key()) {
        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
            steps *= 10;
            isPgUpOrDown = true;
            Q_FALLTHROUGH();
        case Qt::Key_Up:
        case Qt::Key_Down: {
            // NOTE: This requires Qt Private API
            // #ifdef QT_KEYPAD_NAVIGATION
            //             if (QApplicationPrivate::keypadNavigationEnabled()) {
            //                 // Reserve up/down for nav - use left/right for edit.
            //                 if (!hasEditFocus() && (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)) {
            //                     event->ignore();
            //                     return;
            //                 }
            //             }
            // #endif
            event->accept();
            const bool up = (event->key() == Qt::Key_PageUp || event->key() == Qt::Key_Up);
            if (!(q_stepEnabled() & (up ? QAbstractSpinBox::StepUpEnabled : QAbstractSpinBox::StepDownEnabled))) {
                return;
            }
            if (!isPgUpOrDown && (event->modifiers() & stepModifier)) {
                steps *= 10;
            }
            if (!up) {
                steps *= -1;
            }
            if (q->style()->styleHint(QStyle::SH_SpinBox_AnimateButton, nullptr, q)) {
                buttonState = static_cast<Button>(Keyboard | (up ? Up : Down));
            }
            if (!spinClickTimer.isActive()) {
                stepBy(steps);
            }
            if (event->isAutoRepeat() && !isPgUpOrDown) {
                if (!spinClickThresholdTimer.isActive() && !spinClickTimer.isActive()) {
                    updateState(up, true);
                }
            }
#if QT_CONFIG(accessibility)
            QAccessibleValueChangeEvent event(q, value);
            QAccessible::updateAccessibility(&event);
#endif
            return;
        }
            // NOTE: This requires Qt Private API
            // #ifdef QT_KEYPAD_NAVIGATION
            //         case Qt::Key_Left:
            //         case Qt::Key_Right:
            //             if (QApplicationPrivate::keypadNavigationEnabled() && !hasEditFocus()) {
            //                 event->ignore();
            //                 return;
            //             }
            //             break;
            //         case Qt::Key_Back:
            //             if (QApplicationPrivate::keypadNavigationEnabled() && !hasEditFocus()) {
            //                 event->ignore();
            //                 return;
            //             }
            //             break;
            // #endif
        case Qt::Key_Enter:
        case Qt::Key_Return:
            q_edit()->setText(q_edit()->text()); // Clear QLineEdit undo history
            interpret(q->keyboardTracking() ? AlwaysEmit : EmitIfChanged);
            q->selectAll();
            event->ignore();

            Q_EMIT q->editingFinished();
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
            Q_EMIT q->returnPressed();
#endif
            Q_EMIT q_edit()->returnPressed();
            return;

            // NOTE: This requires Qt Private API
            // #ifdef QT_KEYPAD_NAVIGATION
            //         case Qt::Key_Select:
            //             if (QApplicationPrivate::keypadNavigationEnabled()) {
            //                 // Toggles between left/right moving cursor and inc/dec.
            //                 setEditFocus(!hasEditFocus());
            //             }
            //             return;
            // #endif

        case Qt::Key_U:
            if (event->modifiers() & Qt::ControlModifier && QGuiApplication::platformName() == "xcb"_L1) { // only X11
                event->accept();
                if (!q->isReadOnly()) {
                    q->clear();
                }
                return;
            }
            break;

        case Qt::Key_End:
        case Qt::Key_Home:
            if (event->modifiers() & Qt::ShiftModifier) {
                int currentPos = q_edit()->cursorPosition();
                const QString text = q_edit()->displayText();
                if (event->key() == Qt::Key_End) {
                    if ((currentPos == 0 && !prefix.isEmpty()) || text.size() - suffix.size() <= currentPos) {
                        break; // let lineedit handle this
                    } else {
                        q_edit()->setSelection(currentPos, text.size() - suffix.size() - currentPos);
                    }
                } else {
                    if ((currentPos == text.size() && !suffix.isEmpty()) || currentPos <= prefix.size()) {
                        break; // let lineedit handle this
                    } else {
                        q_edit()->setSelection(currentPos, prefix.size() - currentPos);
                    }
                }
                event->accept();
                return;
            }
            break;

        default:
#ifndef QT_NO_SHORTCUT
            if (event == QKeySequence::SelectAll) {
                q->selectAll();
                event->accept();
                return;
            }
#endif
            break;
        }

        q_edit()->event(event);
        if (!q_edit()->text().isEmpty()) {
            cleared = false;
        }
        if (!q->isVisible()) {
            ignoreUpdateEdit = true;
        }
    }

    void keyReleaseEvent(QKeyEvent *event)
    {
        keyboardModifiers = event->modifiers();
        if (buttonState & Keyboard && !event->isAutoRepeat()) {
            reset();
        } else {
            q_edit()->event(event);
        }
    }

    void mouseMoveEvent(QMouseEvent *event)
    {
        Q_Q(QAbstractSpinBox);

        keyboardModifiers = event->modifiers();
        updateHoverControl(event->position().toPoint());

        // If we have a timer ID, update the state
        if (spinClickTimer.isActive() && q->buttonSymbols() != QAbstractSpinBox::NoButtons) {
            const QAbstractSpinBox::StepEnabled se = q_stepEnabled();
            if ((se & QAbstractSpinBox::StepUpEnabled) && hoverControl == QStyle::SC_SpinBoxUp) {
                updateState(true);
            } else if ((se & QAbstractSpinBox::StepDownEnabled) && hoverControl == QStyle::SC_SpinBoxDown) {
                updateState(false);
            } else {
                reset();
            }
            event->accept();
        }
    }

    void mousePressEvent(QMouseEvent *event)
    {
        Q_Q(QAbstractSpinBox);

        keyboardModifiers = event->modifiers();
        if (event->button() != Qt::LeftButton || buttonState != None) {
            event->ignore();
            return;
        }

        updateHoverControl(event->position().toPoint());
        event->accept();

        const QAbstractSpinBox::StepEnabled se =
            (q->buttonSymbols() == QAbstractSpinBox::NoButtons) ? QAbstractSpinBox::StepEnabled(QAbstractSpinBox::StepNone) : q_stepEnabled();
        if ((se & QAbstractSpinBox::StepUpEnabled) && hoverControl == QStyle::SC_SpinBoxUp) {
            updateState(true);
        } else if ((se & QAbstractSpinBox::StepDownEnabled) && hoverControl == QStyle::SC_SpinBoxDown) {
            updateState(false);
        } else {
            event->ignore();
        }
    }

    void mouseReleaseEvent(QMouseEvent *event)
    {
        keyboardModifiers = event->modifiers();
        if ((buttonState & Mouse) != 0) {
            reset();
        }
        event->accept();
    }

    void resizeEvent(QResizeEvent *event)
    {
        Q_Q(QAbstractSpinBox);
        updateEditFieldGeometry();
        q->update();
    }

    void showEvent(QShowEvent *event)
    {
        reset();

        if (ignoreUpdateEdit) {
            ignoreUpdateEdit = false;
        } else {
            updateEdit();
        }
    }

    void timerEvent(QTimerEvent *event)
    {
        Q_Q(QAbstractSpinBox);

        bool doStep = false;
        if (event->id() == spinClickThresholdTimer.id()) {
            spinClickThresholdTimer.stop();
            effectiveSpinRepeatRate = buttonState & Keyboard ? QGuiApplication::styleHints()->keyboardAutoRepeatRateF() : spinClickTimerInterval;
            spinClickTimer.start(effectiveSpinRepeatRate, q);
            doStep = true;
        } else if (event->id() == spinClickTimer.id()) {
            if (q->isAccelerated()) {
                acceleration = acceleration + (int)(effectiveSpinRepeatRate * 0.05);
                auto interval = int(effectiveSpinRepeatRate - acceleration) * 1ms;
                if (interval >= 10ms) {
                    spinClickTimer.start(interval, q);
                }
            }
            doStep = true;
        }

        if (doStep) {
            const bool increaseStepRate = keyboardModifiers & stepModifier;
            const auto st = q_stepEnabled();
            if (buttonState & Up) {
                if (!(st & QAbstractSpinBox::StepUpEnabled)) {
                    reset();
                } else {
                    q->stepBy(increaseStepRate ? 10 : 1);
                }
            } else if (buttonState & Down) {
                if (!(st & QAbstractSpinBox::StepDownEnabled)) {
                    reset();
                } else {
                    q->stepBy(increaseStepRate ? -10 : -1);
                }
            }
            return;
        }
    }

#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event)
    {
#ifdef Q_OS_MACOS
        // If the event comes from a real mouse wheel, rather than a track pad
        // (Qt::MouseEventSynthesizedBySystem), the shift modifier changes the
        // scroll orientation to horizontal.
        // Convert horizontal events back to vertical whilst shift is held.
        if ((event->modifiers() & Qt::ShiftModifier) && event->source() == Qt::MouseEventNotSynthesized) {
            wheelDeltaRemainder += event->angleDelta().x();
        } else {
            wheelDeltaRemainder += event->angleDelta().y();
        }
#else
        wheelDeltaRemainder += event->angleDelta().y();
#endif
        const int steps = wheelDeltaRemainder / 120;
        wheelDeltaRemainder -= steps * 120;
        if (q_stepEnabled() & (steps > 0 ? QAbstractSpinBox::StepUpEnabled : QAbstractSpinBox::StepDownEnabled)) {
            stepBy(event->modifiers() & stepModifier ? steps * 10 : steps);
        }
        event->accept();
    }
#endif // QT_CONFIG(wheelevent)

protected:
    QAbstractSpinBox *const q_ptr;

    Q_DECLARE_TR_FUNCTIONS(QExtraSpinBoxPrivate)

    virtual QLineEdit *q_edit() const = 0;
    virtual void q_initStyleOption(QStyleOptionSpinBox *option) const = 0;
    virtual QAbstractSpinBox::StepEnabled q_stepEnabled() const = 0;
    virtual QValidator::State q_validate(QString &input, int &pos) const = 0;

    virtual QString textFromValue(T value) const = 0;
    virtual T valueFromText(const QString &text) const = 0;

    virtual void emitSignalsImpl() = 0;

private:
    QString stripped(const QString &t, int *pos = nullptr) const
    {
        Q_Q(const QAbstractSpinBox);

        QStringView text(t);

        if (q->specialValueText().size() == 0 || text != q->specialValueText()) {
            int from = 0;
            int size = text.size();
            bool changed = false;
            if (prefix.size() && text.startsWith(prefix)) {
                from += prefix.size();
                size -= from;
                changed = true;
            }
            if (suffix.size() && text.endsWith(suffix)) {
                size -= suffix.size();
                changed = true;
            }
            if (changed) {
                text = text.mid(from, size);
            }
        }

        const int s = text.size();
        text = text.trimmed();
        if (pos) {
            (*pos) -= (s - text.size());
        }

        return text.toString();
    }

    void updateEdit()
    {
        Q_Q(QAbstractSpinBox);
        const QString newText = specialValue() ? q->specialValueText() : prefix + textFromValue(value) + suffix;
        if (newText == q_edit()->displayText() || cleared) {
            return;
        }

        const bool empty = q_edit()->text().isEmpty();
        int cursor = q_edit()->cursorPosition();
        int selsize = q_edit()->selectedText().size();
        const QSignalBlocker blocker(q_edit());
        q_edit()->setText(newText);

        if (!specialValue()) {
            cursor = qBound(prefix.size(), cursor, q_edit()->displayText().size() - suffix.size());

            if (selsize > 0) {
                q_edit()->setSelection(cursor, selsize);
            } else {
                q_edit()->setCursorPosition(empty ? prefix.size() : cursor);
            }
        }
        q->update();
    }

    void clearCache()
    {
        cachedValue.reset();
        cachedText.reset();
        cachedState = QValidator::Acceptable;
    }

    void reset()
    {
        Q_Q(QAbstractSpinBox);

        buttonState = None;
        if (q) {
            spinClickTimer.stop();
            spinClickThresholdTimer.stop();
            acceleration = 0;
            q->update();
        }
    }

    T bound(T value, std::optional<T> old = {}, int steps = 0) const
    {
        Q_Q(const QAbstractSpinBox);
        if (!q->wrapping() || steps == 0 || !old.has_value()) {
            if (value < minimum) {
                value = q->wrapping() ? maximum : minimum;
            }
            if (value > maximum) {
                value = q->wrapping() ? minimum : maximum;
            }
        } else {
            T oldV = old.value();
            const bool wrapped = (value > oldV && steps < 0) || (value < oldV && steps > 0);

            if (value > maximum) {
                value = ((oldV == maximum && !wrapped && steps > 0) || (steps < 0 && oldV != minimum && wrapped)) ? minimum : maximum;
            } else if (wrapped && (value > maximum || value < minimum)) {
                value = ((oldV == maximum && steps > 0) || (oldV != minimum && steps < 0)) ? minimum : maximum;
            } else if (value < minimum) {
                value = (oldV != maximum && oldV != minimum ? minimum : maximum);
            }
        }

        return value;
    }

    void emitSignals(EmitPolicy ep, const QVariant &old)
    {
        if (ep != NeverEmit) {
            pendingEmit = false;
            if (ep == AlwaysEmit || value != old) {
                emitSignalsImpl();
            }
        }
    }

    void editorTextChanged(const QString &t)
    {
        Q_Q(QAbstractSpinBox);

        if (q->keyboardTracking()) {
            QString tmp = t;
            int pos = q_edit()->cursorPosition();
            QValidator::State state = q->validate(tmp, pos);
            if (state == QValidator::Acceptable) {
                const auto v = valueFromText(tmp);
                setValue(v, EmitIfChanged, tmp != t);
                pendingEmit = false;
            } else {
                pendingEmit = true;
            }
        } else {
            pendingEmit = true;
        }
    }

    void editorCursorPositionChanged(int oldpos, int newpos)
    {
        if (!q_edit()->hasSelectedText() && !ignoreCursorPositionChanged && !specialValue()) {
            ignoreCursorPositionChanged = true;

            bool allowSelection = true;
            int pos = -1;
            auto *edit = this->q_edit();

            if (newpos < prefix.size() && newpos != 0) {
                if (oldpos == 0) {
                    allowSelection = false;
                    pos = prefix.size();
                } else {
                    pos = oldpos;
                }
            } else if (newpos > edit->text().size() - suffix.size() && newpos != edit->text().size()) {
                if (oldpos == edit->text().size()) {
                    pos = edit->text().size() - suffix.size();
                    allowSelection = false;
                } else {
                    pos = edit->text().size();
                }
            }
            if (pos != -1) {
                const int selSize = edit->selectionStart() >= 0 && allowSelection ? (edit->selectedText().size() * (newpos < pos ? -1 : 1)) - newpos + pos : 0;

                const QSignalBlocker blocker(edit);
                if (selSize != 0) {
                    edit->setSelection(pos - selSize, selSize);
                } else {
                    edit->setCursorPosition(pos);
                }
            }
            ignoreCursorPositionChanged = false;
        }
    }

    T validateAndInterpret(QString &input, int &pos, QValidator::State &state) const
    {
        if (cachedText == input && !input.isEmpty()) {
            state = cachedState;
            QSBDEBUG() << "cachedText was '" << cachedText.value() << "' state was " << state << " and value was " << cachedValue.value();

            return cachedValue.value();
        }

        Q_Q(const QAbstractSpinBox);

        QString copy = stripped(input, &pos);
        QSBDEBUG() << "input" << input << "copy" << copy;
        state = QValidator::Acceptable;
        T num = minimum;

        if (maximum != minimum && (copy.isEmpty() || (minimum < 0 && copy == "-"_L1) || (maximum >= 0 && copy == "+"_L1))) {
            state = QValidator::Intermediate;
            QSBDEBUG() << __FILE__ << __LINE__ << "num is set to" << num;
        } else if (copy.startsWith(u'-') && minimum >= 0) {
            state = QValidator::Invalid; // special-case -0 will be interpreted as 0 and thus not be invalid with a range from 0-100
        } else {
            bool ok = false;
            if (displayIntegerBase != 10) {
                num = copy.toInt(&ok, displayIntegerBase);
            } else {
                const auto locale = q->locale();
                if constexpr (std::is_same_v<T, int>) {
                    num = locale.toInt(copy, &ok);
                } else if constexpr (std::is_same_v<T, long>) {
                    num = locale.toLong(copy, &ok);
                } else if constexpr (std::is_same_v<T, long long>) {
                    num = locale.toLongLong(copy, &ok);
                } else if constexpr (std::is_same_v<T, short>) {
                    num = locale.toShort(copy, &ok);
                } else if constexpr (std::is_same_v<T, unsigned int>) {
                    num = locale.toUInt(copy, &ok);
                } else if constexpr (std::is_same_v<T, unsigned long>) {
                    num = locale.toULong(copy, &ok);
                } else if constexpr (std::is_same_v<T, unsigned long long>) {
                    num = locale.toULongLong(copy, &ok);
                } else if constexpr (std::is_same_v<T, unsigned short>) {
                    num = locale.toUShort(copy, &ok);
                } else {
                    static_assert(dependent_false<T>::value, "QExtraSpinBoxPrivate: Unsupported type!");
                }
                if (!ok && (maximum >= 1000 || minimum <= -1000)) {
                    const QString sep(locale.groupSeparator());
                    const QString doubleSep = sep + sep;
                    if (copy.contains(sep) && !copy.contains(doubleSep)) {
                        QString copy2 = copy;
                        copy2.remove(sep);
                        num = locale.toInt(copy2, &ok);
                    }
                }
            }
            QSBDEBUG() << __FILE__ << __LINE__ << "num is set to" << num;
            if (!ok) {
                state = QValidator::Invalid;
            } else if (num >= minimum && num <= maximum) {
                state = QValidator::Acceptable;
            } else if (maximum == minimum) {
                state = QValidator::Invalid;
            } else {
                if ((num >= 0 && num > maximum) || (num < 0 && num < minimum)) {
                    state = QValidator::Invalid;
                    QSBDEBUG() << __FILE__ << __LINE__ << "state is set to Invalid";
                } else {
                    state = QValidator::Intermediate;
                    QSBDEBUG() << __FILE__ << __LINE__ << "state is set to Intermediate";
                }
            }
        }
        if (state != QValidator::Acceptable) {
            num = maximum > 0 ? minimum : maximum;
        }
        input = prefix + copy + suffix;
        cachedText = input;
        cachedState = state;
        cachedValue = num;

        QSBDEBUG() << "cachedText is set to '" << cachedText.value() << "' state is set to " << state << " and value is set to " << cachedValue.value();
        return cachedValue.value();
    }

    void interpret(EmitPolicy ep)
    {
        Q_Q(QAbstractSpinBox);

        T v{};
        bool doInterpret = true;
        QString tmp = q_edit()->displayText();
        int pos = q_edit()->cursorPosition();
        const int oldpos = pos;

        if (q->validate(tmp, pos) != QValidator::Acceptable) {
            const QString copy = tmp;
            q->fixup(tmp);
            QSBDEBUG() << "QExtraSpinBoxPrivate::interpret() text '" << q_edit()->displayText() << "' >> '" << copy << '\'' << "' >> '" << tmp << '\'';

            doInterpret = tmp != copy && (q->validate(tmp, pos) == QValidator::Acceptable);
            if (!doInterpret) {
                v = (q->correctionMode() == QAbstractSpinBox::CorrectToNearestValue ? qBound(minimum, v, maximum) : value);
            }
        }
        if (doInterpret) {
            v = valueFromText(tmp);
        }
        clearCache();
        setValue(v, ep, true);
        if (oldpos != pos) {
            q_edit()->setCursorPosition(pos);
        }
    }

    void updateEditFieldGeometry()
    {
        Q_Q(QAbstractSpinBox);
        QStyleOptionSpinBox opt;
        q_initStyleOption(&opt);
        opt.subControls = QStyle::SC_SpinBoxEditField;
        q_edit()->setGeometry(q->style()->subControlRect(QStyle::CC_SpinBox, &opt, QStyle::SC_SpinBoxEditField, q));
    }

    T calculateAdaptiveDecimalStep(int steps) const
    {
        const T absValue = qAbs(value);

        if (absValue < 100)
            return 1;

        const bool valueNegative = value < 0;
        const bool stepsNegative = steps < 0;
        const int signCompensation = (valueNegative == stepsNegative) ? 0 : 1;

        const int log = static_cast<T>(std::log10(absValue - signCompensation)) - 1;
        return static_cast<T>(std::pow(10, log));
    }

    bool updateHoverControl(const QPoint &pos)
    {
        Q_Q(QAbstractSpinBox);
        QRect lastHoverRect = hoverRect;
        QStyle::SubControl lastHoverControl = hoverControl;
        bool doesHover = q->testAttribute(Qt::WA_Hover);
        if (lastHoverControl != newHoverControl(pos) && doesHover) {
            q->update(lastHoverRect);
            q->update(hoverRect);
            return true;
        }
        return !doesHover;
    }

    QStyle::SubControl newHoverControl(const QPoint &pos)
    {
        Q_Q(QAbstractSpinBox);
        QStyleOptionSpinBox opt;
        q_initStyleOption(&opt);
        opt.subControls = QStyle::SC_All;
        hoverControl = q->style()->hitTestComplexControl(QStyle::CC_SpinBox, &opt, pos, q);
        hoverRect = q->style()->subControlRect(QStyle::CC_SpinBox, &opt, hoverControl, q);
        return hoverControl;
    }

    void updateState(bool up, bool fromKeyboard = false)
    {
        Q_Q(QAbstractSpinBox);
        if ((up && (buttonState & Up)) || (!up && (buttonState & Down)))
            return;
        reset();
        if (q && (q_stepEnabled() & (up ? QAbstractSpinBox::StepUpEnabled : QAbstractSpinBox::StepDownEnabled))) {
            buttonState = static_cast<Button>((up ? Up : Down) | (fromKeyboard ? Keyboard : Mouse));
            int steps = up ? 1 : -1;
            if (keyboardModifiers & stepModifier)
                steps *= 10;
            q->stepBy(steps);
            spinClickThresholdTimer.start(spinClickThresholdTimerInterval * 1ms, q);
#if QT_CONFIG(accessibility)
            QAccessibleValueChangeEvent event(q, value);
            QAccessible::updateAccessibility(&event);
#endif
        }
    }
};

template<typename T>
QExtraSpinBoxValidator<T>::QExtraSpinBoxValidator(QAbstractSpinBox *qptr, QExtraSpinBoxPrivate<T> *dptr)
    : QValidator{qptr}
    , qptr{qptr}
    , dptr{dptr}
{
    setObjectName("qtextraspinbox_spinboxvalidator"_L1);
}

template<typename T>
QValidator::State QExtraSpinBoxValidator<T>::validate(QString &input, int &pos) const
{
    if (qptr->specialValueText().size() > 0 && input == qptr->specialValueText()) {
        return QValidator::Acceptable;
    }

    if (!dptr->prefix.isEmpty() && !input.startsWith(dptr->prefix)) {
        input.prepend(dptr->prefix);
        pos += dptr->prefix.size();
    }

    if (!dptr->suffix.isEmpty() && !input.endsWith(dptr->suffix)) {
        input.append(dptr->suffix);
    }

    return qptr->validate(input, pos);
}

template<typename T>
void QExtraSpinBoxValidator<T>::fixup(QString &input) const
{
    qptr->fixup(input);
}
} // namespace QtExtraSpinBoxes

#endif // QTEXTRASPINBOXES_P_H
