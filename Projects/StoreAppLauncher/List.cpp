#include "List.h"

List::List( )
{
	m_hsActivateClassId.Set( RuntimeClass_Windows_Management_Deployment_PackageManager );
}
void List::InitializePackageManager( )
{
	ComPtr<IActivationFactory> activationFactory;
	RoGetActivationFactory( m_hsActivateClassId.Get( ), __uuidof( IActivationFactory ), (void**)&activationFactory );

	ComPtr<IInspectable> iinsp;
	activationFactory->ActivateInstance( &iinsp );

	iinsp.As( &m_pPackageManager );
}

void List::PopulateArray( )
{
	m_pPackageManager->FindPackagesWithPackageTypes( PackageTypes::PackageTypes_Main, &m_pPackages );

	ComPtr<Collections::IIterator<Package*>> pIter;
	m_pPackages->First( &pIter );

	ComPtr<IStorageFolder> storageFolder;
	ComPtr<IStorageItem> storageItem;
	ComPtr<IPackage> ipkg;
	ComPtr<IPackageId> ipkgid;

	boolean hasCurrent;
	try
	{
		for( pIter->get_HasCurrent( &hasCurrent ); hasCurrent; pIter->MoveNext( &hasCurrent ) )
		{
			if( SUCCEEDED( pIter->get_Current( &ipkg ) ) )
			{
				THROW_ON_ERR( ipkg->get_Id( &ipkgid ) );
				if( SUCCEEDED( ipkg->get_InstalledLocation( storageFolder.GetAddressOf( ) ) ) )
				{
					if( storageFolder )
					{
						storageFolder.As( &storageItem );
						wstring temp0, temp1;
						HString name;
						HString location;
						THROW_ON_ERR( ipkgid->get_FamilyName( name.GetAddressOf( ) ) );
						THROW_ON_ERR( storageItem->get_Path( location.GetAddressOf( ) ) );

						temp0 = name.GetRawBuffer( 0 );
						temp1 = location.GetRawBuffer( 0 );
						temp0 += L"!";
						temp1 += L"\\";

						m_mApps[ temp0.c_str( ) ] = temp1.c_str( );
					}
				}
			}
		}
	} 
	catch( std::exception& ex )
	{
		wcout << ex.what( ) << endl;
	}
}

void List::Filter( wstring* blacklist, uint size )
{
	bool erase = false;
	map<wstring, wstring>::iterator it = m_mApps.begin( );
	while( it != m_mApps.end( ) )
	{
		for( uint i = 0; i < size; i++ )
		{
			if( it->first.find( blacklist[ i ] ) != wstring::npos )
			{
				erase = true;
				break;
			}
		}
		if( erase )
		{
			it = m_mApps.erase( it );
			erase = false;
		}
		else
		{
			++it;
		}
	}
}

void List::FilterApps( bool filter )
{
	if( filter )
	{
		wstring blacklist[ ] = {
			L"Microsoft.3DBuilder",
			L"Microsoft.AAD.BrokerPlugin",
			L"Microsoft.AccountsControl",
			L"Microsoft.Appconnector",
			L"Microsoft.Bing",
			L"Microsoft.BioEnrollment",
			L"Microsoft.CommsPhone",
			L"Microsoft.ConnectivityStore",
			L"Microsoft.FreshPaint",
			L"Microsoft.Getstarted",
			L"Microsoft.LockApp",
			L"Microsoft.Messaging",
			L"Microsoft.MicrosoftEdge",
			L"Microsoft.MicrosoftOfficeHub",
			L"Microsoft.NET",
			L"Microsoft.Office",
			L"Microsoft.People",
			L"Microsoft.SkypeApp",
			L"Microsoft.VCLibs",
			L"Microsoft.WinJS",
			L"Microsoft.Windows",
			L"microsoft.windows",
			L"Microsoft.Xbox",
			L"Microsoft.Zune",
			L"Microsoft.StorePurchaseApp",
			L"Microsoft.PPIProjection",
			L"Microsoft.OneConnect",
			L"e2644aa9-08a7-4e35-bf7e-2dd2520e09bd",
			L"Microsoft.MicrosoftStickyNotes",
			L"Microsoft.DesktopAppInstaller",
			L"BetaFish",
			L"Windows.",
			L"windows.",
			L"microsoft.",
			L"ASUSWelcome"
		};
		Filter( blacklist, ARRAYSIZE( blacklist ) );
	}
	else
	{
		wstring blacklist[ ] = {
			L"Microsoft.AAD.BrokerPlugin",
			L"Microsoft.AccountsControl",
			L"Microsoft.Appconnector",
			L"Microsoft.BioEnrollment",
			L"Microsoft.CommsPhone",
			L"Microsoft.ConnectivityStore",
			L"Microsoft.FreshPaint",
			L"Microsoft.Getstarted",
			L"Microsoft.LockApp",
			L"Microsoft.MicrosoftOfficeHub",
			L"Microsoft.NET",
			L"Microsoft.VCLibs",
			L"Microsoft.WinJS",
			L"Microsoft.Windows",
			L"microsoft.windows",
			L"Microsoft.Xbox",
			L"Microsoft.StorePurchaseApp",
			L"Microsoft.PPIProjection",
			L"Microsoft.OneConnect",
			L"e2644aa9-08a7-4e35-bf7e-2dd2520e09bd",
			L"Microsoft.DesktopAppInstaller",
			L"BetaFish"
		};
		Filter( blacklist, ARRAYSIZE( blacklist ) );
	}
}

