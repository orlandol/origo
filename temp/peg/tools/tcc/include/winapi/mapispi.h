/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within this package.
 */
#ifndef MAPISPI_H
#define MAPISPI_H

#include <mapidefs.h>
#include <mapicode.h>
#include <mapiguid.h>
#include <mapitags.h>

#ifndef BEGIN_INTERFACE
#define BEGIN_INTERFACE
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CURRENT_SPI_VERSION 0x00010010L
#define PDK1_SPI_VERSION 0x00010000L
#define PDK2_SPI_VERSION 0x00010008L
#define PDK3_SPI_VERSION 0x00010010L

  DECLARE_MAPI_INTERFACE_PTR(IMAPISupport,LPMAPISUP);

  typedef struct {
    ULONG cb;
    BYTE ab[MAPI_DIM];
  } NOTIFKEY,*LPNOTIFKEY;

#define CbNewNOTIFKEY(_cb) (offsetof(NOTIFKEY,ab) + (_cb))
#define CbNOTIFKEY(_lpkey) (offsetof(NOTIFKEY,ab) + (_lpkey)->cb)
#define SizedNOTIFKEY(_cb,_name) struct _NOTIFKEY_ ## _name { ULONG cb; BYTE ab[_cb]; } _name

#define NOTIFY_SYNC ((ULONG) 0x40000000)
#define NOTIFY_CANCELED ((ULONG) 0x80000000)
#define CALLBACK_DISCONTINUE ((ULONG) 0x80000000)

#define NOTIFY_NEWMAIL ((ULONG) 0x00000001)
#define NOTIFY_READYTOSEND ((ULONG) 0x00000002)
#define NOTIFY_SENTDEFERRED ((ULONG) 0x00000004)
#define NOTIFY_CRITSEC ((ULONG) 0x00001000)
#define NOTIFY_NONCRIT ((ULONG) 0x00002000)
#define NOTIFY_CONFIG_CHANGE ((ULONG) 0x00004000)
#define NOTIFY_CRITICAL_ERROR ((ULONG) 0x10000000)
#define NOTIFY_NEWMAIL_RECEIVED ((ULONG) 0x20000000)

#define STATUSROW_UPDATE ((ULONG) 0x10000000)

#define STGSTRM_RESET ((ULONG) 0x00000000)
#define STGSTRM_CURRENT ((ULONG) 0x10000000)
#define STGSTRM_MODIFY ((ULONG) 0x00000002)
#define STGSTRM_CREATE ((ULONG) 0x00001000)

#define MAPI_NON_READ ((ULONG) 0x00000001)

  typedef HRESULT (WINAPI PREPROCESSMESSAGE)(LPVOID lpvSession,LPMESSAGE lpMessage,LPADRBOOK lpAdrBook,LPMAPIFOLDER lpFolder,LPALLOCATEBUFFER AllocateBuffer,LPALLOCATEMORE AllocateMore,LPFREEBUFFER FreeBuffer,ULONG *lpcOutbound,LPMESSAGE **lpppMessage,LPADRLIST *lppRecipList);
  typedef HRESULT (WINAPI REMOVEPREPROCESSINFO)(LPMESSAGE lpMessage);

