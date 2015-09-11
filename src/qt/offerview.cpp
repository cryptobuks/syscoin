/*
 * Syscoin Developers 2015
 */
#include "offerview.h"
#include "bitcoingui.h"


#include "clientmodel.h"
#include "walletmodel.h"
#include "optionsmodel.h"
#include "myofferlistpage.h"
#include "acceptandpayofferlistpage.h"
#include "offerlistpage.h"
#include "offertablemodel.h"
#include "ui_interface.h"

#include <QAction>
#if QT_VERSION < 0x050000
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif
#include <QPushButton>

OfferView::OfferView(QStackedWidget *parent, BitcoinGUI *_gui):
    gui(_gui),
    clientModel(0),
    walletModel(0)
{

	tabWidget = new QTabWidget();
    offerListPage = new OfferListPage();
    myOfferListPage = new MyOfferListPage();
	acceptandPayOfferListPage = new AcceptandPayOfferListPage();

	tabWidget->addTab(myOfferListPage, tr("&My Offers"));
	tabWidget->addTab(offerListPage, tr("&Search"));
	tabWidget->addTab(acceptandPayOfferListPage, tr("&Purchase Offer"));
	tabWidget->setTabIcon(0, QIcon(":/icons/cart"));
	tabWidget->setTabIcon(1, QIcon(":/icons/search"));
	tabWidget->setTabIcon(2, QIcon(":/icons/send"));
	parent->addWidget(tabWidget);
}

OfferView::~OfferView()
{
}

void OfferView::setBitcoinGUI(BitcoinGUI *gui)
{
    this->gui = gui;
}
void OfferView::setClientModel(ClientModel *clientModel)
{
    this->clientModel = clientModel;
    if (clientModel)
    {
       
        offerListPage->setOptionsModel(clientModel->getOptionsModel());
		myOfferListPage->setOptionsModel(clientModel,clientModel->getOptionsModel());

    }
}

void OfferView::setWalletModel(WalletModel *walletModel)
{

    this->walletModel = walletModel;
    if (walletModel)
    {

        offerListPage->setModel(walletModel, walletModel->getOfferTableModelAll());
		myOfferListPage->setModel(walletModel, walletModel->getOfferTableModelMine());

    }
}


void OfferView::gotoOfferListPage()
{
	tabWidget->setCurrentWidget(offerListPage);
}


bool OfferView::handleURI(const QString& strURI)
{
 // URI has to be valid
    if (acceptandPayOfferListPage->handleURI(strURI))
    {
        tabWidget->setCurrentWidget(acceptandPayOfferListPage);
        emit showNormalIfMinimized();
        return true;
    }
    else if (offerListPage->handleURI(strURI))
    {
        gotoOfferListPage();
        emit showNormalIfMinimized();
        return true;
    }

    // URI has to be valid
    else if (myOfferListPage->handleURI(strURI))
    {
        tabWidget->setCurrentWidget(myOfferListPage);
        emit showNormalIfMinimized();
        return true;
    }
    
    return false;
}

