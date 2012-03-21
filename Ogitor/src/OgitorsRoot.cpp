/*
 * �������ͨ������ô���ز���ģ���һ�νӴ�ô? �Ǿ���ϸ��������������
 *
 * �����������ģ�
 *	1�����ж�ϵͳ�Ƿ��Ѿ������ˣ��Ѽ����򷵻�
 *	2�������DLL���ص��ڴ��У����ؾ�� lib
 *	3����ȡDLL��ڵ�ַ pFunc������д���������ݽṹPLUGINENTRY
 *	4������һ���õ�����������PluginEntryMap�ṹ��
 *	5������ pFunc: pFunc(lib, pluginName)�������������
 *	6���������ʧ�ܣ��ͽ����ڴ�͡�ע�������
 *
 * ��������ϸ�ڣ����������ص㡣
 */

void OgitorsRoot::LoadPlugin(Ogre::String pluginPath)
{
    PluginEntryMap::iterator it = mPlugins.begin();

    while(it != mPlugins.end())
    {
        PLUGINENTRY ent = it->second;
        if(ent.mFileName == pluginPath)
            return;

        it++;
    }

/*
 * ע�����漸�д����Ŀ�ġ���ʽ�ܳ�������---����ƥ��Ĵ��롣
 * Ŀ�����ж�ϵͳ�Ƿ��Ѿ���������������
 * ��Щ���صĲ�����¼��PluginEntryMap����ṹ�С�
 */

    DynLib* lib;
    lib = OGRE_NEW DynLib( pluginPath );

// try �� catch ���������;

    try
    {
        lib->load();
    }
    catch(...)
    {
        OGRE_DELETE lib;
        return;
    }

    // Call startup function
    DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)lib->getSymbol("dllStartPlugin");

    PLUGINENTRY entry;
    entry.mName = "";
    entry.mFileName = pluginPath;
    entry.mLibrary = lib;
    entry.mFeatures = 0;
    entry.mEditorObjects.clear();
    entry.mSerializers.clear();
    mPlugins.insert(PluginEntryMap::value_type((void*)lib, entry));

    it = mPlugins.find(lib);

    Ogre::String pluginName = "";

    if(pFunc && pFunc(lib, pluginName))
    {
        it->second.mName = pluginName;
    }
    else
    {
        mPlugins.erase(it);
        lib->unload();
        OGRE_DELETE lib;
    }
}