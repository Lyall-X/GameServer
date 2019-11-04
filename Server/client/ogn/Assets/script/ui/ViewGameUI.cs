using UnityEngine;
using UnityEngine.UI;

using System.Collections;
using System.Collections.Generic;
public partial class SocketHandler : EventDispatcher
{
    //public int onNetCreateRoomRes(object l, object w)
    //{
    //    SocketAngent socket = l as SocketAngent;
    //    NetCreateRoomRes res = w as NetCreateRoomRes;
    //    ViewRoomUI viewRoomUI =  GameStart.uiMgr.openWindow("room_ui").GetComponent<ViewRoomUI>();
    //    viewRoomUI.onNetCreateRoomRes(res);

    //    return 1;
    //}

    //public int onNetEnterRoomRes(object l, object w)
    //{
    //    SocketAngent socket = l as SocketAngent;
    //    NetEnterRoomRes res = w as NetEnterRoomRes;
    //    ViewRoomUI viewRoomUI = GameStart.uiMgr.openWindow("room_ui").GetComponent<ViewRoomUI>();
    //    viewRoomUI.onNetEnterRoomRes(res);


    //    return 0;
    //}

    //public int onNetLeaveRoomRes(object l, object w)
    //{
    //    SocketAngent socket = l as SocketAngent;
    //    NetLeaveRoomRes res = w as NetLeaveRoomRes;
    //    ViewRoomUI viewRoomUI = GameStart.uiMgr.openWindow("room_ui").GetComponent<ViewRoomUI>();
    //    viewRoomUI.onNetLeaveRoomRes(res);
       
    //    return 0;
    //}

    //public int onNetDismissRoomRes(object l, object w)
    //{
    //    SocketAngent socket = l as SocketAngent;
    //    NetDismissRoomRes res = w as NetDismissRoomRes;
    //    ViewRoomUI viewRoomUI = GameStart.uiMgr.openWindow("room_ui").GetComponent<ViewRoomUI>();
    //    viewRoomUI.onNetDismissRoomRes(res);
    //    return 0;
    //}
}

public class ViewGameUI : MonoBehaviour {

