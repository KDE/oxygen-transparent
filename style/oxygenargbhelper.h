#ifndef oxygenargbhelper_h
#define oxygenargbhelper_h

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

#include "oxygenstylehelper.h"

#include <QObject>
#include <QSet>
#include <QStringList>
#include <QApplication>
#include <QWidget>

#ifdef HAVE_X11
#include <xcb/xcb.h>
#endif

namespace Oxygen
{
    class ArgbHelper: public QObject
    {

        Q_OBJECT

        public:

        //! constructor
        ArgbHelper( QObject*, StyleHelper&  );

        //! register application
        void registerApplication( QApplication* );

        //! register widget
        bool registerWidget( QWidget* );

        //! unregister
        bool unregisterWidget( QWidget* );

        //! application name
        enum AppType
        {
            AppUnknown,
            AppPlasma,
            AppBlackListed
        };

        //! application type
        AppType applicationType( void ) const
        { return _applicationType; }

        //! true if application is black-listed
        bool isBlackListed( void ) const
        { return _applicationType == AppBlackListed; }

        //! true if widget is blackListed
        inline bool isBlackListed( const QWidget* ) const;

        //! true if enabled
        bool enabled( void ) const
        { return _enabled; }

        //! opacity
        bool setOpacity( int value )
        {

            if( _opacity == value ) return false;
            _opacity = value;
            _enabled = (value<0xff);
            return true;

        }

        //! opacity
        int opacity( void ) const
        { return _opacity; }

        //! black list
        void setBlackList( const QStringList blackList )
        { _blackList = blackList; }

        //! returns true if widget is registered
        bool isRegistered( QWidget* widget ) const
        { return _transparentWidgets.contains( widget ); }

        //! returns translucent color
        QColor translucentColor( const QColor& color, const QWidget* widget ) const
        { return translucentColor( color, _helper.hasAlphaChannel( widget ) ); }

        //! returns translucent color
        inline QColor translucentColor( const QColor& color, bool hasAlpha ) const;

        //! event filter
        virtual bool eventFilter( QObject*, QEvent*  );

        protected Q_SLOTS:

        //! unregister transparent widget
        void unregisterTransparentWidget( QObject* object )
        { _transparentWidgets.remove( object ); }

        protected:

        // transparency
        void setupTransparency( QWidget* );

        // true if window is of type X11 embed
        bool isXEmbed( QWidget* ) const;

        private:

        //! helper
        StyleHelper& _helper;

        //! application name
        AppType _applicationType;

        //! black list (unfortunately)
        QStringList _blackList;

        //! enable state
        bool _enabled;

        //! opacity
        int _opacity;

        //! set of transparent widgets (as defined in ::polish)
        typedef QSet<const QObject*> ObjectSet;
        ObjectSet _transparentWidgets;


        #ifdef HAVE_X11

        //! XEmbed atom
        xcb_atom_t _xEmbedAtom;

        #endif

    };

    // inline functions
    bool ArgbHelper::isBlackListed( const QWidget* widget ) const
    {
        return isBlackListed() ||
            ( _applicationType == AppPlasma && !(
            widget->inherits( "QDialog" ) ||
            widget->inherits( "QMenu" ) ) );
    }

    QColor ArgbHelper::translucentColor( const QColor& color, bool hasAlpha ) const
    {
        if( hasAlpha && _enabled )
        {
            QColor out( color );
            out.setAlpha( _opacity );
            return out;
        } else return color;

    }

}

#endif
