/*
 * meegotouchcp-socialweb - libsocialweb account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef SWACCTEDITPAGE_H
#define SWACCTEDITPAGE_H

#include <MApplicationPage>
#include <MTextEdit>
#include <MLabel>
#include <MButton>

#include <libsocialweb-qt/swclientdbustypes.h>
#include <libsocialweb-qt/swclientservice.h>
#include <libsocialweb-qt/swclientserviceconfig.h>

class SwAcctEditPage : public MApplicationPage
{
    Q_OBJECT
public:
    SwAcctEditPage(SwClientService *swService, QGraphicsItem *parent = 0);

private slots:
    void onApplyClicked();
    void onCredsStateChanged(SwClientService *, SwClientService::CredsState);
    void onLoginChanged();

    void onFlickrClicked();

    void onLinkClicked(QString url);

private:
    MTextEdit *mEdtUsername;
    MTextEdit *mEdtPassword;
    MButton *mFlickrButton;

    MButton *mBtnApply;
    MButton *mBtnCancel;

    MLabel *mLblStatus;

    SwClientService *mService;
    SwClientServiceConfig *mServiceConfig;

    QHash<QString, QString> mParams;

    //TODO - a better way...
    bool mFlickrClicked;

};

#endif // SWACCTEDITPAGE_H
