#ifndef OFFER_H
#define OFFER_H

#include "bitcoinrpc.h"
#include "leveldb.h"
#include "script.h"
#include <vector>
class CTransaction;
class CTxOut;
class CValidationState;
class CCoinsViewCache;
class COutPoint;
class CCoins;
class CScript;
class CWalletTx;
class CDiskTxPos;

bool CheckOfferInputs(CBlockIndex *pindex, const CTransaction &tx, CValidationState &state, CCoinsViewCache &inputs, bool fBlock, bool fMiner, bool fJustCheck);
bool ExtractOfferAddress(const CScript& script, std::string& address);
bool IsOfferMine(const CTransaction& tx);
bool IsOfferMine(const CTransaction& tx, const CTxOut& txout);
std::string SendOfferMoneyWithInputTx(CScript scriptPubKey, int64 nValue, int64 nNetFee, CWalletTx& wtxIn, CWalletTx& wtxNew, 
    bool fAskFee, const std::string& txData = "");
bool CreateOfferTransactionWithInputTx(const std::vector<std::pair<CScript, int64> >& vecSend, CWalletTx& wtxIn, int nTxOut, 
    CWalletTx& wtxNew, CReserveKey& reservekey, int64& nFeeRet, const std::string& txData);

bool DecodeOfferTx(const CTransaction& tx, int& op, int& nOut, std::vector<std::vector<unsigned char> >& vvch, int nHeight);
bool DecodeOfferTx(const CCoins& tx, int& op, int& nOut, std::vector<std::vector<unsigned char> >& vvch, int nHeight);
bool DecodeOfferScript(const CScript& script, int& op, std::vector<std::vector<unsigned char> > &vvch);
bool IsOfferOp(int op);
int IndexOfOfferOutput(const CTransaction& tx);
uint64 GetOfferFeeSubsidy(unsigned int nHeight);
bool GetValueOfOfferTxHash(const uint256 &txHash, std::vector<unsigned char>& vchValue, uint256& hash, int& nHeight);
int64 GetOfferTxHashHeight(const uint256 txHash);
int GetOfferTxPosHeight(const CDiskTxPos& txPos);
int GetOfferTxPosHeight2(const CDiskTxPos& txPos, int nHeight);
int GetOfferDisplayExpirationDepth();
int64 GetOfferNetworkFee(const std::vector<unsigned char>& vchCurrency, opcodetype seed, unsigned int nHeight);
int64 GetOfferNetFee(const CTransaction& tx);
bool InsertOfferFee(CBlockIndex *pindex, uint256 hash, uint64 nValue);

std::string offerFromOp(int op);
static const char* norefund = "norefund";
static const char* inprogress = "inprogress";
static const char* complete = "complete";
static const std::vector<unsigned char> OFFER_NOREFUND = std::vector<unsigned char>(norefund, norefund + strlen(norefund));
static const std::vector<unsigned char> OFFER_REFUND_PAYMENT_INPROGRESS = std::vector<unsigned char>(inprogress, inprogress + strlen(inprogress));
static const std::vector<unsigned char> OFFER_REFUND_COMPLETE = std::vector<unsigned char>(complete, complete + strlen(complete));

class CBitcoinAddress;
class COfferAccept {
public:
	std::vector<unsigned char> vchRand;
    std::vector<unsigned char> vchMessage;
    std::vector<unsigned char> vchAddress;
	uint256 txHash;
	uint64 nHeight;
	uint64 nTime;
	uint64 nQty;
	uint64 nPrice;
	uint64 nFee;
	bool bPaid;
	bool bRefunded;
    uint256 txPayId;
	uint256 txRefundId;
	std::vector<unsigned char> vchRefundAddress;
	std::vector<unsigned char> vchLinkOfferAccept;
	COfferAccept() {
        SetNull();
    }

    IMPLEMENT_SERIALIZE (
        READWRITE(vchRand);
        READWRITE(vchMessage);
        READWRITE(vchAddress);
		READWRITE(txHash);
        READWRITE(txPayId);
		READWRITE(nHeight);
    	READWRITE(nTime);
        READWRITE(nQty);
    	READWRITE(nPrice);
        READWRITE(nFee);
    	READWRITE(bPaid);
		READWRITE(bRefunded);
		READWRITE(txRefundId);
		READWRITE(vchRefundAddress);
		READWRITE(vchLinkOfferAccept);
    )

    friend bool operator==(const COfferAccept &a, const COfferAccept &b) {
        return (
        a.vchRand == b.vchRand
        && a.vchMessage == b.vchMessage
        && a.vchAddress == b.vchAddress
        && a.txHash == b.txHash
        && a.nHeight == b.nHeight
        && a.nTime == b.nTime
        && a.nQty == b.nQty
        && a.nPrice == b.nPrice
        && a.nFee == b.nFee
        && a.bPaid == b.bPaid
        && a.txPayId == b.txPayId
		&& a.bRefunded == b.bRefunded
		&& a.txRefundId == b.txRefundId
		&& a.vchRefundAddress == b.vchRefundAddress
		&& a.vchLinkOfferAccept == b.vchLinkOfferAccept
        );
    }

