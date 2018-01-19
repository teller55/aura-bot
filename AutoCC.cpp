//ACC v1.5//

//File: game_base.h ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Declare public variables in CBaseGame class++++++++++++++++++++++++++++++++

  string							classCallString;					  // Auto Class Call String
  string							CC[12];							  // Auto Class Call Player Slot Call
  string							CCstatic[12];					  // Auto Class Call Static Slot Number
  bool							CCStarted;						  // Auto Class Call has been started or not
  string							m_leader;							  // Leader of the game
  
  
 //Declare protected variables in CBaseGame class++++++++++++++++++++++++++++++++
 
  uint32_t						m_GameLoadedTime;		  // GetTime when the game was loaded

 
//After "// other functions" after IsOwner++++++++++++++++++++++++++++++++++++
  virtual bool IsLeader( string name );
  
  
  
//File: game_base.cpp~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Initialize variables in CBaseGame constructor++++++++++++++++++++++++++++++++

    // initialize Auto Class Call variables
	m_GameLoadedTime = 0;
	CCStarted = false;		//set to true to be activated by default
	CCstatic[0] = "1";
	CCstatic[1] = "2";
	CCstatic[2] = "3";
	CCstatic[3] = "4";
	CCstatic[4] = "5";
	CCstatic[5] = "6";
	CCstatic[6] = "7";
	CCstatic[7] = "8";
	CCstatic[8] = "9";
	

//in EventGameLoaded after the first line "CONSOLE_Print"++++++++++++++++++++++
m_GameLoadedTime = GetTime( );


//end of EventPlayerDeleted++++++++++++++++++++++++++++++++++++++++++++

	// reset leader if they leave
	if (IsLeader(player->GetName())) {
		SendAllChat("Your leader [" + m_leader + "] has left the game. You may select a new leader.");
		m_leader.clear();
	}

    // update ACC when a player leaves
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
	
	
//in EventPlayerChatToHost after comment '// calculate timestamp'++++++++++++++++

	//Auto Class Call - Call Class
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
			if (requestedSlot == myColour || (IsLeader(player->GetName()) && m_Slots[requestedSlot].GetSlotStatus() == 2) )
			{
				if (myMsg.size() > 1)
				{
					std::string::iterator end_pos = std::remove(myMsg.begin(), myMsg.end(), '/');
					myMsg.erase(end_pos, myMsg.end());
					if (myMsg.size() > 15)
					{
						myMsg.resize(15);
					}
					CC[requestedSlot] = myMsg;
				}
				else
				{
					CC[requestedSlot] = CCstatic[requestedSlot];
				}
				classCallString = CC[0]+"/"+CC[1]+"/"+CC[2]+"/"+CC[3]+"/"+CC[4]+"/"+CC[5]+"/"+CC[6]+"/"+CC[7]+"/"+CC[8];
				SendAllChat(classCallString);
			}
		}
	}
	
	
//in EventPlayerChangeTeam after 'SwapSlots( oldSID, newSID );'++++++++++++++++++++++++++++++++++++++

		if( oldSID < m_Slots.size( ) && newSID < m_Slots.size( ) && oldSID != newSID )
		{
			string temp = CC[oldSID];
			CC[oldSID] = CC[newSID];
			CC[newSID] = temp;
		}
		classCallString = CC[0]+"/"+CC[1]+"/"+CC[2]+"/"+CC[3]+"/"+CC[4]+"/"+CC[5]+"/"+CC[6]+"/"+CC[7]+"/"+CC[8];

		
//end of EventPlayerJoined+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		

	// display Auto Class Call when a player joins
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
	
	
//After bool CBaseGame :: IsOwner( string name ) function definition++++++++++++++++++++++++++++++++++++
bool CBaseGame :: IsLeader( string name )
{
	string LeaderLower = m_leader;
	transform( name.begin( ), name.end( ), name.begin( ), (int(*)(int))tolower );
	transform( LeaderLower.begin( ), LeaderLower.end( ), LeaderLower.begin( ), (int(*)(int))tolower );
	return name == LeaderLower;
}



