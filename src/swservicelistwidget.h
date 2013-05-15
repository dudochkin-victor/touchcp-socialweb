/*
 * meegotouchcp-socialweb - libsocialweb account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef SWSERVICELISTWIDGET_H
#define SWSERVICELISTWIDGET_H

#include <DcpWidget>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MButton>

#include "swaccteditpage.h"

class SwClientService;

class SwServiceListWidget : public DcpWidget
{
Q_OBJECT
public:
    SwServiceListWidget(QGraphicsWidget *parent = 0);
    bool pagePans() const { return true; }
    bool back();

private slots:
    void onServiceButtonClicked();

private:
    SwAcctEditPage *mAccountEditPage;
    MLinearLayoutPolicy *mPolicy;
    MLayout *mLayout;
    QList<MButton *> mButtons;
    QStringList mServiceNames;
    QList<SwClientService *> mServices;

};

#endif // SWSERVICELISTWIDGET_H