    public GameObject exitObject;
    public Button exitButton;
    public List<GameObject> listPlayers = new List<GameObject>();
    //public List<RoomPlayerInfo> listPlayerInfos = new List<RoomPlayerInfo>();
    public int roomId = 0;
    public int roomMaster = 0;
    public GameObject playerListObject;
    public Text roomNumber;
    public Text popleNumber;
    public int maxPople = 3;
    void Awake()
    {
        /*
        exitObject = gameObject.transform.Find("exit").gameObject;
        exitButton = exitObject.GetComponent<Button>();
        //exitButton.onClick.AddListener(() => onExitClick(exitButton));
        roomNumber = gameObject.transform.Find("room_info/room_number").GetComponent<Text>();
        popleNumber = gameObject.transform.Find("room_info/pople_number").GetComponent<Text>();
        playerListObject = gameObject.transform.Find("player_list").gameObject;
        listPlayers.Clear();
        //listPlayerInfos.Clear();

        for (int i = 0; i < playerListObject.transform.childCount; ++i)
        {
            listPlayers.Add(playerListObject.transform.GetChild(i).gameObject);
            listPlayers[i].SetActive(false);
        }
        */
        //joinBtn.onClick.AddListener(() => onJoinClick(createBtn));  
    }
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}
    //void onExitClick(Button button)
    //{
    //    NetLeaveRoomReq req = new NetLeaveRoomReq();
    //    req.roomId = roomId;
    //    PlayerManager.playerMgr.localPlayer.Send(req);
    //}

    //public  void RefreshPlayerList(List<RoomPlayerInfo> playerInfos)
    //{
    //    for (int i = 0; i < listPlayers.Count; ++i)
    //    {
    //        listPlayers[i].SetActive(false);
    //    }
    //    for (int i = 0; i < playerInfos.Count; ++i)
    //    {
    //        listPlayers[i].SetActive(true);
    //        RoomPlayerInfo info = playerInfos[i];
    //        Text textName = listPlayers[i].transform.Find("name").GetComponent<Text>();
    //        textName.text = info.name;
    //        listPlayerInfos.Add(info);
    //    }
    //}

    //public void AddPlayerToRoom(RoomPlayerInfo info)
    //{
    //    if (listPlayerInfos.Count == maxPople)
    //        return;

    //    if (getRoomPlayerInfo(info.playerId) != null)
    //    {
    //        Debug.Log("房间中已经存在" + info.playerId);
    //        return;
    //    }
    //    int index = listPlayerInfos.Count;
    //    listPlayerInfos.Add(info);
    //    listPlayers[index].SetActive(true);
    //    Text textName = listPlayers[index].transform.Find("name").GetComponent<Text>();
    //    textName.text = info.name;
    //}

    //public RoomPlayerInfo getRoomPlayerInfo(int playerId)
    //{
    //    foreach(RoomPlayerInfo info in listPlayerInfos)
    //    {
    //        if (playerId == info.playerId)
    //            return info;
    //    }

    //    return null;
    //}

    //public void PlayerRemoveRoom(int playerId)
    //{
    //    for (int i = 0; i < listPlayerInfos.Count; ++i)
    //    {
    //        if (listPlayerInfos[i].playerId == playerId)
    //        {
    //            listPlayers[i].SetActive(false);
    //            listPlayerInfos.RemoveAt(i);
    //            break;
    //        }
    //    }

    //    if (listPlayerInfos.Count <= 0)
    //    {
    //        GameStart.uiMgr.openWindow("main_ui");
    //        return;
    //    }

    //    if (playerId == PlayerManager.playerMgr.localPlayer.playerId)
    //    {
    //        GameStart.uiMgr.openWindow("main_ui");
    //        return;
    //    }

    //}


    
    //public int onNetCreateRoomRes(NetCreateRoomRes res)
    //{
    //    GameStart.uiMgr.openWindow("room_ui");

    //    roomId = res.roomInfo.roomId;
    //    roomMaster = res.roomInfo.masterId;

    //    roomNumber.text = "ID:" + roomId.ToString();

    //    RefreshPlayerList(res.roomInfo.playerInfos);

    //    popleNumber.text = listPlayerInfos.Count.ToString() +  "/" + maxPople.ToString();
    //    return 0;
    //}
    //public int onNetLeaveRoomRes(NetLeaveRoomRes res)
    //{
    //    if (res.result == 0)
    //    {
    //        PlayerRemoveRoom(res.playerId);
    //    }
    //    return 0;
    //}

    //public int onNetEnterRoomRes(NetEnterRoomRes res)
    //{
    //    GameStart.uiMgr.openWindow("room_ui");

    //    roomNumber.text = "ID:" + roomId.ToString();

    //    RoomPlayerInfo info = null;
    //    for (int i = 0; i < res.roomInfo.playerInfos.Count; ++i)
    //    {
    //        if (res.roomInfo.playerInfos[i].playerId == PlayerManager.playerMgr.localPlayer.playerId)
    //        {
    //            info = res.roomInfo.playerInfos[i];
    //            break;
    //        }
    //    }
    //    roomId = res.roomInfo.roomId;
    //    roomMaster = res.roomInfo.masterId;
    //    if (info != null)
    //        AddPlayerToRoom(info);

    //    for (int i = 0; i < res.roomInfo.playerInfos.Count; ++i)
    //    {
    //        if (info != null && info.playerId == res.roomInfo.playerInfos[i].playerId)
    //            continue;

    //        AddPlayerToRoom(res.roomInfo.playerInfos[i]);
    //    }

    //    popleNumber.text = listPlayerInfos.Count.ToString() + "/" + maxPople.ToString();
    //    return 0;
    //}

    //public int onNetDismissRoomRes(NetDismissRoomRes res)
    //{
    //    GameStart.uiMgr.openWindow("main_ui");
    //    return 0;
    //}
}
