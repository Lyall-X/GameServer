#pragma once

class RobotManager :  public EventDispatcher
{
public:
	RobotManager();
	~RobotManager();

	Robot* add(Socket* socket, Robot* robot);
	void removeRobat(Socket* socket);
	Robot* getRobat(Socket* socket);
	void update(float32 time, float32 delay);
protected:
	int onNetSessionLeaveNotify(Robot* robot, NetSessionLeaveNotify* nfy);
	
	int onNetPingNotify(Robot* robot, NetPingNotify* nfy);
	int onNetPingGateNotify(Robot* robot, NetPingNotify* nfy);
	int onNetLoginRes(Robot* robot, NetLoginRes* res);
	int onNetCreateRoleRes(Robot* robot, NetCreateRoleRes* res);
	int onNetSelectRoleRes(Robot* robot, NetSelectRoleRes* res);


	int onNetEntityEnterMapNotify(Robot* robot, NetEntityEnterMapNotify* nfy);
	int onNetEntityLeaveMapNotify(Robot* robot, NetEntityLeaveMapNotify* nfy);
	int onNetPlayerEnterViewNotify(Robot* robot, NetPlayerEnterViewNotify* nfy);
	int onNetPlayerLeaveViewNotify(Robot* robot, NetPlayerLeaveViewNotify* nfy);
	int onNetEntityPropertyNotify(Robot* robot, NetEntityPropertyNotify* nfy);
	int onNetChatMsgNotify(Robot* robot, NetChatMsgNotify* nfy);
	int onNetFriendListRes(Robot* robot, NetFriendListRes* res);
	int onNetProductListRes(Robot* robot, NetProductListRes* res);

	int onNetMailListNotify(Robot* robot, NetMailListNotify* nfy);
protected:
	int32 onGmCmd(CmdEvent& e);
	int32 onLogin(CmdEvent& e);
	int32 onCreate(CmdEvent& e);
	int32 onSelect(CmdEvent& e);
	int32 onClose(CmdEvent& e);
	int32 onSendMsg(CmdEvent& e);
	int32 onPing(CmdEvent& e);
	int32 onPingGate(CmdEvent& e);
protected:
	std::map<Socket*, Robot*>		mMapSocketRobat;
	std::map<int32, Robot*>			mMapRobat;
public:
	Robot*							mCurRobot;
	char*			mMapData;
	float			mCell;
};

