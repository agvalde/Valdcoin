// Copyright (c) 2011-2021 The Valdcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef VALDCOIN_QT_VALDCOINAMOUNTFIELD_H
#define VALDCOIN_QT_VALDCOINAMOUNTFIELD_H

#include <consensus/amount.h>
#include <qt/valdcoinunits.h>

#include <QWidget>

class AmountSpinBox;

QT_BEGIN_NAMESPACE
class QValueComboBox;
QT_END_NAMESPACE

/** Widget for entering valdcoin amounts.
  */
class ValdcoinAmountField: public QWidget
{
    Q_OBJECT

    // ugly hack: for some unknown reason CAmount (instead of qint64) does not work here as expected
    // discussion: https://github.com/valdcoin/valdcoin/pull/5117
    Q_PROPERTY(qint64 value READ value WRITE setValue NOTIFY valueChanged USER true)

public:
    explicit ValdcoinAmountField(QWidget *parent = nullptr);

    CAmount value(bool *value=nullptr) const;
    void setValue(const CAmount& value);

    /** If allow empty is set to false the field will be set to the minimum allowed value if left empty. **/
    void SetAllowEmpty(bool allow);

    /** Set the minimum value in satoshis **/
    void SetMinValue(const CAmount& value);

    /** Set the maximum value in satoshis **/
    void SetMaxValue(const CAmount& value);

    /** Set single step in satoshis **/
    void setSingleStep(const CAmount& step);

    /** Make read-only **/
    void setReadOnly(bool fReadOnly);

    /** Mark current value as invalid in UI. */
    void setValid(bool valid);
    /** Perform input validation, mark field as invalid if entered value is not valid. */
    bool validate();

    /** Change unit used to display amount. */
    void setDisplayUnit(ValdcoinUnit new_unit);

    /** Make field empty and ready for new input. */
    void clear();

    /** Enable/Disable. */
    void setEnabled(bool fEnabled);

    /** Qt messes up the tab chain by default in some cases (issue https://bugreports.qt-project.org/browse/QTBUG-10907),
        in these cases we have to set it up manually.
    */
    QWidget *setupTabChain(QWidget *prev);

Q_SIGNALS:
    void valueChanged();

protected:
    /** Intercept focus-in event and ',' key presses */
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    AmountSpinBox* amount{nullptr};
    QValueComboBox *unit;

private Q_SLOTS:
    void unitChanged(int idx);

};

#endif // VALDCOIN_QT_VALDCOINAMOUNTFIELD_H
