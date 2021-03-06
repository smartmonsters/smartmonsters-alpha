#ifndef HUNTERCOIN_H
#define HUNTERCOIN_H

#ifndef Q_MOC_RUN
#include <boost/thread/thread.hpp>
#endif

#include "json/json_spirit.h"

// We can make name_new cheaper, if we want, separately from name_(first)update
// This can be used e.g. to send short messages in the hash field. The coin will be
// destroyed in this case. We can try setting it to 0 though.
static const int64 NAMENEW_COIN_AMOUNT = COIN / 5;
static const int MAX_NAME_LENGTH = 10;
static const int MAX_VALUE_LENGTH = 4095;
static const int OP_NAME_INVALID = 0x00;
static const int OP_NAME_NEW = 0x01;
static const int OP_NAME_FIRSTUPDATE = 0x02;
static const int OP_NAME_UPDATE = 0x03;
static const int OP_NAME_NOP = 0x04;
static const int MIN_FIRSTUPDATE_DEPTH = 2;

/* Configuration for fees and stuff around OP_RETURN data.  */
static const int64 OPRETURN_MIN_LOCKED = CENT;
static const unsigned OPRETURN_MAX_STRLEN = 100;

// The following value is assigned to the name when the player is dead.
// It must not be a valid move JSON string, because it is checked in NameAvailable
// as a shortcut to reading tx and checking IsGameTx.
const static std::string VALUE_DEAD("{\"dead\":1}");

class CNameIndex;
class CDiskTxPos;
class uint256;

extern std::map<vchType, uint256> mapMyNames;
extern std::map<vchType, std::set<uint256> > mapNamePending;

std::string stringFromVch(const std::vector<unsigned char> &vch);
std::vector<unsigned char> vchFromString(const std::string &str);
int GetTxPosHeight(const CNameIndex& txPos);
int GetTxPosHeight(const CDiskTxPos& txPos);
CScript RemoveNameScriptPrefix(const CScript& scriptIn, bool strict = true);
bool NameAvailable (DatabaseSet& dbset, const vchType& vchName);
bool GetTxOfName (CNameDB& dbName, const vchType& vchName, CTransaction& tx);
bool GetTxOfNameAtHeight (CNameDB& dbName, const vchType& vchName,
                          int nHeight, CTransaction& tx);
int IndexOfNameOutput(const CTransaction& tx);
bool GetValueOfTxPos(const CNameIndex& txPos, std::vector<unsigned char>& vchValue, uint256& hash, int& nHeight);
bool GetValueOfTxPos(const CDiskTxPos& txPos, std::vector<unsigned char>& vchValue, uint256& hash, int& nHeight);
bool GetNameOfTx(const CTransaction& tx, std::vector<unsigned char>& name);
bool GetValueOfNameTx(const CTransaction& tx, std::vector<unsigned char>& value);
bool DecodeNameTx(const CTransaction& tx, int& op, int& nOut,
                  std::vector<vchType>& vvch);
bool DecodeNameScript(const CScript& script, int& op,
                      std::vector<vchType> &vvch, CScript::const_iterator& pc);
bool DecodeNameScript(const CScript& script, int& op,
                      std::vector<vchType> &vvch);
bool GetNameAddress(const CTransaction& tx, std::string& strAddress);
bool GetNameAddress(const CTransaction& tx, uint160 &hash160);
std::string SendMoneyWithInputTx(const CScript& scriptPubKey, int64 nValue, const CWalletTx& wtxIn, CWalletTx& wtxNew, bool fAskFee);
bool CreateTransactionWithInputTx(const std::vector<std::pair<CScript, int64> >& vecSend, const CWalletTx& wtxIn, int nTxOut, CWalletTx& wtxNew, CReserveKey& reservekey, int64& nFeeRet);
int64 GetNameCoinAmount (unsigned nHeight, bool frontEnd = false);
bool IsConflictedTx (DatabaseSet& dbset, const CTransaction& tx, vchType& name);
void UnspendInputs(CWalletTx& wtx);
bool IsValidPlayerName (const std::string& player);

/* For the front-end and the game_waitforchange RPC call, we allow threads
   to "register" to be notified when a new block is attached.  This is
   synchronised using the mut_currentState and the condition variable
   cv_stateChange.  When a new block is found, all threads waiting on
   cv_stateChange will be notified.  */
extern boost::mutex mut_currentState;
extern boost::condition_variable cv_stateChange;

/* Handle the name operation part of the RPC call createrawtransaction.  */
void AddRawTxNameOperation(CTransaction& tx, const json_spirit::Object& obj);

#endif // HUNTERCOIN_H
