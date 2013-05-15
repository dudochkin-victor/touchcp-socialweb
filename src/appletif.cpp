/*
 * meegotouchcp-socialweb - libsocialweb account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */


#include "appletif.h"
#include "socialwebbrief.h"
#include "swservicelistwidget.h"

#include <MAction>
#include <MLibrary>
#include <MTheme>
#include <QDebug>


void SocialWebApplet::init()
{
  MTheme::loadCSS(QString(CSSDIR) + "libsocialwebapplet.css");
}

DcpWidget* SocialWebApplet::constructWidget(int widgetId)
{
  DcpWidget *widget = NULL;

  switch (widgetId) {
  case 0:
    widget = new SwServiceListWidget();
    break;
    
  default:
    qDebug() << QString("Unknown widgetid %1 in SocialWebApplet::constructWidget").arg(widgetId);
    break;
  }
  return widget;
}

QVector<MAction *> SocialWebApplet::viewMenuItems()
{
  //we don't intend any having menu items
  return QVector<MAction*>(); 
}

QString SocialWebApplet::title() const
{
  //% "Social Networking Accounts"
  return qtTrId("applet_title_social_networking_accounts");
}

DcpBrief* SocialWebApplet::constructBrief(int)
{
  return new SocialWebBrief();
}

Q_EXPORT_PLUGIN2(SocialWebApplet, SocialWebApplet)
M_LIBRARY