#define MAPI_IMAPISUPPORT_METHODS1(IPURE) MAPIMETHOD(GetLastError) (THIS_ HRESULT hResult,ULONG ulFlags,LPMAPIERROR *lppMAPIError) IPURE; MAPIMETHOD(GetMemAllocRoutines) (THIS_ LPALLOCATEBUFFER *lpAllocateBuffer,LPALLOCATEMORE *lpAllocateMore,LPFREEBUFFER *lpFreeBuffer) IPURE; MAPIMETHOD(Subscribe) (THIS_ LPNOTIFKEY lpKey,ULONG ulEventMask,ULONG ulFlags,LPMAPIADVISESINK lpAdviseSink,ULONG *lpulConnection) IPURE; MAPIMETHOD(Unsubscribe) (THIS_ ULONG ulConnection) IPURE; MAPIMETHOD(Notify) (THIS_ LPNOTIFKEY lpKey,ULONG cNotification,LPNOTIFICATION lpNotifications,ULONG *lpulFlags) IPURE; MAPIMETHOD(ModifyStatusRow) (THIS_ ULONG cValues,LPSPropValue lpColumnVals,ULONG ulFlags) IPURE; MAPIMETHOD(OpenProfileSection) (THIS_ LPMAPIUID lpUid,ULONG ulFlags,LPPROFSECT *lppProfileObj) IPURE; MAPIMETHOD(RegisterPreprocessor) (THIS_ LPMAPIUID lpMuid,LPTSTR lpszAdrType,LPTSTR lpszDLLName,LPSTR lpszPreprocess,LPSTR lpszRemovePreprocessInfo,ULONG ulFlags) IPURE; MAPIMETHOD(NewUID) (THIS_ LPMAPIUID lpMuid) IPURE; MAPIMETHOD(MakeInvalid) (THIS_ ULONG ulFlags,LPVOID lpObject,ULONG ulRefCount,ULONG cMethods) IPURE;
#define MAPI_IMAPISUPPORT_METHODS2(IPURE) MAPIMETHOD(SpoolerYield) (THIS_ ULONG ulFlags) IPURE; MAPIMETHOD(SpoolerNotify) (THIS_ ULONG ulFlags,LPVOID lpvData) IPURE; MAPIMETHOD(CreateOneOff) (THIS_ LPTSTR lpszName,LPTSTR lpszAdrType,LPTSTR lpszAddress,ULONG ulFlags,ULONG *lpcbEntryID,LPENTRYID *lppEntryID) IPURE; MAPIMETHOD(SetProviderUID) (THIS_ LPMAPIUID lpProviderID,ULONG ulFlags) IPURE; MAPIMETHOD(CompareEntryIDs) (THIS_ ULONG cbEntry1,LPENTRYID lpEntry1,ULONG cbEntry2,LPENTRYID lpEntry2,ULONG ulCompareFlags,ULONG *lpulResult) IPURE; MAPIMETHOD(OpenTemplateID) (THIS_ ULONG cbTemplateID,LPENTRYID lpTemplateID,ULONG ulTemplateFlags,LPMAPIPROP lpMAPIPropData,LPCIID lpInterface,LPMAPIPROP *lppMAPIPropNew,LPMAPIPROP lpMAPIPropSibling) IPURE; MAPIMETHOD(OpenEntry) (THIS_ ULONG cbEntryID,LPENTRYID lpEntryID,LPCIID lpInterface,ULONG ulOpenFlags,ULONG *lpulObjType,LPUNKNOWN *lppUnk) IPURE; MAPIMETHOD(GetOneOffTable) (THIS_ ULONG ulFlags,LPMAPITABLE *lppTable) IPURE; MAPIMETHOD(Address) (THIS_ ULONG *lpulUIParam,LPADRPARM lpAdrParms,LPADRLIST *lppAdrList) IPURE; MAPIMETHOD(Details) (THIS_ ULONG *lpulUIParam,LPFNDISMISS lpfnDismiss,LPVOID lpvDismissContext,ULONG cbEntryID,LPENTRYID lpEntryID,LPFNBUTTON lpfButtonCallback,LPVOID lpvButtonContext,LPTSTR lpszButtonText,ULONG ulFlags) IPURE; MAPIMETHOD(NewEntry) (THIS_ ULONG ulUIParam,ULONG ulFlags,ULONG cbEIDContainer,LPENTRYID lpEIDContainer,ULONG cbEIDNewEntryTpl,LPENTRYID lpEIDNewEntryTpl,ULONG *lpcbEIDNewEntry,LPENTRYID *lppEIDNewEntry) IPURE; MAPIMETHOD(DoConfigPropsheet) (THIS_ ULONG ulUIParam,ULONG ulFlags,LPTSTR lpszTitle,LPMAPITABLE lpDisplayTable,LPMAPIPROP lpCOnfigData,ULONG ulTopPage) IPURE; MAPIMETHOD(CopyMessages) (THIS_ LPCIID lpSrcInterface,LPVOID lpSrcFolder,LPENTRYLIST lpMsgList,LPCIID lpDestInterface,LPVOID lpDestFolder,ULONG ulUIParam,LPMAPIPROGRESS lpProgress,ULONG ulFlags) IPURE; MAPIMETHOD(CopyFolder) (THIS_ LPCIID lpSrcInterface,LPVOID lpSrcFolder,ULONG cbEntryID,LPENTRYID lpEntryID,LPCIID lpDestInterface,LPVOID lpDestFolder,LPTSTR lszNewFolderName,ULONG ulUIParam,LPMAPIPROGRESS lpProgress,ULONG ulFlags) IPURE;
#define MAPI_IMAPISUPPORT_METHODS3(IPURE) MAPIMETHOD(DoCopyTo) (THIS_ LPCIID lpSrcInterface,LPVOID lpSrcObj,ULONG ciidExclude,LPCIID rgiidExclude,LPSPropTagArray lpExcludeProps,ULONG ulUIParam,LPMAPIPROGRESS lpProgress,LPCIID lpDestInterface,LPVOID lpDestObj,ULONG ulFlags,LPSPropProblemArray *lppProblems) IPURE; MAPIMETHOD(DoCopyProps) (THIS_ LPCIID lpSrcInterface,LPVOID lpSrcObj,LPSPropTagArray lpIncludeProps,ULONG ulUIParam,LPMAPIPROGRESS lpProgress,LPCIID lpDestInterface,LPVOID lpDestObj,ULONG ulFlags,LPSPropProblemArray *lppProblems) IPURE; MAPIMETHOD(DoProgressDialog) (THIS_ ULONG ulUIParam,ULONG ulFlags,LPMAPIPROGRESS *lppProgress) IPURE; MAPIMETHOD(ReadReceipt) (THIS_ ULONG ulFlags,LPMESSAGE lpReadMessage,LPMESSAGE *lppEmptyMessage) IPURE; MAPIMETHOD(PrepareSubmit) (THIS_ LPMESSAGE lpMessage,ULONG *lpulFlags) IPURE; MAPIMETHOD(ExpandRecips) (THIS_ LPMESSAGE lpMessage,ULONG *lpulFlags) IPURE; MAPIMETHOD(UpdatePAB) (THIS_ ULONG ulFlags,LPMESSAGE lpMessage) IPURE; MAPIMETHOD(DoSentMail) (THIS_ ULONG ulFlags,LPMESSAGE lpMessage) IPURE; MAPIMETHOD(OpenAddressBook) (THIS_ LPCIID lpInterface,ULONG ulFlags,LPADRBOOK *lppAdrBook) IPURE; MAPIMETHOD(Preprocess) (THIS_ ULONG ulFlags,ULONG cbEntryID,LPENTRYID lpEntryID) IPURE; MAPIMETHOD(CompleteMsg) (THIS_ ULONG ulFlags,ULONG cbEntryID,LPENTRYID lpEntryID) IPURE; MAPIMETHOD(StoreLogoffTransports) (THIS_ ULONG *lpulFlags) IPURE; MAPIMETHOD(StatusRecips) (THIS_ LPMESSAGE lpMessage,LPADRLIST lpRecipList) IPURE; MAPIMETHOD(WrapStoreEntryID) (THIS_ ULONG cbOrigEntry,LPENTRYID lpOrigEntry,ULONG *lpcbWrappedEntry,LPENTRYID *lppWrappedEntry) IPURE; MAPIMETHOD(ModifyProfile) (THIS_ ULONG ulFlags) IPURE; MAPIMETHOD(IStorageFromStream) (THIS_ LPUNKNOWN lpUnkIn,LPCIID lpInterface,ULONG ulFlags,LPSTORAGE *lppStorageOut) IPURE; MAPIMETHOD(GetSvcConfigSupportObj) (THIS_ ULONG ulFlags,LPMAPISUP *lppSvcSupport) IPURE;
#undef INTERFACE
#define INTERFACE IMAPISupport
  DECLARE_MAPI_INTERFACE_(IMAPISupport,IUnknown) {
    BEGIN_INTERFACE
      MAPI_IUNKNOWN_METHODS(PURE)
      MAPI_IMAPISUPPORT_METHODS1(PURE)
      MAPI_IMAPISUPPORT_METHODS2(PURE)
      MAPI_IMAPISUPPORT_METHODS3(PURE)
  };

