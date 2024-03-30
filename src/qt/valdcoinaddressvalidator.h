// Copyright (c) 2011-2020 The Valdcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef VALDCOIN_QT_VALDCOINADDRESSVALIDATOR_H
#define VALDCOIN_QT_VALDCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class ValdcoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ValdcoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Valdcoin address widget validator, checks for a valid valdcoin address.
 */
class ValdcoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ValdcoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // VALDCOIN_QT_VALDCOINADDRESSVALIDATOR_H
