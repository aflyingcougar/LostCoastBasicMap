modded class BasicMapController{
	bool IsInitLC = false;

	static string AirdropMarkersPath = "\\AirdropMarkers.json";
	static string CementMarkersPath = "\\CementMarkers.json";
	static string KeycardMarkersPath = "\\KeycardMarkers.json";

	static string AIRDROP_KEY = "BASICMAP_AIRDROP";
	static string CEMENT_KEY = "BASICMAP_CEMENT";
	static string KEYCARD_KEY = "BASICMAP_KEYCARD";

	override void Init(){
		// Make sure we don't break stuff
		super.Init();

		if(!IsInitLC){
			IsInitLC = true;
			//Print("[BasicMap] Debug: Add RPC 'RPCSyncLCData'");
			GetRPCManager().AddRPC("LostCoastBasicMap", "RPCSyncAirdropData", this, SingleplayerExecutionType.Both);
			GetRPCManager().AddRPC("LostCoastBasicMap", "RPCSyncCementData", this, SingleplayerExecutionType.Both);
			GetRPCManager().AddRPC("LostCoastBasicMap", "RPCSyncKeycardData", this, SingleplayerExecutionType.Both);

			Print("[BasicMap] Register Lost Coast Groups");
			RegisterGroup(AIRDROP_KEY, new BasicMapGroupMetaData(AIRDROP_KEY, "AIRDROPS"), NULL);
			RegisterGroup(CEMENT_KEY, new BasicMapGroupMetaData(CEMENT_KEY, "CEMENT MIXERS"), NULL);
			RegisterGroup(KEYCARD_KEY, new BasicMapGroupMetaData(KEYCARD_KEY, "KEYCARD ROOMS"), NULL);
		}

		if(GetGame().IsMultiplayer() && GetGame().IsClient()){
			Print("[BasicMap] Init Client LC Data");
			GetRPCManager().SendRPC("LostCoastBasicMap", "RPCSyncAirdropData", new Param3< array<autoptr BasicMapMarker>, array<autoptr BasicMapCircleMarker>, BasicMapConfig >(NULL, NULL, NULL ), true, NULL);
			GetRPCManager().SendRPC("LostCoastBasicMap", "RPCSyncCementData", new Param3< array<autoptr BasicMapMarker>, array<autoptr BasicMapCircleMarker>, BasicMapConfig >(NULL, NULL, NULL ), true, NULL);
			GetRPCManager().SendRPC("LostCoastBasicMap", "RPCSyncKeycardData", new Param3< array<autoptr BasicMapMarker>, array<autoptr BasicMapCircleMarker>, BasicMapConfig >(NULL, NULL, NULL ), true, NULL);
			LoadClientMarkers();
		}
		else {
			Print("[BasicMap] Init Server LC Data");
			if(!FileExist(BasicMapPath + AirdropMarkersPath)){
				MakeDirectory(BasicMapPath);
				GroupFirstRun(AIRDROP_KEY, AirdropMarkersPath);
				//Print("[BasicMap] Debug: FirstRun Airdrop");
			}
			else {
				LoadMarkers(AIRDROP_KEY, AirdropMarkersPath);
			}

			if(!FileExist(BasicMapPath + CementMarkersPath)){
				MakeDirectory(BasicMapPath);
				GroupFirstRun(CEMENT_KEY, CementMarkersPath);
				//Print("[BasicMap] Debug: FirstRun Cement");
			}
			else {
				LoadMarkers(CEMENT_KEY, CementMarkersPath);
			}

			if(!FileExist(BasicMapPath + KeycardMarkersPath)){
				MakeDirectory(BasicMapPath);
				GroupFirstRun(KEYCARD_KEY, KeycardMarkersPath);
				//Print("[BasicMap] Debug: FirstRun Keycard");
			}
			else {
				LoadMarkers(KEYCARD_KEY, KeycardMarkersPath);
			}
		}		
	}


	void RPCSyncAirdropData(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ){
		Param3<array<autoptr BasicMapMarker>, array<autoptr BasicMapCircleMarker>, ref BasicMapConfig> data;
		array<autoptr BasicMapMarker> markers = new array<autoptr BasicMapMarker>;
		array<autoptr BasicMapCircleMarker> circleMarkers = new array<autoptr BasicMapCircleMarker>;
		if ( !ctx.Read( data ) ) return;
        if ( GetGame().IsMultiplayer() && GetGame().IsClient() ){
			markers = data.param1;
			circleMarkers = data.param2;
			for ( int i = 0; i < markers.Count(); i++){
				markers.Get(i).SetCanEdit(false);
				markers.Get(i).SetGroup(AIRDROP_KEY);
			}
			for ( i = 0; i < circleMarkers.Count(); i++){
				circleMarkers.Get(i).SetCanEdit(false);
				circleMarkers.Get(i).SetGroup(AIRDROP_KEY);
				markers.Insert(circleMarkers.Get(i));
			}
			Markers.Set(AIRDROP_KEY, markers);
			m_BasicMapConfig = data.param3;
		} else if (GetGame().IsMultiplayer() && GetGame().IsServer()){
			if (Markers.Get(AIRDROP_KEY)){
				BasicMapCircleMarker cMarker;
				for ( int j = 0; j < Markers.Get(AIRDROP_KEY).Count(); j++){
					if (Markers.Get(AIRDROP_KEY).Get(j).IsInherited(BasicMapCircleMarker) && Class.CastTo(cMarker, Markers.Get(AIRDROP_KEY).Get(j)) && cMarker.GetRadius() > 0){
						circleMarkers.Insert(cMarker);
					} else {
						markers.Insert(Markers.Get(AIRDROP_KEY).Get(j));
					}
				}
			}
			GetRPCManager().SendRPC("LostCoastBasicMap", "RPCSyncAirdropData", new Param3< array<autoptr BasicMapMarker>,  array<autoptr BasicMapCircleMarker>, ref BasicMapConfig>( markers, circleMarkers, GetBasicMapConfig() ), true, sender);
		}
	}

	void RPCSyncCementData(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ){
		Param3<array<autoptr BasicMapMarker>, array<autoptr BasicMapCircleMarker>, ref BasicMapConfig> data;
		array<autoptr BasicMapMarker> markers = new array<autoptr BasicMapMarker>;
		array<autoptr BasicMapCircleMarker> circleMarkers = new array<autoptr BasicMapCircleMarker>;
		if ( !ctx.Read( data ) ) return;
        if ( GetGame().IsMultiplayer() && GetGame().IsClient() ){
			markers = data.param1;
			circleMarkers = data.param2;
			for ( int i = 0; i < markers.Count(); i++){
				markers.Get(i).SetCanEdit(false);
				markers.Get(i).SetGroup(CEMENT_KEY);
			}
			for ( i = 0; i < circleMarkers.Count(); i++){
				circleMarkers.Get(i).SetCanEdit(false);
				circleMarkers.Get(i).SetGroup(CEMENT_KEY);
				markers.Insert(circleMarkers.Get(i));
			}
			Markers.Set(CEMENT_KEY, markers);
			m_BasicMapConfig = data.param3;
		} else if (GetGame().IsMultiplayer() && GetGame().IsServer()){
			if (Markers.Get(CEMENT_KEY)){
				BasicMapCircleMarker cMarker;
				for ( int j = 0; j < Markers.Get(CEMENT_KEY).Count(); j++){
					if (Markers.Get(CEMENT_KEY).Get(j).IsInherited(BasicMapCircleMarker) && Class.CastTo(cMarker, Markers.Get(CEMENT_KEY).Get(j)) && cMarker.GetRadius() > 0){
						circleMarkers.Insert(cMarker);
					} else {
						markers.Insert(Markers.Get(CEMENT_KEY).Get(j));
					}
				}
			}
			GetRPCManager().SendRPC("LostCoastBasicMap", "RPCSyncCementData", new Param3< array<autoptr BasicMapMarker>,  array<autoptr BasicMapCircleMarker>, ref BasicMapConfig>( markers, circleMarkers, GetBasicMapConfig() ), true, sender);
		}
	}

	void RPCSyncKeycardData(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ){
		Param3<array<autoptr BasicMapMarker>, array<autoptr BasicMapCircleMarker>, ref BasicMapConfig> data;
		array<autoptr BasicMapMarker> markers = new array<autoptr BasicMapMarker>;
		array<autoptr BasicMapCircleMarker> circleMarkers = new array<autoptr BasicMapCircleMarker>;
		if ( !ctx.Read( data ) ) return;
        if ( GetGame().IsMultiplayer() && GetGame().IsClient() ){
			markers = data.param1;
			circleMarkers = data.param2;
			for ( int i = 0; i < markers.Count(); i++){
				markers.Get(i).SetCanEdit(false);
				markers.Get(i).SetGroup(KEYCARD_KEY);
			}
			for ( i = 0; i < circleMarkers.Count(); i++){
				circleMarkers.Get(i).SetCanEdit(false);
				circleMarkers.Get(i).SetGroup(KEYCARD_KEY);
				markers.Insert(circleMarkers.Get(i));
			}
			Markers.Set(KEYCARD_KEY, markers);
			m_BasicMapConfig = data.param3;
		} else if (GetGame().IsMultiplayer() && GetGame().IsServer()){
			if (Markers.Get(KEYCARD_KEY)){
				BasicMapCircleMarker cMarker;
				for ( int j = 0; j < Markers.Get(KEYCARD_KEY).Count(); j++){
					if (Markers.Get(KEYCARD_KEY).Get(j).IsInherited(BasicMapCircleMarker) && Class.CastTo(cMarker, Markers.Get(KEYCARD_KEY).Get(j)) && cMarker.GetRadius() > 0){
						circleMarkers.Insert(cMarker);
					} else {
						markers.Insert(Markers.Get(KEYCARD_KEY).Get(j));
					}
				}
			}
			GetRPCManager().SendRPC("LostCoastBasicMap", "RPCSyncKeycardData", new Param3< array<autoptr BasicMapMarker>,  array<autoptr BasicMapCircleMarker>, ref BasicMapConfig>( markers, circleMarkers, GetBasicMapConfig() ), true, sender);
		}
	}

	void LoadMarkers(string id, string path){
		array<autoptr BasicMapMarker> markers;
		JsonFileLoader< array<autoptr BasicMapMarker> >.JsonLoadFile(BasicMapPath + path, markers);
		if (markers){
			for (int i = 0; i < markers.Count(); i++){
				vector pos = markers.Get(i).GetPosition();
				if (pos[1] == 0) {
					pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
					markers.Get(i).SetPosition(pos);
				}
				markers.Get(i).SetCanEdit(false);
				markers.Get(i).SetGroup(id);
			}
			//Print("[BasicMap] Debug: Set Markers " + id);
			Markers.Set(id, markers);
		}
	}

	void GroupFirstRun(string id, string path){
		array<autoptr BasicMapMarker> markers = new array<autoptr BasicMapMarker>;
		markers.Insert(new BasicMapMarker("GREEN MOUNTAIN", Vector(3693.56, 402.312,6010.05), "BasicMap\\gui\\images\\marker.paa", {212, 138, 251}, 240, true));
		for (int i = 0; i < markers.Count(); i++){
			markers.Get(i).SetCanEdit(false);
			markers.Get(i).SetGroup(id);
		}
		Markers.Set(id, markers);
		JsonFileLoader< array<autoptr BasicMapMarker> >.JsonSaveFile(BasicMapPath + path, markers);
	}
}

