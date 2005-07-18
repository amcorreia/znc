#ifndef _CHAN_H
#define _CHAN_H

#include "main.h"
#include "Nick.h"
#include "String.h"
#include "FileUtils.h"
#include <vector>
#include <map>
#include <set>
using std::vector;
using std::map;
using std::set;

// Forward Declarations
class CUser;
// !Forward Declarations

class CChan {
public:
	typedef enum {
		Voice	= '+',
		HalfOp 	= '%',
		Op		= '@',
		Admin	= '!',
		Owner	= '*'
	} EUserPerms;

	typedef enum {
		M_Private		= 'p',
		M_Secret		= 's',
		M_Moderated		= 'm',
		M_InviteOnly	= 'i',
		M_NoMessages	= 'n',
		M_OpTopic		= 't',
		M_Limit			= 'l',
		M_Key			= 'k',
		M_Op			= 'o',
		M_Voice			= 'v',
		M_Ban			= 'b',
		M_Except		= 'e',
	} EModes;

	CChan(const CString& sName, CUser* pUser, bool bInConfig);
	virtual ~CChan();

	void Reset();
	bool WriteConfig(CFile& File);
	void Joined();
	void Cycle() const;
	void JoinUser(bool bForce = false, const CString& sKey = "");
	void DetachUser();
	void SendBuffer();

	void IncClientRequests();
	bool DecClientRequests();

	bool Who();
	void OnWho(const CString& sNick, const CString& sIdent, const CString& sHost);

	// Modes
	void SetModes(const CString& s);
	void ModeChange(const CString& sModes, const CString& sNick = "");
	bool AddMode(unsigned char uMode, const CString& sArg);
	bool RemMode(unsigned char uMode, const CString& sArg);
	void OnOp(const CString& sOpNick, const CString& sNick, bool bOpped);
	void OnVoice(const CString& sOpNick, const CString& sNick, bool bVoiced);
	CString GetModeString() const;
	CString GetModeArg(CString& sArgs) const;
	// !Modes

	// Nicks
	void ClearNicks();
	CNick* FindNick(const CString& sNick) const;
	int AddNicks(const CString& sNicks);
	bool AddNick(const CString& sNick);
	bool RemNick(const CString& sNick);
	bool ChangeNick(const CString& sOldNick, const CString& sNewNick);
	// !Nicks

	// Buffer
	int AddBuffer(const CString& sLine);
	void ClearBuffer();
	// !Buffer

	// m_Nick wrappers
	CString GetPermStr() const { return m_Nick.GetPermStr(); }
	bool HasPerm(unsigned char uPerm) const { return m_Nick.HasPerm(uPerm); }
	bool AddPerm(unsigned char uPerm) { return m_Nick.AddPerm(uPerm); }
	bool RemPerm(unsigned char uPerm) { return m_Nick.RemPerm(uPerm); }
	// !wrappers

	// Setters
	void IncPermCount(unsigned char uPerm);
	void DecPermCount(unsigned char uPerm);
	void SetIsOn(bool b) { m_bIsOn = b; if (!b) { Reset(); } else { Joined(); } }
	void SetKey(const CString& s) { m_sKey = s; }
	void SetTopic(const CString& s) { m_sTopic = s; }
	void SetTopicOwner(const CString& s) { m_sTopicOwner = s; }
	void SetTopicDate(unsigned long u) { m_ulTopicDate = u; }
	void SetDefaultModes(const CString& s) { m_sDefaultModes = s; }
	void SetBufferCount(unsigned int u) { m_uBufferCount = u; }
	void SetKeepBuffer(bool b) { m_bKeepBuffer = b; }
	void SetAutoCycle(bool b) { m_bAutoCycle = b; }
	void SetWhoDone(bool b = true) { m_bWhoDone = b; }
	void SetDetached(bool b = true) { m_bDetached = b; }
	void SetInConfig(bool b) { m_bInConfig = b; }
	// !Setters

	// Getters
	bool HasMode(unsigned char uMode) const;
	CString GetModeArg(unsigned char uMode) const;
	unsigned int GetPermCount(unsigned char uPerm);
	const bool IsOn() const { return m_bIsOn; }
	const CString& GetName() const { return m_sName; }
	const map<unsigned char, CString>& GetModes() const { return m_musModes; }
	const CString& GetKey() const { return (!m_sCurKey.empty()) ? m_sCurKey : m_sKey; }
	unsigned int GetLimit() const { return m_uLimit; }
	const CString& GetTopic() const { return m_sTopic; }
	const CString& GetTopicOwner() const { return m_sTopicOwner; }
	unsigned int GetTopicDate() const { return m_ulTopicDate; }
	const CString& GetDefaultModes() const { return m_sDefaultModes; }
	const vector<CString>& GetBans() const { return m_vsBans; }
	const vector<CString>& GetBuffer() const { return m_vsBuffer; }
	const map<CString,CNick*>& GetNicks() const { return m_msNicks; }
	unsigned int GetNickCount() const { return m_msNicks.size(); }
	unsigned int GetBufferCount() const { return m_uBufferCount; }
	bool KeepBuffer() const { return m_bKeepBuffer; }
	bool AutoCycle() const { return m_bAutoCycle; }
	bool IsDetached() const { return m_bDetached; }
	bool InConfig() const { return m_bInConfig; }
	// !Getters
private:
protected:
	bool				m_bDetached;
	bool				m_bIsOn;
	bool				m_bWhoDone;
	bool				m_bKeepBuffer;
	bool				m_bAutoCycle;
	bool				m_bInConfig;
	CString				m_sName;
	CString				m_sKey;
	CString				m_sCurKey;
	CString				m_sTopic;
	CString				m_sTopicOwner;
	unsigned long		m_ulTopicDate;
	CUser*				m_pUser;
	CNick				m_Nick;
	unsigned int		m_uLimit;
	CString				m_sDefaultModes;
	vector<CString>		m_vsBans;
	map<CString,CNick*>	m_msNicks;	// Todo: make this caseless (irc style)
	set<unsigned int>	m_suUserPerms;
	unsigned int		m_uBufferCount;
	unsigned int		m_uClientRequests;	// Used to tell how many times a client tried to join this chan
	vector<CString>		m_vsBuffer;

	map<unsigned char, CString>			m_musModes;
	map<unsigned char, unsigned int>	m_muuPermCount;
};

#endif // !_CHAN_H
