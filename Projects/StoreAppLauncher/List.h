#pragma once
#include "Tools.h"

class List
{
public:
	List( );
	~List( ) { };
private:
	map<wstring, wstring> m_mApps;
	map<wstring, wstring> m_mFinalAppMap;
	vector<wstring> m_FinalIdVec;
	HString m_hsActivateClassId;
	ComPtr<IPackageManager2> m_pPackageManager;
	ComPtr<Collections::IIterable<Package*>> m_pPackages;
private:
	void InitializePackageManager( );
	void PopulateArray( );
	void Filter( wstring*, uint );
	void FilterApps( bool );
	void ParseAppManifest( );
	void ShowAppList( bool );
public:
	vector<wstring> FinalIdVec( );
	void Launch( bool filter = 0 );
	void Shutdown( );
};