#define FILL_ENTRY ((ULONG) 0x00000001)

  DECLARE_MAPI_INTERFACE_PTR(IABProvider,LPABPROVIDER);
  DECLARE_MAPI_INTERFACE_PTR(IABLogon,LPABLOGON);

#define MAPI_IABPROVIDER_METHODS(IPURE) MAPIMETHOD(Shutdown) (THIS_ ULONG *lpulFlags) IPURE; MAPIMETHOD(Logon) (THIS_ LPMAPISUP lpMAPISup,ULONG ulUIParam,LPTSTR lpszProfileName,ULONG ulFlags,ULONG *lpulpcbSecurity,LPBYTE *lppbSecurity,LPMAPIERROR *lppMAPIError,LPABLOGON *lppABLogon) IPURE;
#undef INTERFACE
#define INTERFACE IABProvider
  DECLARE_MAPI_INTERFACE_(IABProvider,IUnknown) {
    BEGIN_INTERFACE
      MAPI_IUNKNOWN_METHODS(PURE)
      MAPI_IABPROVIDER_METHODS(PURE)
  };

#define MAPI_IABLOGON_METHODS(IPURE) MAPIMETHOD(GetLastError) (THIS_ HRESULT hResult,ULONG ulFlags,LPMAPIERROR *lppMAPIError) IPURE; MAPIMETHOD(Logoff) (THIS_ ULONG ulFlags) IPURE; MAPIMETHOD(OpenEntry) (THIS_ ULONG cbEntryID,LPENTRYID lpEntryID,LPCIID lpInterface,ULONG ulFlags,ULONG *lpulObjType,LPUNKNOWN *lppUnk) IPURE; MAPIMETHOD(CompareEntryIDs) (THIS_ ULONG cbEntryID1,LPENTRYID lpEntryID1,ULONG cbEntryID2,LPENTRYID lpEntryID2,ULONG ulFlags,ULONG *lpulResult) IPURE; MAPIMETHOD(Advise) (THIS_ ULONG cbEntryID,LPENTRYID lpEntryID,ULONG ulEventMask,LPMAPIADVISESINK lpAdviseSink,ULONG *lpulConnection) IPURE; MAPIMETHOD(Unadvise) (THIS_ ULONG ulConnection) IPURE; MAPIMETHOD(OpenStatusEntry) (THIS_ LPCIID lpInterface,ULONG ulFlags,ULONG *lpulObjType,LPMAPISTATUS *lppEntry) IPURE; MAPIMETHOD(OpenTemplateID) (THIS_ ULONG cbTemplateID,LPENTRYID lpTemplateID,ULONG ulTemplateFlags,LPMAPIPROP lpMAPIPropData,LPCIID lpInterface,LPMAPIPROP *lppMAPIPropNew,LPMAPIPROP lpMAPIPropSibling) IPURE; MAPIMETHOD(GetOneOffTable) (THIS_ ULONG ulFlags,LPMAPITABLE *lppTable) IPURE; MAPIMETHOD(PrepareRecips) (THIS_ ULONG ulFlags,LPSPropTagArray lpPropTagArray,LPADRLIST lpRecipList) IPURE;
#undef INTERFACE
#define INTERFACE IABLogon
  DECLARE_MAPI_INTERFACE_(IABLogon,IUnknown) {
    BEGIN_INTERFACE
      MAPI_IUNKNOWN_METHODS(PURE)
      MAPI_IABLOGON_METHODS(PURE)
  };

  typedef HRESULT (__cdecl ABPROVIDERINIT)(HINSTANCE hInstance,LPMALLOC lpMalloc,LPALLOCATEBUFFER lpAllocateBuffer,LPALLOCATEMORE lpAllocateMore,LPFREEBUFFER lpFreeBuffer,ULONG ulFlags,ULONG ulMAPIVer,ULONG *lpulProviderVer,LPABPROVIDER *lppABProvider);

  ABPROVIDERINIT ABProviderInit;