    COfferAccept operator=(const COfferAccept &b) {
        vchRand = b.vchRand;
        vchMessage = b.vchMessage;
        vchAddress = b.vchAddress;
        txHash = b.txHash;
        nHeight = b.nHeight;
        nTime = b.nTime;
        nQty = b.nQty;
        nPrice = b.nPrice;
        nFee = b.nFee;
        bPaid = b.bPaid;
        txPayId = b.txPayId;
		bRefunded = b.bRefunded;
		txRefundId = b.txRefundId;
		vchRefundAddress = b.vchRefundAddress;
		vchLinkOfferAccept = b.vchLinkOfferAccept;
        return *this;
    }

    friend bool operator!=(const COfferAccept &a, const COfferAccept &b) {
        return !(a == b);
    }

    void SetNull() { nHeight = nTime = nPrice = nQty = 0; txHash = 0; bPaid = false; txPayId = 0; txRefundId=0; bRefunded=false;vchRefundAddress.clear();vchLinkOfferAccept.clear(); }
    bool IsNull() const { return (nTime == 0 && txHash == 0 && nHeight == 0 && nPrice == 0 && nQty == 0 && bPaid == 0 && txPayId == 0 && bRefunded == false && txRefundId == 0); }

};
class COfferLinkWhitelistEntry {
public:
	std::vector<unsigned char> certLinkVchRand;
	int nDiscountPct;
	COfferLinkWhitelistEntry() {
		SetNull();
	}

    IMPLEMENT_SERIALIZE (
        READWRITE(certLinkVchRand);
		READWRITE(nDiscountPct);
	)

    friend bool operator==(const COfferLinkWhitelistEntry &a, const COfferLinkWhitelistEntry &b) {
        return (
           a.certLinkVchRand == b.certLinkVchRand
		&& a.nDiscountPct == b.nDiscountPct
        );
    }

    COfferLinkWhitelistEntry operator=(const COfferLinkWhitelistEntry &b) {
    	certLinkVchRand = b.certLinkVchRand;
		nDiscountPct = b.nDiscountPct;
        return *this;
    }

    friend bool operator!=(const COfferLinkWhitelistEntry &a, const COfferLinkWhitelistEntry &b) {
        return !(a == b);
    }
    
    void SetNull() { certLinkVchRand.clear(); nDiscountPct = 0;}
    bool IsNull() const { return (certLinkVchRand.empty() && nDiscountPct == 0); }

};
class COfferLinkWhitelist {
public:
	std::vector<COfferLinkWhitelistEntry> entries;
	bool bExclusiveResell;
	COfferLinkWhitelist() {
		SetNull();
	}

    IMPLEMENT_SERIALIZE (
        READWRITE(entries);
		READWRITE(bExclusiveResell);

	)
    bool GetLinkEntryByHash(std::vector<unsigned char> ahash, COfferLinkWhitelistEntry &entry) {
    	for(unsigned int i=0;i<entries.size();i++) {
    		if(entries[i].certLinkVchRand == ahash) {
    			entry = entries[i];
    			return true;
    		}
    	}
    	return false;
    }
    bool RemoveWhitelistEntry(std::vector<unsigned char> ahash) {
    	for(unsigned int i=0;i<entries.size();i++) {
    		if(entries[i].certLinkVchRand == ahash) {
    			return entries.erase(entries.begin()+i) != entries.end();
    		}
    	}
    	return false;
    }	
    void PutWhitelistEntry(COfferLinkWhitelistEntry &theEntry) {
    	for(unsigned int i=0;i<entries.size();i++) {
    		COfferLinkWhitelistEntry entry = entries[i];
    		if(theEntry.certLinkVchRand == entry.certLinkVchRand) {
    			entries[i] = theEntry;
    			return;
    		}
    	}
    	entries.push_back(theEntry);
    }
    friend bool operator==(const COfferLinkWhitelist &a, const COfferLinkWhitelist &b) {
        return (
           a.entries == b.entries
		&& a.bExclusiveResell == b.bExclusiveResell

        );
    }

    COfferLinkWhitelist operator=(const COfferLinkWhitelist &b) {
    	entries = b.entries;
		bExclusiveResell = b.bExclusiveResell;
        return *this;
    }

    friend bool operator!=(const COfferLinkWhitelist &a, const COfferLinkWhitelist &b) {
        return !(a == b);
    }
    
