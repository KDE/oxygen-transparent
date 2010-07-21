#ifndef oxygenblurhelper_h
#define oxygenblurhelper_h

//////////////////////////////////////////////////////////////////////////////
// oxygenblurhelper.h
// handle regions passed to kwin for blurring
// -------------------
//
// Copyright (c) 2010 Hugo Pereira Da Costa <hugo@oxygen-icons.org>
//
// Largely inspired from BeSpin style
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

#include <QtCore/QObject>
#include <QtGui/QRegion>

#ifdef Q_WS_X11
#include <X11/Xdefs.h>
#endif

namespace Oxygen
{
    class BlurHelper: public QObject
    {

        Q_OBJECT

        public:

        //! constructor
        BlurHelper( QObject*, StyleHelper& );

        //! destructor
        virtual ~BlurHelper( void )
        {}

        //! enable state
        virtual void setEnabled( bool value )
        { _enabled = value; }

        //! enabled
        virtual bool enabled( void ) const
        { return _enabled; }

        //! register widget
        virtual bool registerWidget( QWidget* );

        //! event filter
        virtual bool eventFilter( QObject*, QEvent* );

        protected:

        //! get list of blur-behind regions matching a given widget
        QRegion blurRegion( QWidget* ) const;

        //! trim blur region to remove unnecessary areas (recursive)
        void trimBlurRegion( QWidget*, QWidget*, QRegion& ) const;

        //! update blur regions for given widget
        void update( QWidget* ) const;

        //! clear blur regions for given widget
        void clear( QWidget* ) const;

        private:

        //! helper
        StyleHelper& _helper;

        //! enability
        bool _enabled;

        #ifdef Q_WS_X11
        //! blur atom
        Atom _atom;
        #endif

    };

}

#endif