#define DEINIT_NORMAL ((ULONG) 0x00000001)
#define DEINIT_HURRY ((ULONG) 0x80000000)

#define LOGON_NO_DIALOG ((ULONG) 0x00000001)
#define LOGON_NO_CONNECT ((ULONG) 0x00000004)
#define LOGON_NO_INBOUND ((ULONG) 0x00000008)
#define LOGON_NO_OUTBOUND ((ULONG) 0x00000010)

#define LOGON_SP_IDLE ((ULONG) 0x00010000)
#define LOGON_SP_POLL ((ULONG) 0x00020000)
#define LOGON_SP_RESOLVE ((ULONG) 0x00040000)

  DECLARE_MAPI_INTERFACE_PTR(IXPProvider,LPXPPROVIDER);
  DECLARE_MAPI_INTERFACE_PTR(IXPLogon,LPXPLOGON);

#define MAPI_IXPPROVIDER_METHODS(IPURE) MAPIMETHOD(Shutdown) (THIS_ ULONG *lpulFlags) IPURE; MAPIMETHOD(TransportLogon) (THIS_ LPMAPISUP lpMAPISup,ULONG ulUIParam,LPTSTR lpszProfileName,ULONG *lpulFlags,LPMAPIERROR *lppMAPIError,LPXPLOGON *lppXPLogon) IPURE;
#undef INTERFACE
#define INTERFACE IXPProvider
  DECLARE_MAPI_INTERFACE_(IXPProvider,IUnknown) {
    BEGIN_INTERFACE
      MAPI_IUNKNOWN_METHODS(PURE)
      MAPI_IXPPROVIDER_METHODS(PURE)
  };

