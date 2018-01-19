//ACC v1.5//

//game.h ----------------------------------------------------
//Declare public variables in CGame class++++++++++++++++++++++++++++++++

  std::string					classCallString;				  // Auto Class Call String
  std::string					CC[12];							  // Auto Class Call Player Slot Call
  std::string					CCstatic[12];					  // Auto Class Call Static Slot Number
  bool							CCStarted;						  // Auto Class Call has been started or not
	
//Declare protected variables in CGame class++++++++++++++++++++++++++++++++

  uint32_t						 m_GameLoadedTime;				  // GetTime when the game was loaded
	
//game_base.cpp----------------------------------------------------
//Initialize variables in CGame constructor++++++++++++++++++++++++++++++++

m_GameLoadedTime = 0;
CCStarted = true; //set to false to be deactivated by default
CCstatic[0] = "1";
CCstatic[1] = "2";
CCstatic[2] = "3";
CCstatic[3] = "4";
CCstatic[4] = "5";
CCstatic[5] = "6";
CCstatic[6] = "7";
CCstatic[7] = "8";
CCstatic[8] = "9";

//in EventGameLoaded after "CONSOLE_Print"++++++++++++++++++++++++++++++++
m_GameLoadedTime = GetTime( );

//end of EventPlayerDeleted++++++++++++++++++++++++++++++++

if (CCStarted && m_GameLoaded)
{
     float iTime = (float)((GetTime() - m_GameLoadedTime)/60);
     string sTime = UTIL_ToString( iTime,0 ) + "m";
     int myDrop = GetSIDFromPID( player->GetPID( ) );
     SendAllChat("Warning: Your "+CC[myDrop]+" just dropped!");
     CC[myDrop] = CC[myDrop] + " dropped: " + sTime;
     classCallString = CC[0]+"/"+CC[1]+"/"+CC[2]+"/"+CC[3]+"/"+CC[4]+"/"+CC[5]+"/"+CC[6]+"/"+CC[7]+"/"+CC[8];
}
else if (CCStarted && !m_GameLoaded)
{
     for( unsigned int i = 0; i < 9; i++ )
     {
         if( m_Slots[i].GetSlotStatus() != 2 )
         {
             CC[i] = "-";
          }
          else if (CC[i].length() <= 1)
          {
             CC[i] = CCstatic[i];
          }
      }
      classCallString = CC[0]+"/"+CC[1]+"/"+CC[2]+"/"+CC[3]+"/"+CC[4]+"/"+CC[5]+"/"+CC[6]+"/"+CC[7]+"/"+CC[8];
      SendAllChat(classCallString);
}

//in EventPlayerChatToHost++++++++++++++++++++++++++++++++
//after comment "calculate timestamp"

if (CCStarted && !m_GameLoaded)
{
	string myMsg = chatPlayer->GetMessage( );
	int firstChar = (int)(myMsg[0]);
	int secondChar = (int)(myMsg[1]);
	bool firstDigit = false;
	bool secondDigit = false;
	if (firstChar > 48 && firstChar < 58)
		firstDigit = true;
	if (secondChar > 47 && secondChar < 58)
		secondDigit = true;
	if ( firstDigit && !secondDigit )
	{
		for( unsigned int i = 0; i < 9; i++ )
		{
			if( m_Slots[i].GetSlotStatus() != 2 )
			{
				CC[i] = "-";
			}
			else if (CC[i].length() <= 1)
			{
				CC[i] = CCstatic[i];
			}
		}
		unsigned char myColour = m_Slots[GetSIDFromPID ( player->GetPID( ) )].GetColour( );
		int currentChar = int(myMsg[0]);
		while (currentChar < 65 && !myMsg.empty())
		{
			myMsg = myMsg.erase(0,1);
			currentChar = int(myMsg[0]);
		}
		int requestedSlot = firstChar - 49;
		if ( requestedSlot == myColour )
		{
			if (myMsg.size() > 1)
			{
				  std::string::iterator end_pos = std::remove(myMsg.begin(), myMsg.end(), '/');
				  myMsg.erase(end_pos, myMsg.end());
				  if (myMsg.size() > 12)
				  {
					  myMsg.resize(12);
				  }
				  CC[myColour] = myMsg;
			}
			else
			{
				CC[myColour] = CCstatic[myColour];
			}
			classCallString = CC[0]+"/"+CC[1]+"/"+CC[2]+"/"+CC[3]+"/"+CC[4]+"/"+CC[5]+"/"+CC[6]+"/"+CC[7]+"/"+CC[8];
			SendAllChat(classCallString);
		}
	}
}