    void SetNull() { entries.clear();bExclusiveResell = false;}
    bool IsNull() const { return (entries.empty() && bExclusiveResell == false);}

};
class COffer {
public:
	std::vector<unsigned char> vchRand;
    std::vector<unsigned char> vchPaymentAddress;
    uint256 txHash;
    uint64 nHeight;
    uint64 nTime;
    uint256 hash;
    uint64 n;
	std::vector<unsigned char> sCategory;
	std::vector<unsigned char> sTitle;
	std::vector<unsigned char> sDescription;
	uint64 nPrice;
	uint64 nQty;
	uint64 nFee;
	std::vector<COfferAccept>accepts;
	std::vector<unsigned char> vchLinkOffer;
	std::vector<unsigned char> sCurrencyCode;
	COfferLinkWhitelist linkWhitelist;
	COffer() { 
        SetNull();
    }

    COffer(const CTransaction &tx) {
        SetNull();
        UnserializeFromTx(tx);
    }

    IMPLEMENT_SERIALIZE (
        READWRITE(vchRand);
        READWRITE(vchPaymentAddress);
        READWRITE(sCategory);
        READWRITE(sTitle);
        READWRITE(sDescription);
		READWRITE(txHash);
        READWRITE(hash);
		READWRITE(nHeight);
		READWRITE(nTime);
    	READWRITE(n);
    	READWRITE(nPrice);
    	READWRITE(nQty);
    	READWRITE(nFee);
    	READWRITE(accepts);
		READWRITE(vchLinkOffer);
		READWRITE(linkWhitelist);
		READWRITE(sCurrencyCode);
		
	)
	uint64 GetPrice(const COfferLinkWhitelistEntry& entry=COfferLinkWhitelistEntry()){
		double price = (double)nPrice;
		if(!entry.IsNull())
		{
			if(entry.nDiscountPct < -99 || entry.nDiscountPct > 99)
				return (uint64)price;
			double fDiscount = (double)entry.nDiscountPct / 100.0;
			price -= fDiscount*price;

		}
		return (uint64)price;
	}
    bool GetAcceptByHash(std::vector<unsigned char> ahash, COfferAccept &ca) {
    	for(unsigned int i=0;i<accepts.size();i++) {
    		if(accepts[i].vchRand == ahash) {
    			ca = accepts[i];
    			return true;
    		}
    	}
    	return false;
    }

    void PutOfferAccept(COfferAccept &theOA) {
    	for(unsigned int i=0;i<accepts.size();i++) {
    		COfferAccept oa = accepts[i];
    		if(theOA.vchRand == oa.vchRand) {
    			accepts[i] = theOA;
    			return;
    		}
    	}
    	accepts.push_back(theOA);
    }

    void PutToOfferList(std::vector<COffer> &offerList) {
        for(unsigned int i=0;i<offerList.size();i++) {
            COffer o = offerList[i];
            if(o.nHeight == nHeight) {
                offerList[i] = *this;
                return;
            }
        }
        offerList.push_back(*this);
    }

    bool GetOfferFromList(const std::vector<COffer> &offerList) {
        if(offerList.size() == 0) return false;
        for(unsigned int i=0;i<offerList.size();i++) {
            COffer o = offerList[i];
            if(o.nHeight == nHeight) {
                *this = offerList[i];
                return true;
            }
        }
        *this = offerList.back();
        return false;
    }

    friend bool operator==(const COffer &a, const COffer &b) {
        return (
           a.vchRand == b.vchRand
        && a.sCategory==b.sCategory
        && a.sTitle == b.sTitle 
        && a.sDescription == b.sDescription 
        && a.nPrice == b.nPrice 
        && a.nQty == b.nQty 
        && a.nFee == b.nFee
        && a.n == b.n
        && a.hash == b.hash
        && a.txHash == b.txHash
        && a.nHeight == b.nHeight
        && a.nTime == b.nTime
        && a.accepts == b.accepts
        && a.vchPaymentAddress == b.vchPaymentAddress
		&& a.vchLinkOffer == b.vchLinkOffer
		&& a.linkWhitelist == b.linkWhitelist
		&& a.sCurrencyCode == b.sCurrencyCode
		
        );
    }

    COffer operator=(const COffer &b) {
    	vchRand = b.vchRand;
        sCategory = b.sCategory;
        sTitle = b.sTitle;
        sDescription = b.sDescription;
        nPrice = b.nPrice;
        nFee = b.nFee;
        nQty = b.nQty;
        n = b.n;
        hash = b.hash;
        txHash = b.txHash;
        nHeight = b.nHeight;
        nTime = b.nTime;
        accepts = b.accepts;
        vchPaymentAddress = b.vchPaymentAddress;
		vchLinkOffer = b.vchLinkOffer;
		linkWhitelist = b.linkWhitelist;
		sCurrencyCode = b.sCurrencyCode;
        return *this;
    }