#define OPTION_TYPE_RECIPIENT ((ULONG) 0x00000001)
#define OPTION_TYPE_MESSAGE ((ULONG) 0x00000002)

  typedef struct _OPTIONDATA {
    ULONG ulFlags;
    LPGUID lpRecipGUID;
    LPTSTR lpszAdrType;
    LPTSTR lpszDLLName;
    ULONG ulOrdinal;
    ULONG cbOptionsData;
    LPBYTE lpbOptionsData;
    ULONG cOptionsProps;
    LPSPropValue lpOptionsProps;
  } OPTIONDATA,*LPOPTIONDATA;

  typedef SCODE (__cdecl OPTIONCALLBACK)(HINSTANCE hInst,LPMALLOC lpMalloc,ULONG ulFlags,ULONG cbOptionData,LPBYTE lpbOptionData,LPMAPISUP lpMAPISup,LPMAPIPROP lpDataSource,LPMAPIPROP *lppWrappedSource,LPMAPIERROR *lppMAPIError);

#define NOTIFY_ABORT_DEFERRED ((ULONG) 0x40000000)
#define NOTIFY_CANCEL_MESSAGE ((ULONG) 0x80000000)
#define NOTIFY_BEGIN_INBOUND ((ULONG) 0x00000001)
#define NOTIFY_END_INBOUND ((ULONG) 0x00010000)
#define NOTIFY_BEGIN_OUTBOUND ((ULONG) 0x00000002)
#define NOTIFY_END_OUTBOUND ((ULONG) 0x00020000)
#define NOTIFY_BEGIN_INBOUND_FLUSH ((ULONG) 0x00000004)
#define NOTIFY_END_INBOUND_FLUSH ((ULONG) 0x00040000)
#define NOTIFY_BEGIN_OUTBOUND_FLUSH ((ULONG) 0x00000008)
#define NOTIFY_END_OUTBOUND_FLUSH ((ULONG) 0x00080000)

#define LOGOFF_NORMAL ((ULONG) 0x00000001)
#define LOGOFF_HURRY ((ULONG) 0x80000000)

#define BEGIN_DEFERRED ((ULONG) 0x00000001)

#define END_RESEND_NOW ((ULONG) 0x00010000)
#define END_RESEND_LATER ((ULONG) 0x00020000)
#define END_DONT_RESEND ((ULONG) 0x00040000)