//in EventPlayerChangeTeam++++++++++++++++++++++++++++++++
//after SwapSlots( oldSID, newSID );	

		if( oldSID < m_Slots.size( ) && newSID < m_Slots.size( ) && oldSID != newSID )
		{
			string temp = CC[oldSID];
			CC[oldSID] = CC[newSID];
			CC[newSID] = temp;
		}
		classCallString = CC[0]+"/"+CC[1]+"/"+CC[2]+"/"+CC[3]+"/"+CC[4]+"/"+CC[5]+"/"+CC[6]+"/"+CC[7]+"/"+CC[8];

	
//end of EventPlayerJoined++++++++++++++++++++++++++++++++

if (CCStarted && !m_GameLoaded)
{
      for( unsigned int i = 0; i < 9; i++ )
      {
           if( m_Slots[i].GetSlotStatus() != 2 )
           {
                CC[i] = "-";
           }
           else if (CC[i].length() <= 1)
           {
                CC[i] = CCstatic[i];
           }
      }
      classCallString = CC[0]+"/"+CC[1]+"/"+CC[2]+"/"+CC[3]+"/"+CC[4]+"/"+CC[5]+"/"+CC[6]+"/"+CC[7]+"/"+CC[8];
      SendAllChat(classCallString);
}

//in game.cpp----------------------------------------------------
//at end of "!SWAP (swap slots)"++++++++++++++++++++++++++++++++

else																												//already in code
{																													//already in code
	SwapSlots( (unsigned char)( SID1 - 1 ), (unsigned char)( SID2 - 1 ) );		//already in code
	if( SID1 < m_Slots.size( ) && SID2 < m_Slots.size( ) && SID1 != SID2 )
	{
		string temp = CC[(int)(SID1 - 1)];
		CC[(int)(SID1 - 1)] = CC[(int)(SID2 - 1)];
		CC[(int)(SID2 - 1)] = temp;
	}
        classCallString = CC[0]+"/"+CC[1]+"/"+CC[2]+"/"+CC[3]+"/"+CC[4]+"/"+CC[5]+"/"+CC[6]+"/"+CC[7]+"/"+CC[8];
}																													//already in code

//after comment "ADMIN COMMANDS")"++++++++++++++++++++++++++++++++
		//
		// !STARTCC
		//

		case HashCode("startCC"):
		case HashCode("startcc"):
		{
			if (!CCStarted && !m_GameLoaded)
			{
				CCStarted = true;
				SendAllChat("Automated Class Caller Activated");
			}
		}

		//
		// !STOPCC
		//

		case HashCode("stopCC"):
		case HashCode("stopcc"):
		{
			if (CCStarted && !m_GameLoaded)
			{
				CCStarted = false;
				SendAllChat("Automated Class Caller Deactivated");
			}
		}

//after comment "NON ADMIN COMMANDS"")"++++++++++++++++++++++++++++++++

	//
	// !CC
	//

	case HashCode("cc"):
	case HashCode("CC"):
	{
		if (m_GameLoaded)
			SendAllChat(classCallString);
		else if (CCStarted && !m_GameLoaded)
		{
			for (unsigned int i = 0; i < 9; i++)
			{
				if (m_Slots[i].GetSlotStatus() != 2)
					CC[i] = "-";
				else if (CC[i].length() <= 1)
					CC[i] = CCstatic[i];
			}
			classCallString = CC[0] + "/" + CC[1] + "/" + CC[2] + "/" + CC[3] + "/" + CC[4] + "/" + CC[5] + "/" + CC[6] + "/" + CC[7] + "/" + CC[8];
			SendAllChat(classCallString);
		}
	}