void List::ParseAppManifest( )
{
	tinyxml2::XMLDocument doc;
	XMLError xmlError;
	wstring manifestFile;

	map<wstring, wstring>::iterator it = m_mApps.begin( );
	while( it != m_mApps.end( ) )
	{
		manifestFile = it->second.c_str( );
		manifestFile += L"AppxManifest.xml";

		xmlError = doc.LoadFile( Tools::Instance( )->WideByteToByte( manifestFile ).c_str( ) );
		XMLCheckResult( xmlError );

		if( xmlError == XML_SUCCESS )
		{
			XMLNode* pRoot = doc.FirstChildElement( "Package" );
			if( pRoot == nullptr )
			{
				wcout << L"xmlError: Trouble with getting a root node" << endl;
				break;
			}
			else
			{
				string displayName;
				XMLElement* pElementDisplayName = pRoot->FirstChildElement( "Properties" )->FirstChildElement( "DisplayName" );
				if( pElementDisplayName == nullptr )
				{
					wcout << L"xmlError: yadayada" << endl;
					break;
				}
				else
				{
					string filter[] =
					{
						"ms-resource",
						"AppTitle",
						"App_DisplayName"
					};

					displayName = pElementDisplayName->GetText( );
					bool replace = false;
					for( uint i = 0; i < ARRAYSIZE( filter ); i++ )
					{
						if( displayName.find( filter[ i ] ) != string::npos )
						{
							replace = true;
							break;
						}
					}
					if( replace )
					{
						XMLElement* pReplacement = pRoot->FirstChildElement( "Applications" )->FirstChildElement( "Application" );
						if( pReplacement != nullptr )
						{
							displayName = pReplacement->Attribute( "Executable" );
						}
						else
						{
							displayName = "Unidentified App";
						}
					}
				}
				XMLElement* pElement = pRoot->FirstChildElement( "Applications" )->FirstChildElement( "Application" );
				if( pElement == nullptr )
				{
					wcout << L"xmlError: Trouble with parsing first element of a root node" << endl;
					break;
				}
				else
				{
					string id;
					id = pElement->Attribute( "Id" );

					wstring wId = Tools::Instance( )->ByteToWideByte( id );
					wstring wName = Tools::Instance( )->ByteToWideByte( displayName );
					m_mFinalAppMap[ wName ] = it->first + wId;
				}
			}
		}
		else
		{
			wcout << L"xmlError: " << xmlError << endl;
			break;
		}
		doc.Clear( );

		++it;
	}
}

void List::ShowAppList( bool filter )
{
	InitializePackageManager( );
	PopulateArray( );
	FilterApps( filter );
	ParseAppManifest( );

	std::wofstream file;
	file.open( "Applications.txt" );
	wcout << L"wtest" << endl;
	for( map<wstring, wstring>::iterator it = m_mFinalAppMap.begin( ); it != m_mFinalAppMap.end( ); ++it )
	{
		wstring out = L"TITLE: ";
		out += it->first.c_str( );
		out += ( L"\nID: " );
		out += it->second.c_str( );

		m_FinalIdVec.push_back( it->second.c_str( ) );

		out += L"\n-----------------------------------------------------------\n";
		file << out.c_str( );
	}

	file.close( );
}
vector<wstring> List::FinalIdVec( )
{
	return m_FinalIdVec;
}
void List::Launch( bool filter )
{
	ShowAppList( filter );
	_tsystem( _T( "explorer.exe ""Applications.txt"" " ) );
	Shutdown( );
}

void List::Shutdown( )
{
	m_pPackages.Reset( );
	m_pPackageManager.Reset( );
}
