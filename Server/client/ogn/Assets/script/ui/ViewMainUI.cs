using UnityEngine;
using System.Collections;
using UnityEngine.UI;


public class ViewMainUI : MonoBehaviour {

	// Use this for initialization

    public GameObject createObj;
    public Button createBtn;
    public GameObject joinObj;
    public Button joinBtn;

    public GameObject main;

    public GameObject roomList;
    public GameObject roomListGrid;

    public GameObject EnterRoomPlane;
    public Button EnterRoomPlaneClose;
    public Button EnterRoomPlaneEnter;
    public InputField EnterRoomPlaneInput;
    public Text textPlayerName;
    public Text textPlayerId;
    public Text textPlayerGold;
    public Image imagePlayerSex;
    void Awake()
    {
        /*
        main = gameObject.transform.Find("main").gameObject;
        createObj = main.transform.Find("create").gameObject;
        createBtn = createObj.GetComponent<Button>();
        joinObj = main.transform.Find("join").gameObject;
        joinBtn = joinObj.GetComponent<Button>();

        textPlayerName = gameObject.transform.Find("info/name").GetComponent<Text>();
        textPlayerId = gameObject.transform.Find("info/id").GetComponent<Text>();
        textPlayerGold = gameObject.transform.Find("info/gold").GetComponent<Text>();
        imagePlayerSex = gameObject.transform.Find("info/head/sex").GetComponent<Image>();

        EnterRoomPlane = gameObject.transform.Find("EnterRoomPlane").gameObject;
        EnterRoomPlane.SetActive(false);
        EnterRoomPlaneClose = EnterRoomPlane.transform.Find("Close").GetComponent<Button>();
        EnterRoomPlaneEnter = EnterRoomPlane.transform.Find("Enter").GetComponent<Button>();
        EnterRoomPlaneInput = EnterRoomPlane.transform.Find("InputField").GetComponent<InputField>();
        
        roomList = gameObject.transform.Find("roomList").gameObject;
        roomListGrid = roomList.transform.Find("Grid").gameObject;
        */
        //createBtn.onClick.AddListener(() => onCreateClick(createBtn));
        //joinBtn.onClick.AddListener(() => onJoinClick(joinBtn));

        //EnterRoomPlaneClose.onClick.AddListener(() => onCloseClick(EnterRoomPlaneClose));
        //EnterRoomPlaneEnter.onClick.AddListener(() => onEnterClick(EnterRoomPlaneEnter));  
    }

	void Start () {


	}
	
	// Update is called once per frame
	void Update () {
	
	}
    /*
    void onCreateClick(Button button)
    {
        NetCreateRoomReq req = new NetCreateRoomReq();
        PlayerManager.playerMgr.localPlayer.Send(req);
    }

    void onJoinClick(Button button)
    {
        EnterRoomPlane.SetActive(true);
        Debug.Log("onJoinClick");
    }

    void onCloseClick(Button button)
    {
        EnterRoomPlane.SetActive(false);
    }

    void onEnterClick(Button button)
    {
        NetEnterRoomReq req = new NetEnterRoomReq();
        req.roomId = int.Parse(EnterRoomPlaneInput.text);
        PlayerManager.playerMgr.localPlayer.Send(req);

        EnterRoomPlane.SetActive(false);
    }

    public void RefreshInfo(Player player)
    {
        textPlayerName.text = player.name;
        textPlayerId.text = "ID:" + player.playerId.ToString();

        PlayerProperty property = player.getProperty("PlayerProperty") as PlayerProperty;
        textPlayerGold.text = "金币:" + property.gold.ToString();

        if (property.sex == 1)
        {
            //imagePlayerSex.sprite = 
            //imagePlayerSex;
        }
        else
        {

        }
    }
    */
}