//File: game.cpp~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//at end of !SWAP (swap slots)+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		SwapSlots( (unsigned char)( SID1 - 1 ), (unsigned char)( SID2 - 1 ) );					//after this line already in the code
		if( SID1 < m_Slots.size( ) && SID2 < m_Slots.size( ) && SID1 != SID2 )
		{
			string temp = CC[(int)(SID1 - 1)];
			CC[(int)(SID1 - 1)] = CC[(int)(SID2 - 1)];
			CC[(int)(SID2 - 1)] = temp;
		}
			classCallString = CC[0]+"/"+CC[1]+"/"+CC[2]+"/"+CC[3]+"/"+CC[4]+"/"+CC[5]+"/"+CC[6]+"/"+CC[7]+"/"+CC[8];
	

//before comment "ADMIN COMMANDS"+++++++++++++++++++++++++++++++

	//modify the If statements to allow Leaders to use admin Commands
	if( player->GetSpoofed( ) && ( AdminCheck || RootAdminCheck || IsOwner( User ) || IsLeader( User ) ) )  //added IsLeader
	{
		CONSOLE_Print( "[GAME: " + m_GameName + "] admin [" + User + "] sent Command [" + Command + "] with payload [" + Payload + "]" );

			if( !m_Locked || RootAdminCheck || IsOwner( User ) || IsLeader( User ) )		//added IsLeader
			
			
//Add 3 Admin Commands in "ADMIN COMMANDS" section+++++++++++++++++++++++++++++++++++++++

			//
			// !startCC
			//
			if ((Command =="startCC" || Command == "startcc") && !CCStarted && !m_GameLoaded)
			{
				  CCStarted = true;
				  SendAllChat("Automated Class Caller Activated");
			}

			//
			// !stopCC
			//
			if ((Command =="stopCC" || Command == "stopcc") && CCStarted && !m_GameLoaded)
			{
				  CCStarted = false;
				  SendAllChat("Automated Class Caller Deactivated");
			}

			//
			// !LEADER
			//

			if( Command == "leader" )
			{
				if (Payload.empty() && m_leader.empty())
				{
					SendAllChat("A leader has not yet been designated.");
				} else if(Payload.empty()) {
					SendAllChat("Your leader is: [" + m_leader + "].");
				} else {
					CGamePlayer*    LastMatch = NULL;
					uint32_t        Matches = GetPlayerFromNamePartial(Payload, &LastMatch);

					if (m_leader.empty() || IsLeader(User) || RootAdminCheck) {
						if (Matches == 0)
							SendAllChat("Unable to set leader to [" + Payload + "]. No matches found.");
						else if (Matches == 1) {
							m_leader = LastMatch->GetName();
							SendAllChat("Your leader is now: [" + m_leader + "].");
						} else 
							SendAllChat("Unable to set leader to [" + Payload + "]. Found more than one match.");
	                    
					} else 
						SendAllChat("Only the leader [" + m_leader + "] can set a new leader.");                
				}
			}


//Add 1 Non-Admin Command in "NON ADMIN COMMANDS" section+++++++++++++++++++++++++++++++

	//
	// !CC
	//

	if ( CCStarted && (Command == "cc" || Command == "CC") && m_GameLoaded )
		  SendAllChat(classCallString);
	else if ( CCStarted && (Command == "cc" || Command == "CC") && !m_GameLoaded )
	{
		  for( unsigned int i = 0; i < 9; i++ )
		  {
			 if( m_Slots[i].GetSlotStatus() != 2 )
				CC[i] = "-";
			 else if (CC[i].length() <= 1)
				CC[i] = CCstatic[i];
		  }
		  classCallString = CC[0]+"/"+CC[1]+"/"+CC[2]+"/"+CC[3]+"/"+CC[4]+"/"+CC[5]+"/"+CC[6]+"/"+CC[7]+"/"+CC[8];
		  SendAllChat(classCallString);
	}

	//
	// !LEADER
	//

	if( Command == "leader" )
	{
		if (!IsLeader(User) && !AdminCheck && !IsOwner(User)) {
			if (m_leader.empty())
				SendAllChat("A leader has not yet been designated.");
			else
				SendAllChat("Your leader is: [" + m_leader + "].");
		}
	}