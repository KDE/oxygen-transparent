#ifndef oxygenblacklistmodel_h
#define oxygenblacklistmodel_h
//////////////////////////////////////////////////////////////////////////////
// oxygenblacklistmodel.h
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

#include "oxygenlistmodel.h"

#include <QtCore/QString>
#include <QtGui/QStyledItemDelegate>

namespace Oxygen
{
    
    typedef QPair<QString, bool> BlackListPair;
    
    //! qlistview for object counters
    class BlackListModel: public ListModel<BlackListPair>
    {
        public:
        
        //! number of columns
        enum { nColumns = 2 };
        
        //! column type enumeration
        enum ColumnType { 
            ENABLED,
            NAME 
        };
        
        //!@name methods reimplemented from base class
        //@{
        
        // return data for a given index
        virtual QVariant data( const QModelIndex&, int ) const;

        // modify data
        virtual bool setData( const QModelIndex&, const QVariant&, int = Qt::EditRole );
        
        //! header data
        virtual QVariant headerData( int, Qt::Orientation, int = Qt::DisplayRole) const
        { return QVariant(); }
        
        //! flags
        virtual Qt::ItemFlags flags(const QModelIndex&) const;
        
        //! number of columns for a given index
        virtual int columnCount(const QModelIndex& ) const
        { return nColumns; }
        
        //@}
        
        protected:
        
        //! sort
        virtual void _sort( int, Qt::SortOrder );
        
        private:
        
        //! used to sort IconCaches
        class SortFTor: public ItemModel::SortFTor
        {
            
            public:
            
            //! constructor
            SortFTor( const int& type, Qt::SortOrder order = Qt::AscendingOrder ):
                ItemModel::SortFTor( type, order )
                {}
                
            //! prediction
            bool operator() ( BlackListPair, BlackListPair ) const;
                
        };
            
        //! column titles
        static const QString columnTitles_[ nColumns ];
            
    };

    class Delegate : public QStyledItemDelegate
    {

        public:
        
        //! constructor
        Delegate(QObject *parent = 0):
            QStyledItemDelegate( parent )
            {}
        
        //! create editor
        QWidget *createEditor(QWidget*, const QStyleOptionViewItem&, const QModelIndex& ) const;
        
        //! set editor data
        void setEditorData( QWidget*, const QModelIndex& ) const;
        
        //! set model data from editor
        void setModelData( QWidget*, QAbstractItemModel*, const QModelIndex& ) const;
        
        //! editor geometry
        void updateEditorGeometry( QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex& ) const
        { editor->setGeometry(option.rect); }
        
    };

}
#endif
