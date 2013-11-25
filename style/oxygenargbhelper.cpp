//////////////////////////////////////////////////////////////////////////////
// oxygenargbhelper.h
// handle oxygen ARGB (translucent windows) support
// -------------------
//
// Copyright (c) 2010 Hugo Pereira Da Costa <hugo@oxygen-icons.org>
//
// Loosely inspired (and largely rewritten) from BeSpin style
// Copyright (C) 2007 Thomas Luebking <thomas.luebking@web.de>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//////////////////////////////////////////////////////////////////////////////

#include "oxygenargbhelper.h"
#include "oxygenargbhelper.moc"

#include <QtGui/QDialog>
#include <QtGui/QIcon>

#ifdef Q_WS_X11
#include <QtGui/QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif

namespace Oxygen
{

    //______________________________________________________________
    ArgbHelper::ArgbHelper( QObject* parent, StyleHelper& helper ):
        QObject( parent ),
        _helper( helper ),
        _applicationType( AppUnknown ),
        _enabled( false ),
        _opacity( 0xff )
    {

        #ifdef Q_WS_X11

        // create atom
        _xEmbedAtom = XInternAtom( QX11Info::display(), "_XEMBED_INFO", False);

        #endif

    }

    //______________________________________________________________
    void ArgbHelper::registerApplication( QApplication* app )
    {

        // get application full path
        if( app->argc() < 1 ) return;

        QString appName( app->argv()[0] );
        int position( appName.lastIndexOf( '/' ) );
        if( position >= 0 ) appName.remove( 0, position+1 );

        if( appName == "plasma" || appName.startsWith( "plasma-" ) )
        {

            /*
            HACK: need to detect if application is of type Plasma
            because applying translucency to some of its widgets creates
            painting issues which could not be identified with a 'generic'
            criteria.
            */
            _applicationType = AppPlasma;

        } else if( _blackList.contains( appName ) ) {

            // black-list application
            _applicationType = AppBlackListed;

        }

    }

    //______________________________________________________________
    bool ArgbHelper::registerWidget( QWidget *widget )
    {

        // check blacklisted applications
        if( _applicationType == AppBlackListed ) return false;

        // adjust flags for windows and dialogs
        switch( widget->windowFlags() & Qt::WindowType_Mask )
        {

            case Qt::Window:
            case Qt::Dialog:
            case Qt::Sheet:
            {

                // do not handle all kind of 'special background' widgets
                if( widget->windowType() == Qt::Desktop ||
                    widget->testAttribute(Qt::WA_X11NetWmWindowTypeDesktop) ||
                    widget->testAttribute(Qt::WA_NoSystemBackground) ||
                    widget->testAttribute(Qt::WA_PaintOnScreen)
                    ) break;

                // disable kde screensaver windows
                /*
                the kscreensaver widgets get a WA_PaintOnScreen flag set,
                which should have been covered by the above, but somehow the flag is set too late,
                and notably after polish is called. Or so it seems.
                */
                if( widget->inherits( "KScreenSaver" ) ) break;

                // Hack: stop here if application is of type Plasma
                /*
                Right now we need to reject window candidates if the application is of type plasma
                because it conflicts with some widgets embedded into the SysTray. Ideally one would
                rather find a "generic" reason, not to handle them
                */
                if( _applicationType == AppPlasma && !qobject_cast<QDialog*>(widget) ) break;

                // stop here if no translucent background selected/supported
                if( !( _helper.compositingActive() && enabled() ) ) break;

                // more tests
                if( !widget->isWindow() ) break;
                if(
                    widget->inherits( "QTipLabel") ||
                    widget->inherits( "QSplashScreen") ) break;

                if( widget->windowFlags().testFlag( Qt::FramelessWindowHint ) ) break;
                if( isXEmbed( widget ) ) break;

                // setup transparency and return
                setupTransparency( widget );
                return true;
            }

            default: break;

        }

        return false;

    }

    //______________________________________________________________
    bool ArgbHelper::unregisterWidget( QWidget* widget )
    {

        if( _transparentWidgets.contains( widget ) )
        {
            widget->setAttribute(Qt::WA_NoSystemBackground, false);
            widget->setAttribute(Qt::WA_TranslucentBackground, false);
            return true;
        }

        return false;

    }

    //______________________________________________________________
    bool ArgbHelper::eventFilter( QObject* object, QEvent* event )
    {

        switch( event->type() )
        {

            case QEvent::WinIdChange:
            {
                // cast to widget (safe)
                QWidget* widget( static_cast<QWidget*>( object ) );
                if( widget->testAttribute(Qt::WA_WState_Created) && widget->internalWinId() )
                {
                    // set Argb X property if possible, and remove from event filter
                    _helper.setHasArgb( widget->winId(), true );
                    widget->removeEventFilter( this );
                }
                break;
            }

            default: break;

        }

        // fallback
        return QObject::eventFilter( object, event );

    }

    //______________________________________________________________
    void ArgbHelper::setupTransparency( QWidget* widget )
    {

        // check whether already registered
        if( _transparentWidgets.contains( widget ) ) return;

        // translucent background already set. Do nothing.
        if( widget->testAttribute(Qt::WA_TranslucentBackground) )
        {
            // set Argb xproperty
            _helper.setHasArgb( widget->winId(), true );
            return;
        }

        /*
        whenever you set the translucency flag, Qt will create a new widget under the hood, replacing the old
        Unfortunately some properties are lost, among them the window icon. We save it and restore it manually
        */

        // store icon
        QIcon icon(widget->windowIcon());

        const bool wasVisible( widget->isVisible() );
        const bool moved( widget->testAttribute( Qt::WA_Moved ) );

        // hide widget
        if( wasVisible ) widget->hide();

        // set translucent flag
        widget->setAttribute( Qt::WA_TranslucentBackground );

        /*
        reset WA_Moved flag, which is incorrectly set to true when
        window is replaced by its transparent counterpart
        */
        if( !moved ) widget->setAttribute(Qt::WA_Moved, false);

        // show widget if was originally visible
        if( wasVisible ) widget->show();

        // re-install icon
        widget->setWindowIcon(icon);

        // add to set of transparent widgets and connect destruction signal
        _transparentWidgets.insert( widget );
        connect( widget, SIGNAL( destroyed( QObject* ) ), SLOT( unregisterTransparentWidget( QObject* ) ) );

        if( widget->testAttribute(Qt::WA_WState_Created) && widget->internalWinId() )
        {

            // set Argb xproperty, if already possible
            _helper.setHasArgb( widget->winId(), true );

        } else {

            // install event filter to set window property on WinIdChange event
            widget->installEventFilter( this );

        }

        return;
    }

    //______________________________________________________________
    bool ArgbHelper::isXEmbed( QWidget* widget ) const
    {

        #ifdef Q_WS_X11

        // QTextStream( stdout ) << "ArgbHelper::isXEmbed" << endl;

        Atom type = None;
        int format = 0;
        unsigned char *data = 0x0;
        unsigned long count = 0;
        unsigned long after = 0;
        const int length = 32768;

        // get window property
        return XGetWindowProperty(
            QX11Info::display(), widget->winId(), _xEmbedAtom,
            0L, length, false, XA_ATOM, &type, &format, &count, &after, &data) == Success && data;

        #else
        return false;
        #endif

    }

}
