/*
 * meegotouchcp-socialweb - libsocialweb account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "socialwebbrief.h"
#include <DcpWidgetTypes>

SocialWebBrief::SocialWebBrief() : DcpBrief()
{
}

SocialWebBrief::~SocialWebBrief()
{
}

QString SocialWebBrief::titleText() const
{
  //% "Social Networking Accounts"
  return qtTrId("brief_title_social_networking_accounts");
}

int SocialWebBrief::widgetTypeID() const
{
  return DcpWidgetType::Label;
}