    friend bool operator!=(const COffer &a, const COffer &b) {
        return !(a == b);
    }
    
    void SetNull() { nHeight = n = nPrice = nQty = 0; txHash = hash = 0; accepts.clear(); vchRand.clear(); sTitle.clear(); sDescription.clear();vchLinkOffer.clear();linkWhitelist.SetNull();sCurrencyCode.clear();}
    bool IsNull() const { return (n == 0 && txHash == 0 && hash == 0 && nHeight == 0 && nPrice == 0 && nQty == 0 &&  linkWhitelist.IsNull()); }

    bool UnserializeFromTx(const CTransaction &tx);
    void SerializeToTx(CTransaction &tx);
    std::string SerializeToString();
};

class COfferFee {
public:
	uint256 hash;
	uint64 nHeight;
	uint64 nTime;
	uint64 nFee;

	COfferFee() {
		nTime = 0; nHeight = 0; hash = 0; nFee = 0;
	}

	IMPLEMENT_SERIALIZE (
	    READWRITE(hash);
	    READWRITE(nHeight);
		READWRITE(nTime);
		READWRITE(nFee);
	)

    friend bool operator==(const COfferFee &a, const COfferFee &b) {
        return (
        a.nTime==b.nTime
        && a.hash==b.hash
        && a.nHeight==b.nHeight
        && a.nFee == b.nFee 
        );
    }

    COfferFee operator=(const COfferFee &b) {
        nTime = b.nTime;
        nFee = b.nFee;
        hash = b.hash;
        nHeight = b.nHeight;
        return *this;
    }

    friend bool operator!=(const COfferFee &a, const COfferFee &b) { return !(a == b); }
	void SetNull() { hash = nTime = nHeight = nFee = 0;}
    bool IsNull() const { return (nTime == 0 && nFee == 0 && hash == 0 && nHeight == 0); }
};
bool RemoveOfferFee(COfferFee &txnVal);

class COfferDB : public CLevelDB {
public:
	COfferDB(size_t nCacheSize, bool fMemory, bool fWipe) : CLevelDB(GetDataDir() / "offers", nCacheSize, fMemory, fWipe) {}

	bool WriteOffer(const std::vector<unsigned char>& name, std::vector<COffer>& vtxPos) {
		return Write(make_pair(std::string("offeri"), name), vtxPos);
	}

	bool EraseOffer(const std::vector<unsigned char>& name) {
	    return Erase(make_pair(std::string("offeri"), name));
	}

	bool ReadOffer(const std::vector<unsigned char>& name, std::vector<COffer>& vtxPos) {
		return Read(make_pair(std::string("offeri"), name), vtxPos);
	}

	bool ExistsOffer(const std::vector<unsigned char>& name) {
	    return Exists(make_pair(std::string("offeri"), name));
	}

	bool WriteOfferAccept(const std::vector<unsigned char>& name, std::vector<unsigned char>& vchValue) {
		return Write(make_pair(std::string("offera"), name), vchValue);
	}

	bool EraseOfferAccept(const std::vector<unsigned char>& name) {
	    return Erase(make_pair(std::string("offera"), name));
	}

	bool ReadOfferAccept(const std::vector<unsigned char>& name, std::vector<unsigned char>& vchValue) {
		return Read(make_pair(std::string("offera"), name), vchValue);
	}

	bool ExistsOfferAccept(const std::vector<unsigned char>& name) {
	    return Exists(make_pair(std::string("offera"), name));
	}

	bool WriteOfferTxFees(std::vector<COfferFee>& vtxPos) {
		return Write(make_pair(std::string("offera"), std::string("offertxf")), vtxPos);
	}

	bool ReadOfferTxFees(std::vector<COfferFee>& vtxPos) {
		return Read(make_pair(std::string("offera"), std::string("offertxf")), vtxPos);
	}

    bool WriteOfferIndex(std::vector<std::vector<unsigned char> >& vtxPos) {
        return Write(make_pair(std::string("offera"), std::string("offerndx")), vtxPos);
    }

    bool ReadOfferIndex(std::vector<std::vector<unsigned char> >& vtxPos) {
        return Read(make_pair(std::string("offera"), std::string("offerndx")), vtxPos);
    }

    bool ScanOffers(
            const std::vector<unsigned char>& vchName,
            unsigned int nMax,
            std::vector<std::pair<std::vector<unsigned char>, COffer> >& offerScan);

    bool ReconstructOfferIndex(CBlockIndex *pindexRescan);
};
extern std::list<COfferFee> lstOfferFees;


bool GetTxOfOffer(COfferDB& dbOffer, const std::vector<unsigned char> &vchOffer, COffer& txPos, CTransaction& tx);
#endif // OFFER_H