#define MAPI_IXPLOGON_METHODS(IPURE) MAPIMETHOD(AddressTypes) (THIS_ ULONG *lpulFlags,ULONG *lpcAdrType,LPTSTR **lpppAdrTypeArray,ULONG *lpcMAPIUID,LPMAPIUID **lpppUIDArray) IPURE; MAPIMETHOD(RegisterOptions) (THIS_ ULONG *lpulFlags,ULONG *lpcOptions,LPOPTIONDATA *lppOptions) IPURE; MAPIMETHOD(TransportNotify) (THIS_ ULONG *lpulFlags,LPVOID *lppvData) IPURE; MAPIMETHOD(Idle) (THIS_ ULONG ulFlags) IPURE; MAPIMETHOD(TransportLogoff) (THIS_ ULONG ulFlags) IPURE; MAPIMETHOD(SubmitMessage) (THIS_ ULONG ulFlags,LPMESSAGE lpMessage,ULONG *lpulMsgRef,ULONG *lpulReturnParm) IPURE; MAPIMETHOD(EndMessage) (THIS_ ULONG ulMsgRef,ULONG *lpulFlags) IPURE; MAPIMETHOD(Poll) (THIS_ ULONG *lpulIncoming) IPURE; MAPIMETHOD(StartMessage) (THIS_ ULONG ulFlags,LPMESSAGE lpMessage,ULONG *lpulMsgRef) IPURE; MAPIMETHOD(OpenStatusEntry) (THIS_ LPCIID lpInterface,ULONG ulFlags,ULONG *lpulObjType,LPMAPISTATUS *lppEntry) IPURE; MAPIMETHOD(ValidateState) (THIS_ ULONG ulUIParam,ULONG ulFlags) IPURE; MAPIMETHOD(FlushQueues) (THIS_ ULONG ulUIParam,ULONG cbTargetTransport,LPENTRYID lpTargetTransport,ULONG ulFlags) IPURE;
#undef INTERFACE
#define INTERFACE IXPLogon
  DECLARE_MAPI_INTERFACE_(IXPLogon,IUnknown) {
    BEGIN_INTERFACE
      MAPI_IUNKNOWN_METHODS(PURE)
      MAPI_IXPLOGON_METHODS(PURE)
  };

  typedef HRESULT (__cdecl XPPROVIDERINIT)(HINSTANCE hInstance,LPMALLOC lpMalloc,LPALLOCATEBUFFER lpAllocateBuffer,LPALLOCATEMORE lpAllocateMore,LPFREEBUFFER lpFreeBuffer,ULONG ulFlags,ULONG ulMAPIVer,ULONG *lpulProviderVer,LPXPPROVIDER *lppXPProvider);

  XPPROVIDERINIT XPProviderInit;

#define LOGON_SP_TRANSPORT ((ULONG) 0x00000001)
#define LOGON_SP_PROMPT ((ULONG) 0x00000002)
#define LOGON_SP_NEWPW ((ULONG) 0x00000004)
#define LOGON_CHANGED ((ULONG) 0x00000008)

#define DIALOG_FOLDER ((ULONG) 0x00000001)
#define DIALOG_MESSAGE ((ULONG) 0x00000002)
#define DIALOG_PROP ((ULONG) 0x00000004)
#define DIALOG_ATTACH ((ULONG) 0x00000008)

#define DIALOG_MOVE ((ULONG) 0x00000010)
#define DIALOG_COPY ((ULONG) 0x00000020)
#define DIALOG_DELETE ((ULONG) 0x00000040)

#define DIALOG_ALLOW_CANCEL ((ULONG) 0x00000080)
#define DIALOG_CONFIRM_CANCEL ((ULONG) 0x00000100)

#define NEEDS_PREPROCESSING ((ULONG) 0x00000001)
#define NEEDS_SPOOLER ((ULONG) 0x00000002)

#define CHECK_SENDER ((ULONG) 0x00000001)
#define NON_STANDARD ((ULONG) 0x00010000)

  DECLARE_MAPI_INTERFACE_PTR(IMSLogon,LPMSLOGON);
  DECLARE_MAPI_INTERFACE_PTR(IMSProvider,LPMSPROVIDER);

