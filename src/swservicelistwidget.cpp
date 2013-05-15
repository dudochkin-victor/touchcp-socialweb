/*
 * meegotouchcp-socialweb - libsocialweb account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "swservicelistwidget.h"

#include <MLabel>
#include <MWidgetCreator>
#include <MApplicationPage>

#include <libsocialweb-qt/swclient.h>

SwServiceListWidget::SwServiceListWidget(QGraphicsWidget *parent) :
        DcpWidget(parent),
        mAccountEditPage(0)
{
    SwClient *client = new SwClient();
    mServiceNames = *client->getServices();

    mLayout = new MLayout;
    mPolicy = new MLinearLayoutPolicy(mLayout, Qt::Vertical);

    if (mServiceNames.count() == 0) {
        //% "libsocialweb error - no web services to configure!"
        MLabel *lblNoServices = new MLabel(qtTrId("label_no_services_available"));
        lblNoServices->setObjectName("SwServiceListWidgetNoServicesLabel");
        mPolicy->addItem(lblNoServices);
    } else {
        foreach (QString service, mServiceNames) {
            SwClientService *swService = client->getService(service);
            if (swService) {
                mServices.append(swService);
                SwClientServiceConfig *swConfig = swService->getServiceConfig();
                MButton *btn = new MButton(swConfig->getDisplayName());
                mButtons.append(btn);
                mPolicy->addItem(btn, Qt::AlignHCenter);
                connect(btn,
                        SIGNAL(clicked()),
                        this,
                        SLOT(onServiceButtonClicked()));
            }
        }
    }

    mLayout->setPolicy(mPolicy);
    setLayout(mLayout);
}

bool SwServiceListWidget::back()
{
    return DcpWidget::back();
}

//Private slots

void SwServiceListWidget::onServiceButtonClicked()
{
    MButton *btn = qobject_cast<MButton *>(QObject::sender());
    int idx = mButtons.indexOf(btn);
    if (idx != -1) {
        mAccountEditPage = new SwAcctEditPage(mServices.at(idx));
        mAccountEditPage->appear(MSceneWindow::DestroyWhenDismissed);
    }
}
