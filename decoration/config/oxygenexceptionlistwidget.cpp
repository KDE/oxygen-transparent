//////////////////////////////////////////////////////////////////////////////
// oxygenexceptionlistwidget.cpp
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

#include "oxygenexceptionlistwidget.h"
#include "oxygenexceptionlistwidget.moc"
#include "oxygenexceptiondialog.h"

#include <QtCore/QSharedPointer>
#include <KLocale>
#include <KMessageBox>

//__________________________________________________________
namespace Oxygen
{

    //__________________________________________________________
    ExceptionListWidget::ExceptionListWidget( QWidget* parent, Configuration default_configuration ):
        QWidget( parent ),
        defaultConfiguration_( default_configuration ),
        opacityFromStyle_( true )
    {

        //! ui
        ui.setupUi( this );

        // list
        ui.exceptionListView->setAllColumnsShowFocus( true );
        ui.exceptionListView->setRootIsDecorated( false );
        ui.exceptionListView->setSortingEnabled( false );
        ui.exceptionListView->setModel( &model() );
        ui.exceptionListView->sortByColumn( ExceptionModel::TYPE );
        ui.exceptionListView->setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Ignored ) );

        KIconLoader* iconLoader = KIconLoader::global();
        ui.moveUpButton->setIcon( KIcon( "arrow-up", iconLoader ) );
        ui.moveDownButton->setIcon( KIcon( "arrow-down", iconLoader ) );
        ui.addButton->setIcon( KIcon( "list-add", iconLoader ) );
        ui.removeButton->setIcon( KIcon( "list-remove", iconLoader ) );
        ui.editButton->setIcon( KIcon( "edit-rename", iconLoader ) );

        connect( ui.addButton, SIGNAL( clicked() ), SLOT( add() ) );
        connect( ui.editButton, SIGNAL( clicked() ), SLOT( edit() ) );
        connect( ui.removeButton, SIGNAL( clicked() ), SLOT( remove() ) );
        connect( ui.moveUpButton, SIGNAL( clicked() ), SLOT( up() ) );
        connect( ui.moveDownButton, SIGNAL( clicked() ), SLOT( down() ) );

        connect( ui.exceptionListView, SIGNAL( activated( const QModelIndex& ) ), SLOT( edit() ) );
        connect( ui.exceptionListView, SIGNAL( clicked( const QModelIndex& ) ), SLOT( toggle( const QModelIndex& ) ) );
        connect( ui.exceptionListView->selectionModel(), SIGNAL( selectionChanged(const QItemSelection &, const QItemSelection &) ), SLOT( updateButtons() ) );

        updateButtons();
        resizeColumns();


    }

    //__________________________________________________________
    void ExceptionListWidget::setExceptions( const ExceptionList& exceptions )
    {
        model().set( ExceptionModel::List( exceptions.begin(), exceptions.end() ) );
        resizeColumns();
    }

    //__________________________________________________________
    ExceptionList ExceptionListWidget::exceptions( void ) const
    {

        ExceptionModel::List exceptions( model().get() );
        ExceptionList out;
        for( ExceptionModel::List::const_iterator iter = exceptions.begin(); iter != exceptions.end(); ++iter )
        { out.push_back( *iter ); }
        return out;

    }

    //__________________________________________________________
    void ExceptionListWidget::updateButtons( void )
    {

        bool has_selection( !ui.exceptionListView->selectionModel()->selectedRows().empty() );
        ui.removeButton->setEnabled( has_selection );
        ui.editButton->setEnabled( has_selection );

        ui.moveUpButton->setEnabled( has_selection && !ui.exceptionListView->selectionModel()->isRowSelected( 0, QModelIndex() ) );
        ui.moveDownButton->setEnabled( has_selection && !ui.exceptionListView->selectionModel()->isRowSelected( model().rowCount()-1, QModelIndex() ) );

    }


    //_______________________________________________________
    void ExceptionListWidget::add( void )
    {

        // update opacityFromStyle setting
        defaultConfiguration_.setOpacityFromStyle( opacityFromStyle_ );

        // map dialog
        QSharedPointer<ExceptionDialog> dialog( new ExceptionDialog( this ) );
        dialog->setException( defaultConfiguration_ );
        if( dialog->exec() == QDialog::Rejected ) return;

        // retrieve exception and check
        Exception exception( dialog->exception() );
        if( !checkException( exception ) ) return;

        // create new item
        model().add( exception );

        // make sure item is selected
        QModelIndex index( model().index( exception ) );
        if( index != ui.exceptionListView->selectionModel()->currentIndex() )
        {
            ui.exceptionListView->selectionModel()->select( index,  QItemSelectionModel::Clear|QItemSelectionModel::Select|QItemSelectionModel::Rows );
            ui.exceptionListView->selectionModel()->setCurrentIndex( index,  QItemSelectionModel::Current|QItemSelectionModel::Rows );
        }

        resizeColumns();
        emit changed();
        return;

    }

    //_______________________________________________________
    void ExceptionListWidget::edit( void )
    {

        // retrieve selection
        QModelIndex current( ui.exceptionListView->selectionModel()->currentIndex() );
        if( !current.isValid() ) return;

        // retrieve exception
        // update opacityFromStyle setting
        Exception& exception( model().get( current ) );
        exception.setOpacityFromStyle( opacityFromStyle_ );

        // create dialog
        QSharedPointer<ExceptionDialog> dialog( new ExceptionDialog( this ) );
        dialog->setException( exception );

        // map dialog
        if( dialog->exec() == QDialog::Rejected ) return;
        Exception newException = dialog->exception();

        // check if exception was changed
        if( exception == newException ) return;

        // check new exception validity
        if( !checkException( newException ) ) return;

        // asign new exception
        *&exception = newException;
        resizeColumns();
        emit changed();
        return;

    }

    //_______________________________________________________
    void ExceptionListWidget::remove( void )
    {

        // should use a konfirmation dialog
        if( KMessageBox::questionYesNo( this, i18n("Remove selected exception?") ) == KMessageBox::No ) return;

        // remove
        model().remove( model().get( ui.exceptionListView->selectionModel()->selectedRows() ) );
        resizeColumns();
        emit changed();
        return;

    }

    //_______________________________________________________
    void ExceptionListWidget::toggle( const QModelIndex& index )
    {

        if( !index.isValid() ) return;
        if( index.column() != ExceptionModel::ENABLED ) return;

        // get matching exception
        Exception& exception( model().get( index ) );
        exception.setEnabled( !exception.enabled() );
        model().add( exception );

        emit changed();
        return;

    }

    //_______________________________________________________
    void ExceptionListWidget::up( void )
    {

        ExceptionModel::List selection( model().get( ui.exceptionListView->selectionModel()->selectedRows() ) );
        if( selection.empty() ) { return; }

        // retrieve selected indexes in list and store in model
        QModelIndexList selected_indexes( ui.exceptionListView->selectionModel()->selectedRows() );
        ExceptionModel::List selected_exceptions( model().get( selected_indexes ) );

        ExceptionModel::List current_exceptions( model().get() );
        ExceptionModel::List newExceptions;

        for( ExceptionModel::List::const_iterator iter = current_exceptions.begin(); iter != current_exceptions.end(); ++iter )
        {

            // check if new list is not empty, current index is selected and last index is not.
            // if yes, move.
            if(
                !( newExceptions.empty() ||
                selected_indexes.indexOf( model().index( *iter ) ) == -1 ||
                selected_indexes.indexOf( model().index( newExceptions.back() ) ) != -1
                ) )
            {
                Exception last( newExceptions.back() );
                newExceptions.pop_back();
                newExceptions.push_back( *iter );
                newExceptions.push_back( last );
            } else newExceptions.push_back( *iter );

        }

        model().set( newExceptions );

        // restore selection
        ui.exceptionListView->selectionModel()->select( model().index( selected_exceptions.front() ),  QItemSelectionModel::Clear|QItemSelectionModel::Select|QItemSelectionModel::Rows );
        for( ExceptionModel::List::const_iterator iter = selected_exceptions.begin(); iter != selected_exceptions.end(); ++iter )
        { ui.exceptionListView->selectionModel()->select( model().index( *iter ), QItemSelectionModel::Select|QItemSelectionModel::Rows ); }

        emit changed();
        return;

    }

    //_______________________________________________________
    void ExceptionListWidget::down( void )
    {

        ExceptionModel::List selection( model().get( ui.exceptionListView->selectionModel()->selectedRows() ) );
        if( selection.empty() )
        { return; }

        // retrieve selected indexes in list and store in model
        QModelIndexList selected_indexes( ui.exceptionListView->selectionModel()->selectedIndexes() );
        ExceptionModel::List selected_exceptions( model().get( selected_indexes ) );

        ExceptionModel::List current_exceptions( model().get() );
        ExceptionModel::List newExceptions;

        for( ExceptionModel::List::reverse_iterator iter = current_exceptions.rbegin(); iter != current_exceptions.rend(); ++iter )
        {

            // check if new list is not empty, current index is selected and last index is not.
            // if yes, move.
            if(
                !( newExceptions.empty() ||
                selected_indexes.indexOf( model().index( *iter ) ) == -1 ||
                selected_indexes.indexOf( model().index( newExceptions.back() ) ) != -1
                ) )
            {

                Exception last( newExceptions.back() );
                newExceptions.pop_back();
                newExceptions.push_back( *iter );
                newExceptions.push_back( last );

            } else newExceptions.push_back( *iter );
        }

        model().set( ExceptionModel::List( newExceptions.rbegin(), newExceptions.rend() ) );

        // restore selection
        ui.exceptionListView->selectionModel()->select( model().index( selected_exceptions.front() ),  QItemSelectionModel::Clear|QItemSelectionModel::Select|QItemSelectionModel::Rows );
        for( ExceptionModel::List::const_iterator iter = selected_exceptions.begin(); iter != selected_exceptions.end(); ++iter )
        { ui.exceptionListView->selectionModel()->select( model().index( *iter ), QItemSelectionModel::Select|QItemSelectionModel::Rows ); }

        emit changed();
        return;

    }

    //_______________________________________________________
    void ExceptionListWidget::resizeColumns( void ) const
    {
        ui.exceptionListView->resizeColumnToContents( ExceptionModel::ENABLED );
        ui.exceptionListView->resizeColumnToContents( ExceptionModel::TYPE );
        ui.exceptionListView->resizeColumnToContents( ExceptionModel::REGEXP );
    }

    //_______________________________________________________
    bool ExceptionListWidget::checkException( Exception& exception )
    {

        while( !exception.regExp().isValid() )
        {

            KMessageBox::error( this, i18n("Regular Expression syntax is incorrect") );
            QSharedPointer<ExceptionDialog> dialog( new ExceptionDialog( this ) );
            dialog->setException( exception );
            if( dialog->exec() == QDialog::Rejected ) return false;
            exception = dialog->exception();

        }

        return true;
    }

}