#define MAPI_IMSPROVIDER_METHODS(IPURE) MAPIMETHOD(Shutdown) (THIS_ ULONG *lpulFlags) IPURE; MAPIMETHOD(Logon) (THIS_ LPMAPISUP lpMAPISup,ULONG ulUIParam,LPTSTR lpszProfileName,ULONG cbEntryID,LPENTRYID lpEntryID,ULONG ulFlags,LPCIID lpInterface,ULONG *lpcbSpoolSecurity,LPBYTE *lppbSpoolSecurity,LPMAPIERROR *lppMAPIError,LPMSLOGON *lppMSLogon,LPMDB *lppMDB) IPURE; MAPIMETHOD(SpoolerLogon) (THIS_ LPMAPISUP lpMAPISup,ULONG ulUIParam,LPTSTR lpszProfileName,ULONG cbEntryID,LPENTRYID lpEntryID,ULONG ulFlags,LPCIID lpInterface,ULONG cbSpoolSecurity,LPBYTE lpbSpoolSecurity,LPMAPIERROR *lppMAPIError,LPMSLOGON *lppMSLogon,LPMDB *lppMDB) IPURE; MAPIMETHOD(CompareStoreIDs) (THIS_ ULONG cbEntryID1,LPENTRYID lpEntryID1,ULONG cbEntryID2,LPENTRYID lpEntryID2,ULONG ulFlags,ULONG *lpulResult) IPURE;
#undef INTERFACE
#define INTERFACE IMSProvider
  DECLARE_MAPI_INTERFACE_(IMSProvider,IUnknown) {
    BEGIN_INTERFACE
      MAPI_IUNKNOWN_METHODS(PURE)
      MAPI_IMSPROVIDER_METHODS(PURE)
  };

#define MAPI_IMSLOGON_METHODS(IPURE) MAPIMETHOD(GetLastError) (THIS_ HRESULT hResult,ULONG ulFlags,LPMAPIERROR *lppMAPIError) IPURE; MAPIMETHOD(Logoff) (THIS_ ULONG *lpulFlags) IPURE; MAPIMETHOD(OpenEntry) (THIS_ ULONG cbEntryID,LPENTRYID lpEntryID,LPCIID lpInterface,ULONG ulFlags,ULONG *lpulObjType,LPUNKNOWN *lppUnk) IPURE; MAPIMETHOD(CompareEntryIDs) (THIS_ ULONG cbEntryID1,LPENTRYID lpEntryID1,ULONG cbEntryID2,LPENTRYID lpEntryID2,ULONG ulFlags,ULONG *lpulResult) IPURE; MAPIMETHOD(Advise) (THIS_ ULONG cbEntryID,LPENTRYID lpEntryID,ULONG ulEventMask,LPMAPIADVISESINK lpAdviseSink,ULONG *lpulConnection) IPURE; MAPIMETHOD(Unadvise) (THIS_ ULONG ulConnection) IPURE; MAPIMETHOD(OpenStatusEntry) (THIS_ LPCIID lpInterface,ULONG ulFlags,ULONG *lpulObjType,LPVOID *lppEntry) IPURE;
#undef INTERFACE
#define INTERFACE IMSLogon
  DECLARE_MAPI_INTERFACE_(IMSLogon,IUnknown) {
    BEGIN_INTERFACE
      MAPI_IUNKNOWN_METHODS(PURE)
      MAPI_IMSLOGON_METHODS(PURE)
  };

  typedef HRESULT (__cdecl MSPROVIDERINIT)(HINSTANCE hInstance,LPMALLOC lpMalloc,LPALLOCATEBUFFER lpAllocateBuffer,LPALLOCATEMORE lpAllocateMore,LPFREEBUFFER lpFreeBuffer,ULONG ulFlags,ULONG ulMAPIVer,ULONG *lpulProviderVer,LPMSPROVIDER *lppMSProvider);

  MSPROVIDERINIT MSProviderInit;

#define MSG_SERVICE_UI_READ_ONLY 0x00000008
#define SERVICE_LOGON_FAILED 0x00000020

#define MSG_SERVICE_INSTALL 0x00000001
#define MSG_SERVICE_CREATE 0x00000002
#define MSG_SERVICE_CONFIGURE 0x00000003
#define MSG_SERVICE_DELETE 0x00000004
#define MSG_SERVICE_UNINSTALL 0x00000005
#define MSG_SERVICE_PROVIDER_CREATE 0x00000006
#define MSG_SERVICE_PROVIDER_DELETE 0x00000007

  typedef HRESULT (WINAPI MSGSERVICEENTRY)(HINSTANCE hInstance,LPMALLOC lpMalloc,LPMAPISUP lpMAPISup,ULONG ulUIParam,ULONG ulFlags,ULONG ulContext,ULONG cValues,LPSPropValue lpProps,LPPROVIDERADMIN lpProviderAdmin,LPMAPIERROR *lppMapiError);
  typedef MSGSERVICEENTRY *LPMSGSERVICEENTRY;

#ifdef __cplusplus
}
#endif
#endif
