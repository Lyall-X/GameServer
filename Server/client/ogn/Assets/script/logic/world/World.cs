using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class World : Singleton<World>
{
    public Dictionary<int, Entity> dictEntity = new Dictionary<int, Entity>();
    public Dictionary<long, Entity> dictEntityIns = new Dictionary<long, Entity>();
    public World()
    {

    }

    public Entity addEntity(Entity ent)
    {
        if (dictEntity.ContainsKey(ent.InsId))
            return null;
        dictEntity.Add(ent.InsId, ent);
        dictEntityIns.Add(ent.Guid, ent);
        return ent;
    }

    public void removeEntity(int insId)
    {
        Entity entity = null;
        if (!dictEntity.ContainsKey(insId))
            return;
        entity = dictEntity[insId];

        if (dictEntityIns.ContainsKey(entity.Guid))
            dictEntityIns.Remove(entity.Guid);

        entity.Destory();
        dictEntity.Remove(insId);
    }

    public Entity FindEntityByInsId(int insId)
    {
        if (dictEntity.ContainsKey(insId))
            return dictEntity[insId];
        return null;
    }
    public Entity FindEntityByGuid(long guid)
    {
        if (dictEntityIns.ContainsKey(guid))
            return dictEntityIns[guid];
        return null;
    }


    public bool update()
    {
        return true;
    }
}
