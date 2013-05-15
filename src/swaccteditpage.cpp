/*
 * meegotouchcp-socialweb - libsocialweb account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "swaccteditpage.h"

#include <QtDebug>
#include <QtCore>
#include <QDesktopServices>

#include <MLayout>
#include <MGridLayoutPolicy>
#include <MLabel>
#include <MWidgetCreator>

//% "<b></b>You'll need an account with %1 and an Internet connection to use this web service."
#define SERVICE_HINT_TEXT qtTrId("service_hint_text")

//% "<b></b>Please enter your account information"
#define STATUS_TEXT_UNCONFIGURED qtTrId("status_text_unconfigured")
//% "<b></b>Verifying login, just a moment"
#define STATUS_TEXT_VERIFY qtTrId("status_text_verifying")
//% "<b></b>Login failed. Please check your information and try again"
#define STATUS_TEXT_INVALID qtTrId("status_text_invalid_login")
//% "<b></b>Login successful!"
#define STATUS_TEXT_VALID qtTrId("status_text_valid_login")
//% "<b></b>Error while saving login"
#define STATUS_TEXT_ERROR qtTrId("status_text_error_login")

#define USER_KEY "user"
#define PASS_KEY "password"

SwAcctEditPage::SwAcctEditPage(SwClientService *swService, QGraphicsItem *parent) :
        MApplicationPage(parent),
        mService(swService),
        mServiceConfig(mService->getServiceConfig()),
        mFlickrClicked(false)
{
    int adj = 0;
    setEscapeMode(MApplicationPageModel::EscapeManualBack);
    connect(this, SIGNAL(backButtonClicked()),
            this, SLOT(dismiss()));

    connect(mService,
            SIGNAL(CredsStateChanged(SwClientService*,SwClientService::CredsState)),
            this,
            SLOT(onCredsStateChanged(SwClientService *, SwClientService::CredsState)));
    if (!mServiceConfig->isValid())
        dismiss();
    mParams = mServiceConfig->getConfigParams();

    MLayout *layout = new MLayout;
    MGridLayoutPolicy *policy = new MGridLayoutPolicy(layout);

    QString link = mServiceConfig->getLink();
    MLabel *lblServiceName = new MLabel();
    if (!link.isEmpty())
        lblServiceName->setText(QString("<a href=\"%1\">%2</a>").arg(link, mServiceConfig->getDisplayName()));
    else
        lblServiceName->setText(mServiceConfig->getDisplayName());
    lblServiceName->setObjectName("SwAcctEditPageServiceNameLabel");
    policy->addItem(lblServiceName, 0, 0, 1, 2, Qt::AlignHCenter);

    connect(lblServiceName,
            SIGNAL(linkActivated(QString)),
            this,
            SLOT(onLinkClicked(QString)));

    QString desc = mServiceConfig->getDescription();
    if (!desc.isEmpty()) {
        MLabel *lblServiceDesc = new MLabel();
        lblServiceDesc->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        lblServiceDesc->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //Have to insert a random HTML tag to make word wrap actually work...
        //Should be able to remove at some point once MTF word wrap works properly
        lblServiceDesc->setText(QString("<b></b>").append(desc));
        lblServiceDesc->setObjectName("SwAcctEditPageServiceDescLabel");
        policy->addItem(lblServiceDesc, 1, 0, 1, 2, Qt::AlignLeft);
    } else {
        adj -= 1;
    }

    //This is *ugly*
    if (mServiceConfig->getAuthtype() == SwClientServiceConfig::AuthTypeFlickr) {
        if (!mParams.value(USER_KEY).isEmpty()) {
            //% "User %1"
            MLabel *lblUsername = new MLabel(qtTrId("label_flickr_username").arg(mParams.value(USER_KEY)));
            lblUsername->setObjectName("SwAcctEditPageFlickrUserLabel");
            policy->addItem(lblUsername, 2+adj, 0, 1, 2, Qt::AlignHCenter);
            adj += 1;
        }

        mFlickrButton = new MButton();
        mFlickrButton->setObjectName("SwAcctEditPageFlickrButton");
        policy->addItem(mFlickrButton, 2+adj, 0, 1, 2, Qt::AlignHCenter);

        connect(mFlickrButton,
                SIGNAL(clicked()),
                this,
                SLOT(onFlickrClicked()));

        adj += 1;
    } else {
        MLabel *lblUsername = new MLabel();
        lblUsername->setObjectName("SwAcctEditPageUsernameLabel");
        lblUsername->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //% "Username:"
        lblUsername->setText(qtTrId("label_username"));
        policy->addItem(lblUsername, 2+adj, 0);

        mEdtUsername = new MTextEdit(MTextEditModel::SingleLine);
        mEdtUsername->setObjectName("SwAcctEditPageUsernameButton");
        mEdtUsername->setText(mParams.value(USER_KEY));
        //qDebug() << QString("Set username to %1").arg(mParams.value(USER_KEY));
        policy->addItem(mEdtUsername, 2+adj, 1);

        if (mServiceConfig->getAuthtype() == SwClientServiceConfig::AuthTypePassword) {
            MLabel *lblPassword = new MLabel();
            lblPassword->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            //% "Password:"
            lblPassword->setText(qtTrId("label_password"));
            lblPassword->setObjectName("SwAcctEditPagePasswordLabel");
            policy->addItem(lblPassword, 3+adj, 0);
            mEdtPassword = new MTextEdit(MTextEditModel::SingleLine);
            mEdtPassword->setText(mParams.value(PASS_KEY));
            mEdtPassword->setEchoMode(MTextEditModel::Password);
            mEdtPassword->setObjectName("SwAcctEditPagePasswordButton");
    //        qDebug() << QString("Set password to %1").arg(mParams.value(PASS_KEY));
            policy->addItem(mEdtPassword, 3+adj, 1);
            connect(mEdtPassword,
                    SIGNAL(textChanged()),
                    this,
                    SLOT(onLoginChanged()));
            adj += 1;
        }
    }

    mLblStatus = new MLabel(STATUS_TEXT_UNCONFIGURED);
    mLblStatus->setObjectName("SwAcctEditPageStatusLabel");
    policy->addItem(mLblStatus, 3+adj, 0, 1, 2, Qt::AlignHCenter);

    onCredsStateChanged(mService, mService->credsState());

    MLabel *lblServiceHint = new MLabel();
    lblServiceHint->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    lblServiceHint->setObjectName("SwAcctEditPageServiceHintLabel");
    lblServiceHint->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lblServiceHint->setText(SERVICE_HINT_TEXT.arg(mService->getDisplayName()));
    policy->addItem(lblServiceHint, 4+adj, 0, 1, 2);


    //This, also, is *ugly*
    if (mServiceConfig->getAuthtype() != SwClientServiceConfig::AuthTypeFlickr) {
        //% "Apply"
        mBtnApply = new MButton(qtTrId("button_apply"));
        mBtnApply->setObjectName("SwAcctEditPageApplyButton");
        policy->addItem(mBtnApply, 5+adj, 0, 1, 2, Qt::AlignHCenter);

        //% "Cancel"
        mBtnCancel = new MButton(qtTrId("button_cancel"));
        mBtnCancel->setObjectName("SwAcctEditPageCancelButton");
        policy->addItem(mBtnCancel, 6+adj, 0, 1, 2, Qt::AlignHCenter);

        connect(mEdtUsername,
                SIGNAL(textChanged()),
                this,
                SLOT(onLoginChanged()));

        connect(mBtnApply,
                SIGNAL(clicked()),
                this,
                SLOT(onApplyClicked()));
        connect(mBtnCancel,
                SIGNAL(clicked()),
                this,
                SLOT(dismiss()));
    }


    layout->setPolicy(policy);
    centralWidget()->setLayout(layout);
}

//Private slots

void SwAcctEditPage::onApplyClicked()
{
    mBtnApply->setEnabled(false);

    if (mServiceConfig->getAuthtype() == SwClientServiceConfig::AuthTypePassword) {
        mParams.insert(PASS_KEY, mEdtPassword->text());
    }

    mParams.insert(USER_KEY, mEdtUsername->text());
    if (!mServiceConfig->setConfigParams(mParams)) {
        mLblStatus->setText(STATUS_TEXT_ERROR);
    } else {
        if (mService->hasVerifyCreds()) {
            mLblStatus->setText(STATUS_TEXT_VERIFY);
        } else {
            mLblStatus->setText(STATUS_TEXT_UNCONFIGURED);
        }
    }
}

void SwAcctEditPage::onCredsStateChanged(SwClientService *swService, SwClientService::CredsState state)
{
    if (swService->getServiceConfig()->getAuthtype()
            == SwClientServiceConfig::AuthTypeFlickr) {

        if (state == SwClientService::CredsValid) {
            //% "Log out"
            mFlickrButton->setText(qtTrId("button_logout"));
            mFlickrButton->setEnabled(true);
        } else if ((state == SwClientService::CredsInvalid) || (!swService->isConfigured())) {
            //% "Log in"
            mFlickrButton->setText(qtTrId("button_login"));
            mFlickrClicked = false;
            mFlickrButton->setEnabled(true);
        } else if (state == SwClientService::CredsUnknown) {
            //% "Working..."
            mFlickrButton->setText(qtTrId("button_working"));
            mFlickrButton->setEnabled(false);
        } else {
            qDebug("Got an unknown SwClientService state in onCredsStateChanged!");
        }

    }

    if (swService->isConfigured()) {
        if (swService->hasVerifyCreds()) {
            switch (state) {
            case SwClientService::CredsValid:
                mLblStatus->setText(STATUS_TEXT_VALID);
                break;
            case SwClientService::CredsInvalid:
                mLblStatus->setText(STATUS_TEXT_INVALID);
                break;
            case SwClientService::CredsUnknown:
                mLblStatus->setText(STATUS_TEXT_VERIFY);
                break;
            }
        } else {
            mLblStatus->setText(STATUS_TEXT_UNCONFIGURED);
        }
    } else {
        mLblStatus->setText(STATUS_TEXT_UNCONFIGURED);
    }
}

void SwAcctEditPage::onLoginChanged()
{
    mBtnApply->setEnabled(true);
}

void SwAcctEditPage::onFlickrClicked()
{
    //We're logging out...
    if (mService->isConfigured() && (mService->credsState() == SwClientService::CredsValid)) {
        mServiceConfig->flickrDeleteLogin();
        mFlickrClicked = false;
    } else if (!mFlickrClicked) {
        //We're pressing Login...

        if (mServiceConfig->flickrOpenLogin())
            //% "Continue"
            mFlickrButton->setText(qtTrId("button_continue"));
        else
            qWarning("Couldn't open Flickr login!");
        mFlickrClicked = true;
    } else {
        //We're pressing Continue
        mServiceConfig->flickrContinueLogin();
        mFlickrClicked = false;
        //If we get success, it will switch to log out at soon as
        //libsocialweb updates the DynCaps.
        //If we get failure, we want it to say "Log in" to try again...
        //% "Log in"
        mFlickrButton->setText(qtTrId("button_login"));
    }
}

void SwAcctEditPage::onLinkClicked(QString url)
{
    QDesktopServices::openUrl(url);
}
