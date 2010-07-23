//////////////////////////////////////////////////////////////////////////////
// oxygenblacklistmodel.cpp
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
#include <KLocale>
#include <KLineEdit>

namespace Oxygen
{
    
    //_______________________________________________
    const QString BlackListModel::columnTitles_[ BlackListModel::nColumns ] =
    { 
        "",
        i18n("Application Name") 
    };
    
    //__________________________________________________________________
    QVariant BlackListModel::data( const QModelIndex& index, int role ) const
    {
        
        // check index, role and column
        if( !index.isValid() ) return QVariant();
        
        // retrieve associated file info
        const BlackListPair& pair( get(index) );
        
        // return text associated to file and column
        if( index.column() == NAME ) 
        {
            
            if( role == Qt::DisplayRole ) return pair.first;

        } else if( index.column() == ENABLED ) {
        
            if( role == Qt::CheckStateRole ) return pair.second ? Qt::Checked : Qt::Unchecked;
        
        }
        
        return QVariant();
    }
    
    //__________________________________________________________________
    bool BlackListModel::setData(const QModelIndex &index, const QVariant& value, int role )
    {

        if( !(index.isValid() && index.column() == NAME && role == Qt::EditRole ) ) return false;
        BlackListPair &pair( get(index) );        
        pair.first = value.toString();
        add( pair );
        
        return true;
        
    }
    
    //__________________________________________________________________
    Qt::ItemFlags BlackListModel::flags(const QModelIndex &index) const
    {
        if (!index.isValid()) return Qt::ItemFlags();
        else if( index.column() == ENABLED ) return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        else return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
    
    //____________________________________________________________
    void BlackListModel::privateSort( int column, Qt::SortOrder order )
    { std::sort( _get().begin(), _get().end(), SortFTor( (ColumnType) column, order ) ); }
    
    //________________________________________________________
    bool BlackListModel::SortFTor::operator () ( BlackListPair first, BlackListPair second ) const
    {
        
        if( _order == Qt::AscendingOrder ) std::swap( first, second );
        switch( _type )
        {
            case NAME: return first.first < second.first;
            case ENABLED: return first.second < second.second;
            default: return true;
        }
        
    }  
    
    //______________________________________________________________
    QWidget* Delegate::createEditor( QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& ) const
    {
        KLineEdit *editor = new KLineEdit( parent );
        editor->setFrame( false );
        return editor;
    }
    
    //______________________________________________________________
    void Delegate::setEditorData(QWidget *widget, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::DisplayRole).toString();
        KLineEdit *editor = static_cast<KLineEdit*>(widget);
        editor->setText( text );
    }
    
    //______________________________________________________________
    void Delegate::setModelData(QWidget *widget, QAbstractItemModel *model, const QModelIndex &index) const
    {
        KLineEdit *editor = static_cast<KLineEdit*>(widget);
        QString value( editor->text() );
        model->setData( index, value, Qt::EditRole);
    }
        
}
