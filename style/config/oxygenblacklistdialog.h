#ifndef oxygenblacklistdialog_h
#define oxygenblacklistdialog_h
//////////////////////////////////////////////////////////////////////////////
// oxygenblacklistdialog.h
// -------------------
//
// Copyright (c) 2009 Hugo Pereira Da Costa <hugo.pereira@free.fr>
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

#include "oxygenblacklistmodel.h"
#include "ui_oxygenblacklistdialog.h"

#include <KDialog>
#include <QtCore/QStringList>

namespace Oxygen
{

    //! oxygen black-listed applications list
    class BlackListDialog: public KDialog
    {

        Q_OBJECT

        public:

        //! constructor
        BlackListDialog( QWidget* parent );

        //! set lists
        /*! first argument is grey list. Second argument is blackList */
        void setLists( const QStringList&, const QStringList& );
    
        //! get grey list
        QStringList greyList( void ) const;
        
        //! get black list
        QStringList blackList( void ) const;

        protected:
        
        void resizeColumns( void ) const;

        protected slots:

        //! toggle
        virtual void toggle( const QModelIndex& );

        //! add
        virtual void add( void );

        //! remove
        virtual void remove( void );

        //! edit
        virtual void edit( void );

        //! update button states
        virtual void updateButtons( void );
        
        private:

        //! ui
        Ui_OxygenBlackListWidget ui;

        //! model
        BlackListModel _model;
        
    };

}

#endif
