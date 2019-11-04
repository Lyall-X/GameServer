using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


public class EventTarget
{
    public string name = "";
   	public EventDispatcher targetDispatcher = null;
	public EventRegister targetRegister = null;
}

public class EventRegister
{
    public static int s_id = 0;
    public int id = 0;
    public string name = "";
    public EventDispatcher.CallBack callback = null;
    public object param = null;

    public EventRegister()
    {
        this.id = ++s_id;
    }
    public bool equalListener(string name, EventDispatcher.CallBack callback)
    {
        if (this.name == name && this.callback == callback)
		    return true;

	    return false;
    }
}

public class EventInvoke
{
    public virtual void Invoke(object p1)
    {

    }

    public virtual void Invoke(object p1, object p2)
    {

    }

    public virtual void Invoke(object p1, object p2, object p3)
    {

    }

    public virtual void Invoke(object p1, object p2, object p3, object p4)
    {

    }
}

public class EventCallBack1<T1> : EventInvoke
{
    public delegate int Callback(T1 lprama);
    public Callback callback;
    public EventCallBack1(Callback cb)
    {
        callback = cb;
    }
    public override void Invoke(object t1)
    {
        T1 p1 = (T1)t1;
        callback.Invoke(p1);
    }
}

public class EventCallBack2<T1, T2> : EventInvoke
{
    public delegate int Callback(T1 lprama, T2 wprama);
    public Callback callback;
    public EventCallBack2(Callback cb)
    {
        callback = cb;
    }
    public override void Invoke(object t1, object t2)
    {
        T1 p1 = (T1)t1;
        T2 p2 = (T2)t2;
        callback.Invoke(p1, p2);
    }
}

public class EventDispatcher
{
    public delegate int CallBack(EventTarget e);
    protected Dictionary<string, List<EventRegister>> listener = new Dictionary<string, List<EventRegister>>();
    protected Dictionary<int, List<EventInvoke>> listener_ = new Dictionary<int, List<EventInvoke>>();

    public EventDispatcher() {

    }

    public int addEventListener(string name, CallBack callback, object param = null)
    {
        EventRegister lis = new EventRegister();
        lis.name = name;
	    lis.callback = callback;
	    lis.param = param;
        List<EventRegister> listeners = null;
        if (listener.ContainsKey(name)) {
            listeners = listener[name];
        }
        else {
            listeners = new List<EventRegister>();
            listener[name] = listeners;
        }

        listeners.Add(lis);

        return lis.id;
    }

    public int addEventListener(int id, EventInvoke callback)
    {
        List<EventInvoke> listeners = null;
        if (listener_.ContainsKey(id)) {
            listeners = listener_[id];
        }
        else
        {
            listeners = new List<EventInvoke>();
            listener_[id] = listeners;
        }
        listeners.Add(callback);
        return 0;
    }

    public int removeEventListener(string name, CallBack callback)
    {
        List<EventRegister> listeners = null;
        if (!listener.ContainsKey(name))
            return 0;
           
        listeners = listener[name];
        for (int i = 0; i < listeners.Count; ++i )
        {
            if (listeners[i].equalListener(name, callback))
            {
                listeners.RemoveAt(i);
                return 0;
            }
        }
        return 0;
    }
	public int dispatchEvent(EventTarget e)
    {
        List<EventRegister> listeners = null;
        if (!listener.ContainsKey(e.name))
            return 0;
           
        listeners = listener[e.name];
        for (int i = 0; i < listeners.Count; ++i)
        {
            e.targetDispatcher = this;
            e.targetRegister = listeners[i];
            listeners[i].callback.Invoke(e);
            //listeners[i].thisObject.(listeners[i].callback)(e);
        }
        return 0;
    }

    public int dispatchEvent(int id, object p1)
    {
        List<EventInvoke> listeners = null;
        if (!listener_.ContainsKey(id))
            return 0;

        listeners = listener_[id];
        for (int i = 0; i < listeners.Count; ++i) {
            listeners[i].Invoke(p1);
        }
        return 0;
    }

    public int dispatchEvent(int id, object p1, object p2)
    {
        List<EventInvoke> listeners = null;
        if (!listener_.ContainsKey(id))
            return 0;

        listeners = listener_[id];
        for (int i = 0; i < listeners.Count; ++i)
        {
            listeners[i].Invoke(p1, p2);
        }
        return 0;
    }

}
