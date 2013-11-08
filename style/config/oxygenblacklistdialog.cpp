//////////////////////////////////////////////////////////////////////////////
// oxygenblacklistdialog.cpp
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

#include "oxygenblacklistdialog.h"
#include "oxygenblacklistdialog.moc"

#include <KLocalizedString>
#include <KMessageBox>

#include <QIcon>
#include <QTextStream>

namespace Oxygen
{

    //___________________________________________
    BlackListDialog::BlackListDialog( QWidget* parent ):
        QDialog( parent )
    {

        setWindowTitle( i18n( "Exceptions - Oxygen Settings" ) );

        // setup ui
        ui.setupUi( this );
        ui.listView->setModel( &_model );
        ui.listView->sortByColumn( BlackListModel::NAME );
        ui.listView->setItemDelegate( new Delegate( this ) );

        // set icons
        ui.addButton->setIcon( QIcon::fromTheme( QStringLiteral( "list-add" ) ) );
        ui.removeButton->setIcon( QIcon::fromTheme( QStringLiteral( "list-remove" ) ) );
        ui.editButton->setIcon( QIcon::fromTheme( QStringLiteral( "edit-rename" ) ) );

        // connections
        connect( ui.listView, SIGNAL( clicked( const QModelIndex& ) ), SLOT( toggle( const QModelIndex& ) ) );
        connect( ui.listView->selectionModel(), SIGNAL( selectionChanged(const QItemSelection &, const QItemSelection &) ), SLOT( updateButtons() ) );
        connect( ui.addButton, SIGNAL( clicked() ), SLOT( add() ) );
        connect( ui.removeButton, SIGNAL( clicked() ), SLOT( remove() ) );
        connect( ui.editButton, SIGNAL( clicked() ), SLOT( edit() ) );

        updateButtons();

    }

    //___________________________________________
    void BlackListDialog::setLists(
        const QStringList& internalList,
        const QStringList& greyList,
        const QStringList& blackList )
    {

        // store internal list
        _internalBlackList = internalList;

        typedef QMap<QString, bool> Map;
        Map blackListMap;

        // add internal
        /*
        all internals are set to disabled. They will be set to enabled
        if also found in grey list, and not in black list
        */
        foreach( const QString& appName, internalList )
        {  if( !appName.trimmed().isEmpty() )  blackListMap[appName] = true; }

        // add grey
        foreach( const QString& appName, greyList )
        { if( !appName.trimmed().isEmpty() ) blackListMap[appName] = false; }

        // add black
        foreach( const QString& appName, blackList )
        { if( !appName.trimmed().isEmpty() ) blackListMap[appName] = true; }

        // put map into model
        BlackListModel::List items;
        for( Map::const_iterator iter = blackListMap.constBegin(); iter != blackListMap.constEnd(); iter++ )
        { items.push_back( BlackListPair( iter.key(), iter.value() ) ); }

        _model.set( items );

        resizeColumns();

    }

    //___________________________________________
    QStringList BlackListDialog::greyList( void ) const
    {

        BlackListModel::List items( _model.get() );
        QStringList list;

        foreach( const BlackListPair& pair, items )
        { if( !pair.first.trimmed().isEmpty() ) list << pair.first; }

        return list;
    }

    //___________________________________________
    QStringList BlackListDialog::blackList( void ) const
    {

        BlackListModel::List items( _model.get() );
        QStringList list;

        foreach( const BlackListPair& pair, items )
        { if( pair.second && !pair.first.trimmed().isEmpty() ) list << pair.first; }

        return list;
    }

    //_______________________________________________________
    void BlackListDialog::resizeColumns( void ) const
    {
        ui.listView->resizeColumnToContents( BlackListModel::ENABLED );
        ui.listView->resizeColumnToContents( BlackListModel::NAME );
    }

    //__________________________________________________________
    void BlackListDialog::updateButtons( void )
    {

        QModelIndex index( ui.listView->selectionModel()->currentIndex() );
        const bool enabled( index.isValid() && _internalBlackList.indexOf( _model.get( index ).first ) < 0 );

        ui.removeButton->setEnabled( enabled );
        ui.editButton->setEnabled( enabled );

    }

    //_______________________________________________________
    void BlackListDialog::toggle( const QModelIndex& index )
    {

        if( !index.isValid() ) return;
        if( index.column() != BlackListModel::ENABLED ) return;

        // get matching exception
        BlackListPair& pair( _model.get( index ) );
        pair.second = !pair.second;
        _model.add( pair );

        return;

    }

    //_______________________________________________________
    void BlackListDialog::add( void )
    {

        BlackListPair pair( QString(), true );
        _model.add( pair );
        QModelIndex index( _model.index( pair ) );
        if( !index.isValid() ) return;

        // set index selected
        ui.listView->selectionModel()->select( index,  QItemSelectionModel::Clear|QItemSelectionModel::Select|QItemSelectionModel::Rows );

        // and edit
        index = _model.index( index.row(), BlackListModel::NAME );
        ui.listView->edit( index );

        return;

    }

    //_______________________________________________________
    void BlackListDialog::remove( void )
    {

        // get selection
        QModelIndex index( ui.listView->selectionModel()->currentIndex() );
        if( !index.isValid() ) return;

        // ask confirmation
        if( KMessageBox::questionYesNo( this, i18n("Remove selected exception?") ) == KMessageBox::No ) return;

        // remove
        _model.remove( _model.get( index ) );
        resizeColumns();
        return;

    }

    //_______________________________________________________
    void BlackListDialog::edit( void )
    {

        // retrieve selection
        QModelIndex index( ui.listView->selectionModel()->currentIndex() );
        if( !index.isValid() ) return;

        // make sure the correct column is selected for edition
        index = _model.index( index.row(), BlackListModel::NAME );

        // and edit
        ui.listView->edit( index );
        return;

    }

}
